#include "../include/meta_explorer.h"

MetaFileInfo* getMetaFileInfo(char* metaFileName)
{
    MetaFileInfo* metaFileInfo = NULL;
    printf("\nFILE       : %s",metaFileName);

    FILE* metaFile = openFile(metaFileName,"rb+");
    metaFileInfo = (MetaFileInfo*) calloc(1,sizeof(MetaFileInfo));

    fseek(metaFile,sizeof(long),SEEK_SET);
    fread(&metaFileInfo->pazCount,sizeof(long),1,metaFile);
    printf("\nPAZ_COUNT  : %ld", metaFileInfo->pazCount);


    fseek(metaFile,(metaFileInfo->pazCount * (3 * sizeof(long))),SEEK_CUR);
    fread(&metaFileInfo->fileBlocksCount,sizeof(long),1,metaFile);
    printf("\nFILES_COUNT: %ld\n", metaFileInfo->fileBlocksCount);

    metaFileInfo->fileBlocksStart = ftell(metaFile);
    metaFileInfo->fileBlocksEnd = metaFileInfo->fileBlocksStart + (metaFileInfo->fileBlocksCount * ONE_REGISTRY);

    printf("\nFILE_BLOCKS_START: %ld \n", metaFileInfo->fileBlocksStart);
    printf("FILE_BLOCKS_END  : %ld\n\n", metaFileInfo->fileBlocksEnd);

    fclose(metaFile);

    return metaFileInfo;
}

long* getFileBlocksHashes(char* metaFileName, long* hashesCount)
{
    long* hashes = NULL;
    FILE* metaFile = fopen(metaFileName,"rb+");
    if (!metaFile)
    {
        return hashes;
    }

    long pazCount = 0;
    fseek(metaFile,sizeof(long),SEEK_SET);
    fread(&pazCount,sizeof(long),1,metaFile);

    fseek(metaFile,(pazCount * (3 * sizeof(long))),SEEK_CUR);
    fread(hashesCount,sizeof(long),1,metaFile);

    hashes = (long*) malloc((*hashesCount) * sizeof(long));
    int i = 0;
    for(i = 0 ; i < (*hashesCount); i++)
    {
        fread(&hashes[i],sizeof(long),1,metaFile);
        fseek(metaFile,(6 * sizeof(long)),SEEK_CUR);
    }
    qsort(hashes,(*hashesCount),sizeof(long),long_sort);
    return hashes;
}

FileBlock* fillFileBlocks(MetaFileInfo* metaFileInfo, char* metaFileName, int useArmorNamesFile)
{
    printf("Filling File Blocks...");
      /// Allocates Fileblocks
    FileBlock* fileBlocks = NULL;
    int i = 0;

    fileBlocks = (FileBlock*)calloc(metaFileInfo->fileBlocksCount + 1, sizeof(FileBlock));

    FILE* metaFile = openFile(metaFileName,"rb");

    // Go to where the file blocks start
    fseek(metaFile,metaFileInfo->fileBlocksStart,SEEK_SET);

   // Fill the File Blocks
   for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
   {
        // Saves the exact byte where this registry begins (VERY IMPORTANT)
        fileBlocks[i].metaOffset = ftell(metaFile);

        // Reads the next 28 bytes of the meta file and stores it
        fread(&fileBlocks[i].hash,sizeof(long),1,metaFile);
        fread(&fileBlocks[i].folderNum,sizeof(long),1,metaFile);
        fread(&fileBlocks[i].fileNum,sizeof(long),1,metaFile);
        fread(&fileBlocks[i].pazNum,sizeof(long),1,metaFile);
        fread(&fileBlocks[i].fileOffset,sizeof(long),1,metaFile);
        fread(&fileBlocks[i].zsize,sizeof(long),1,metaFile);
        fread(&fileBlocks[i].size,sizeof(long),1,metaFile);
   }
   printf("\nDone.\n");

    printf("\nDecrypting Folder Names...\n");
    long folders_part_length = 0;
    fread(&folders_part_length, sizeof(long),1,metaFile);
    printf("Encrypted folder names total length: %ld\n\n", folders_part_length);


/// ******************* FOLDER NAMES DECRYPTION **********************


    uint8_t *ctext = (uint8_t *) malloc(folders_part_length); // alloc to meet our need.
    uint8_t *ptext = (uint8_t *) malloc(folders_part_length);

    uint8_t *ctext_backup_pointer = ctext;
    uint8_t *ptext_backup_pointer = ptext;

    long bytesRead = fread(ctext, 1, folders_part_length, metaFile);

    if (bytesRead != folders_part_length)
    {
          printf ("Error:\nTotal number of bytes read: %ld\nExpected: %ld\n", bytesRead, folders_part_length);
          printf("\nWarning: Meta file Info is not valid.");
          Sleep(3000);
          return fileBlocks;
    }

    const uint8_t *decrypction_key = (const uint8_t*)"\x51\xF3\x0F\x11\x04\x24\x6A\x00"; // The Black Desert ICE decryption key

    ICE_KEY *ik = ice_key_create(0); // init the key
    ice_key_set(ik, decrypction_key); // set the key

    long cuts = folders_part_length/8;
    while (cuts--){
        ice_key_decrypt(ik, ctext, ptext); // key, in (encrypted text), out (decrypted text)

        ctext +=8;
        ptext +=8;
    }

    ptext -= folders_part_length; // reset the pointer back to the beginning.



/// FOLDER NAMES FILLING


     // fileNamesArray has all the decrypted file names in order as they are read, they will be assigned to the fileBlocks later
    metaFileInfo->folderNames = (char**) malloc(metaFileInfo->fileBlocksCount * sizeof(char*));

    // Until the \0 is found, copy the letters to the folderNamesArray[i]

    int j = 0; // The current byte of the decrypted text
    int k = 0; // The current letter of the folderNamesArray[i]
    printf("Saving Folder Names...\n");

    i = 0; // Used to advance the fileNamesArray
    long folderNameLength = 0;
    for(j = (2 * sizeof(long)) /* Skips the first 2 numbers */; j < folders_part_length; j++)
    {
        // Prevents it from going out of the fileNamesArray allocated memory
        if (i + 1 > metaFileInfo->fileBlocksCount)
        {
            break;
        }

        // If it's not a \0
        if (ptext[j] != 0)
        {
            folderNameLength++;
        }
        else // If the \0 is found
        {
            metaFileInfo->folderNames[i] = (char*) malloc((folderNameLength * sizeof(char)) + 1);
            j -= folderNameLength;
            for(k = 0; k < folderNameLength; k++)
            {
                metaFileInfo->folderNames[i][k] = ptext[j];
                //printf("%c", folderNamesArray[i][k]);
                j++;
            }

            metaFileInfo->folderNames[i][k] = '\0'; // Ends the string

            i++; // Advances the fileNameArray position
            folderNameLength = 0;
            j += (2 * sizeof(long)); /* Skips the first 2 numbers */
        }
    }

    metaFileInfo->folderNamesCount = i;
    printf("Folder Names saved: %d\n\n", metaFileInfo->folderNamesCount);

    // Assigns the right File Name from the fileNameArray to the right File Block, based on the File Num
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        // Allocates FolderName memory
        fileBlocks[i].folderName = (char*) malloc((strlen(metaFileInfo->folderNames[fileBlocks[i].folderNum]) * sizeof(char)) + 1);

        // Goes to the folderNum position indicated in "fileBlocks[i].folderNum" and copies the folder name to the File Block
        strcpy(fileBlocks[i].folderName,metaFileInfo->folderNames[fileBlocks[i].folderNum]);

        // Ends the string
        fileBlocks[i].folderName[strlen(metaFileInfo->folderNames[fileBlocks[i].folderNum])] = '\0';
    }

    free(ctext_backup_pointer);
    free(ptext_backup_pointer);



/// ******************* FILE NAMES DECRYPTION **********************

    printf("Decrypting File Names...\n");

    long filenames_part_length = 0;
    fread(&filenames_part_length, sizeof(long),1,metaFile);
    printf("Encrypted file names total length: %ld\n\n", filenames_part_length);


    ctext = (uint8_t *) malloc(filenames_part_length); // alloc to meet our need.
    ptext = (uint8_t *) malloc(filenames_part_length);

    ctext_backup_pointer = ctext;
    ptext_backup_pointer = ptext;



    // bytesRead is the real bytes filenames_part_length that program read out, but should be same as "filenames_part_length".
    bytesRead = fread(ctext, 1, filenames_part_length, metaFile);

    if (bytesRead != filenames_part_length)
    {
          printf ("Error:\nTotal number of bytes read: %ld\nExpected: %ld\n", bytesRead, filenames_part_length);
          printf("\nWarning: Meta file Info is not valid.");
          Sleep(3000);
          return fileBlocks;
    }

    cuts = filenames_part_length/8;
    while (cuts--){
        ice_key_decrypt(ik, ctext, ptext); // key, in (encrypted text), out (decrypted text)

        ctext +=8;
        ptext +=8;
    }

    ptext -= filenames_part_length; // reset the pointer back to the begining.


/// FILE NAMES FILLING

    // fileNamesArray has all the decrypted file names in order as they are read, they will be assigned to the fileBlocks later
    char** fileNamesArray = (char**) malloc(metaFileInfo->fileBlocksCount * sizeof(char*));

    // Until the \0 is found, copy the letters to the fileName[i]

    j = 0; // The current byte of the decrypted text
    k = 0; // The current letter of the fileName[i]
    printf("Saving file names...\n");

    i = 0; // Used to advance the fileNamesArray
    long fileNameLength = 0;
    for(j = 0; j < filenames_part_length; j++)
    {

        // Prevents it from going out of the fileNamesArray allocated memory
        if (i + 1 > metaFileInfo->fileBlocksCount)
        {
            break;
        }

        // If it's not a \0
        if (ptext[j] != 0)
        {
            fileNameLength++;
        }
        else // If the \0 is found
        {
            fileNamesArray[i] = (char*) malloc((fileNameLength * sizeof(char)) + 1);
            j -= fileNameLength;
            for(k = 0; k < fileNameLength; k++)
            {
                fileNamesArray[i][k] = ptext[j];
                //printf("%c", fileNamesArray[i][k]);
                j++;
            }

            fileNamesArray[i][k] = '\0'; // Ends the string
            i++; // Advances the fileNameArray position
            fileNameLength = 0;
        }
    }
    FileBlock** armorFileBlocks = NULL;

    if (useArmorNamesFile)
    {
        armorFileBlocks = (FileBlock**) malloc(metaFileInfo->fileBlocksCount * sizeof(FileBlock*));
    }

    int armorFileBlocksCount = 0;

    // Assigns the right File Name from the fileNameArray to the right File Block, based on the File Num
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {

       // Allocates fileName memory
        fileBlocks[i].fileName = (char*) malloc((strlen(fileNamesArray[fileBlocks[i].fileNum]) * sizeof(char)) + 1);

        // Goes to the fileNum position indicated in "fileBlocks[i].fileNum" and copies the file name to the File Block
        strcpy(fileBlocks[i].fileName,fileNamesArray[fileBlocks[i].fileNum]);

        // Ends the string
        fileBlocks[i].fileName[strlen(fileNamesArray[fileBlocks[i].fileNum])] = '\0';


        // Allocates FolderName memory
        fileBlocks[i].folderName = (char*) malloc((strlen(metaFileInfo->folderNames[fileBlocks[i].folderNum]) * sizeof(char)) + 1);

        // Goes to the folderNum position indicated in "fileBlocks[i].folderNum" and copies the folder name to the File Block
        strcpy(fileBlocks[i].folderName,metaFileInfo->folderNames[fileBlocks[i].folderNum]);

        // Ends the string
        fileBlocks[i].folderName[strlen(metaFileInfo->folderNames[fileBlocks[i].folderNum])] = '\0';

        if (useArmorNamesFile)
        {
            if (strstr(fileBlocks[i].folderName,"9_upperbody"))
            {
                armorFileBlocks[armorFileBlocksCount] = &fileBlocks[i];
                if (!isRegular(armorFileBlocks[armorFileBlocksCount]->fileName))
                {
                    armorFileBlocks[armorFileBlocksCount]->isClassExclusive = 1;
                }
                armorFileBlocksCount++;
            }
        }
    }
    printf("File Names saved: %d (%ld less than expected)\n", i, metaFileInfo->fileBlocksCount - i);

    if (useArmorNamesFile)
    {
    printf("\nMarking class-exclusive armors...");
    qsort(armorFileBlocks,armorFileBlocksCount,sizeof(FileBlock*),sort_by_file_name_asc);

    for (i = 0; i < armorFileBlocksCount; i++)
    {
        for(j = i + 1; j < armorFileBlocksCount - 1; j++)
        {
            if (strstr(armorFileBlocks[i]->fileName,removeClassPrefix(armorFileBlocks[j]->fileName)))
            {
                armorFileBlocks[i]->isClassExclusive = 0;
                armorFileBlocks[j]->isClassExclusive = 0;
            }
        }
    }
    printf("\nDone\n");
    free(armorFileBlocks);

        printf("\nSorting Folder Names...");
        // QUICK SORT
        qsort(metaFileInfo->folderNames,metaFileInfo->folderNamesCount,sizeof(char*),sort_by_name_asc);
        printf("\nDone.\n");
    }

    printf("\nSorting File Blocks...");
    // QUICK SORT
    qsort(fileBlocks,metaFileInfo->fileBlocksCount,sizeof(FileBlock),sort_by_extension_pac_dds_priority_simple_pointer_desc);
    printf("\nDone.\n");

    metaFileInfo->pacFileBlocksEnd = 0;
    metaFileInfo->textureFileBlocksStart = 0;
    metaFileInfo->textureFileBlocksEnd = 0;
    printf("\nDetermining ext transitions...");
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (metaFileInfo->pacFileBlocksEnd == 0)
        {
            if (fileBlocks[i].fileName[strlen(fileBlocks[i].fileName)-1] != 'c')
            {
                metaFileInfo->pacFileBlocksEnd = i - 1;
                metaFileInfo->textureFileBlocksStart = i;
            }
        }
        if (metaFileInfo->textureFileBlocksStart != 0)
        {
            if (fileBlocks[i].fileName[strlen(fileBlocks[i].fileName)-1] != 's')
            {
                metaFileInfo->textureFileBlocksEnd = i - 1;
                break;
            }
        }
    }
    printf("\nDone\n");

    /*printf("\nstart: %s (%d)", fileBlocks[metaFileInfo->pacFileBlocksStart].fileName,metaFileInfo->pacFileBlocksStart);
    printf("\n%s (%d)", fileBlocks[metaFileInfo->pacFileBlocksEnd-1].fileName,metaFileInfo->pacFileBlocksEnd-1);
    printf("\nend: %s (%d)", fileBlocks[metaFileInfo->pacFileBlocksEnd].fileName,metaFileInfo->pacFileBlocksEnd);
    printf("\n%s (%d)", fileBlocks[metaFileInfo->pacFileBlocksEnd + 1].fileName,metaFileInfo->pacFileBlocksEnd+1);

    printf("\n\n%s (%d)", fileBlocks[metaFileInfo->textureFileBlocksStart - 1].fileName,metaFileInfo->textureFileBlocksStart - 1);
    printf("\nstart:%s (%d)", fileBlocks[metaFileInfo->textureFileBlocksStart].fileName,metaFileInfo->textureFileBlocksStart);

    printf("\n\n%s (%d)", fileBlocks[metaFileInfo->textureFileBlocksEnd - 1].fileName,metaFileInfo->textureFileBlocksEnd - 1);
    printf("\nend:%s (%d)", fileBlocks[metaFileInfo->textureFileBlocksEnd].fileName,metaFileInfo->textureFileBlocksEnd);
    printf("\n%s (%d)", fileBlocks[metaFileInfo->textureFileBlocksEnd+1].fileName,metaFileInfo->textureFileBlocksEnd+1);

    PAUSE();*/

    fclose(metaFile);

    /*for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        printf("%ld|%ld|%ld|%ld|%s|%s\n",fileBlocks[i].hash,fileBlocks[i].folderNum,fileBlocks[i].fileNum,fileBlocks[i].pazNum,fileBlocks[i].folderName,fileBlocks[i].fileName);
    }*/



    free(ctext_backup_pointer);
    free(ptext_backup_pointer);


    for(i = 0; i < metaFileInfo->fileBlocksCount ; i++)
    {
        free(fileNamesArray[i]);
    }
    free(fileNamesArray);

    return fileBlocks;
}

char** fillFolderNamesList(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, int* folderNamesCount)
{
    int i = 0;
    (*folderNamesCount) = 1;
    char** folderNamesList = NULL;
    for (i = 1; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (strcmp(fileBlocks[i - 1].folderName,fileBlocks[i].folderName) != 0)
        {
            (*folderNamesCount)++;
        }
    }

    //printf("\nfolderNamesCount: %d\n", (*folderNamesCount));


    folderNamesList = (char**) malloc(((*folderNamesCount)+10) * sizeof(char*));

    folderNamesList[0] = (char*) malloc(strlen(fileBlocks[0].folderName + 1));
    strcpy(folderNamesList[0],fileBlocks[0].folderName);

    int currentFolderName = 0;
    for (i = 1; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (strcmp(fileBlocks[i - 1].folderName,fileBlocks[i].folderName) != 0)
        {
            folderNamesList[currentFolderName] = (char*) malloc(strlen(fileBlocks[i].folderName + 1));
            strcpy(folderNamesList[currentFolderName],fileBlocks[i].folderName);
            currentFolderName++;
        }
    }

    (*folderNamesCount)--;

    return folderNamesList;
}


struct Folder* fillFolderTree(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo)
{
    int i = 0;
    struct Folder *rootFolder = NULL;

     for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (contains(fileBlocks[i].folderName,"character/model/1_pc"))
        {
            ECHO = 0;
        }
        else
        {
            ECHO = 0;
        }
        if (i == 0)
        {
            // First insertion
            if (ECHO)
            printf("\n----------------------------\nfileBlock.folderName: %s\n",fileBlocks[i].folderName);

             // Removes the / at the end
            fileBlocks[i].folderName[strlen(fileBlocks[i].folderName) - 1] = '\0';
            insertFolder(fileBlocks[i].folderName,&rootFolder);

            if (ECHO)
            {
                printf("\n\nFirst insertion tree:");
                printFolderTree(rootFolder);
                PAUSE();
            }
        }
        else
        {
            // If the previous folder name is different than the current one
            if (strcmp(fileBlocks[i-1].folderName,fileBlocks[i].folderName) != 0/* && contains(fileBlocks[i].folderName,"character/model/1_pc")*/)
            {
                if (ECHO)
                printf("\n----------------------------\nfileBlock.folderName: %s\n",fileBlocks[i].folderName);

                char* fullPath = (char*) malloc(strlen(fileBlocks[i].folderName) + 1);
                strcpy(fullPath,fileBlocks[i].folderName);

                // First tok
                char* folderName = strtok(fullPath,"/");
                char* previousFolderName = NULL;



                while (folderName != NULL)
                {
                    if (ECHO)
                    printf("\nstrtok: %s", folderName);

                    // Case "character/" from "character/something_else/" or "something_else/"
                    if (previousFolderName == NULL)// If it's the first tok
                    {
                        // If the folder wasn't added at all yet
                        if (getFolder(folderName,rootFolder) == NULL)
                        {
                            //printf("\n\nInsert root older %s", folderName);

                            // Insert in the  new root, because it's the first tok
                            insertFolder(folderName,&rootFolder);
                        }
                        else // If the folder already exists
                        {
                             if (ECHO)
                             {
                                printf("\nDo nothing.");
                                printf("\n\ntree:");
                                printFolderTree(rootFolder);
                             }
                            // Do nothing, let the program get the next subfolder in the next loop
                        }
                    }
                    else // If is not the first tok, then it's a subfolder
                    {


                        // If the subfolder wasn't added at all yet
                        if (getFolder(folderName,rootFolder) == NULL)
                        {
                             // printf("\nGet folder %s (previous to %s)\n",previousFolderName, folderName);
                            struct Folder* previousFolder = getFolder(previousFolderName,rootFolder);
                            //printf("\n\nInsert \"%s\" under \"%s\"", folderName, previousFolder->folderName);
                            insertSubFolder(folderName,&previousFolder);

                        }
                        else // If the subfolder was already added
                        {
                            if (ECHO)
                             {
                                printf("\nDo nothing.");
                                printf("\n\ntree:");
                                printFolderTree(rootFolder);
                             }
                            // Do nothing, let the program get the next subfolder in the next loop
                        }
                    }

                    if (previousFolderName != NULL)
                    {
                         free(previousFolderName);
                    }

                    // Saves this as previous folder name
                    previousFolderName = (char*) malloc(strlen(folderName) + 1);
                    strcpy(previousFolderName,folderName);

                    // Get next subfolder
                    folderName = strtok(NULL,"/");
                    if (ECHO)
                    {
                        printf("\n\npath: %s",fileBlocks[i].folderName);
                        PAUSE();
                    }
                }
            }
        }

    }
    return rootFolder;
}


void readBlock(FileBlock* fileBlock, FILE* fp)
{
    fileBlock->metaOffset = ftell(fp);

    fread(&fileBlock->hash,sizeof(long),1,fp);
    fread(&fileBlock->folderNum,sizeof(long),1,fp);
    fread(&fileBlock->fileNum,sizeof(long),1,fp);
    fread(&fileBlock->pazNum,sizeof(long),1,fp);
    fread(&fileBlock->fileOffset,sizeof(long),1,fp);
    fread(&fileBlock->zsize,sizeof(long),1,fp);
    fread(&fileBlock->size,sizeof(long),1,fp);

   // printf("%ld|%ld|%ld|PAZ:%ld\n",fileBlock->hash,fileBlock->folderNum,fileBlock->fileNum,fileBlock->pazNum);

}

void printBlock(FileBlock* fileBlock)
{
    printf("\nhash: %ld", fileBlock->hash);
    printf(" folder: %ld", fileBlock->folderNum);
    printf(" file: %ld", fileBlock->fileNum);
    printf(" paz_num: %ld", fileBlock->pazNum);
    printf(" offset: %ld", fileBlock->metaOffset);
    printf("| %s", fileBlock->folderName);
    printf("%s", fileBlock->fileName);
}

void printMetaFileInfo(MetaFileInfo* metaFileInfo)
{
    printf("\npazCount: %ld", metaFileInfo->pazCount);
    printf("\nfilesCount: %ld", metaFileInfo->fileBlocksCount);
    printf("\nfileBlocksCount: %ld", metaFileInfo->fileBlocksCount);
    printf("\nfileBlockStart: %ld", metaFileInfo->fileBlocksStart);
    printf("\nfileBlocksEnd: %ld", metaFileInfo->fileBlocksEnd);
}

long getPazCount(char* metaFileName)
{
    long pazCount = 0;
    FILE* metaFile = openFile(metaFileName,"rb");

    fseek(metaFile,sizeof(long),SEEK_SET);
    fread(&pazCount,sizeof(long),1,metaFile);

    fclose(metaFile);
    return pazCount;
}

long getFileCount(char* metaFileName)
{
    long pazCount = 0;
    FILE* metaFile = openFile(metaFileName,"rb");

    fseek(metaFile,sizeof(long),SEEK_SET);
    fread(&pazCount,sizeof(long),1,metaFile);

    fseek(metaFile,(pazCount * (3 * sizeof(long))),SEEK_CUR);
    long fileCount = 0;

    fread(&fileCount,sizeof(long),1,metaFile);

    fclose(metaFile);

    return fileCount;
}



