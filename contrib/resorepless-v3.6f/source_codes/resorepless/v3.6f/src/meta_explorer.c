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
    fread(&metaFileInfo->filesCount,sizeof(long),1,metaFile);
    printf("\nFILES_COUNT: %ld\n", metaFileInfo->filesCount);

    metaFileInfo->originalFileBlocksStart = ftell(metaFile);
    metaFileInfo->fileBlocksStart = metaFileInfo->originalFileBlocksStart;
    metaFileInfo->originalFileBlocksEnd = metaFileInfo->originalFileBlocksStart + (metaFileInfo->filesCount * ONE_REGISTRY);

    fclose(metaFile);

    return metaFileInfo;
}

FileBlock* fillFileBlocks(MetaFileInfo* metaFileInfo)
{
      /// Allocates Fileblocks
    FileBlock* fileBlocks = NULL;
    FileBlock* currentFileBlock = NULL;
    long multiplemodeldesc_hash = 631490897;
    long num_read = 0; // Buffer that reads all the ints until if finds the desired hash
    long hash_offset_start = 0;
    int hash_found = 0;
    int i = 0;

    fileBlocks = (FileBlock*)calloc(metaFileInfo->filesCount + 1, sizeof(FileBlock));
    metaFileInfo->fileBlocksCount = 0;

    FILE* metaFile = openFile(getLatestBackup(),"rb");
    //fseek(metaFile,metaFileInfo->originalFileBlocksStart,SEEK_SET); // Go to where the file blocks are supposed to start


    printf("\nFilling File Blocks...\n");
    fread(&num_read,sizeof(long),1,metaFile) ;



    // Searches for the hash, if found, from that point, it goes down block by block , storing the fileBlocks if they are valid, if they are not, it means that we reached the end
    // So we do the same thing, but going up this time
    while(fread(&num_read,sizeof(long),1,metaFile) && hash_found == 0)
    {
         // printf("%d ", num_read);

       if (num_read == multiplemodeldesc_hash) // If the desired hash was found
        {

           hash_found = 1;

            hash_offset_start = (ftell(metaFile) -sizeof(long));

            // Search down for valid File Blocks
            fseek(metaFile,hash_offset_start,SEEK_SET);
            do
            {
                currentFileBlock = &fileBlocks[metaFileInfo->fileBlocksCount];
                readBlock(currentFileBlock,metaFile);
                if (metaFileInfo->fileBlocksCount <= metaFileInfo->filesCount)
                {
                    metaFileInfo->fileBlocksCount++;
                }
                else
                {
                    break;
                }
            } while ((currentFileBlock->fileNum >= 0 && currentFileBlock->fileNum <= metaFileInfo->filesCount));

            metaFileInfo->fileBlocksCount--; // Subtracts the last registry because it doesn't contains a valid File Block
            metaFileInfo->fileBlocksEnd = ftell(metaFile) - ONE_REGISTRY; // Sets the end of the File Blocks here.


            // Search up for valid File Blocks
            fseek(metaFile,hash_offset_start,SEEK_SET);
            do
            {
                currentFileBlock = &fileBlocks[metaFileInfo->fileBlocksCount];
                fseek(metaFile,-ONE_REGISTRY,SEEK_CUR);
                readBlock(currentFileBlock,metaFile);
                //printBlock(currentFileBlock);
                if (metaFileInfo->fileBlocksCount <= metaFileInfo->filesCount)
                {
                    metaFileInfo->fileBlocksCount++;
                    fseek(metaFile,-ONE_REGISTRY,SEEK_CUR);
                }
                else
                {
                    break;
                }
            } while ((currentFileBlock->fileNum >= 0 && currentFileBlock->fileNum <= metaFileInfo->filesCount) && ftell(metaFile) >= metaFileInfo->originalFileBlocksStart);

            metaFileInfo->fileBlocksCount--; // Subtracts the last registry because it doesn't contains a valid File Block
            metaFileInfo->fileBlocksStart = ftell(metaFile) + ONE_REGISTRY; // Updates the file block start position to this one*/

        }
    }
     printf("FILE_BLOCKS_COUNT: %ld (%ld missing files)\n", metaFileInfo->fileBlocksCount,metaFileInfo->filesCount -  metaFileInfo->fileBlocksCount);

    printf("\nFILE_BLOCKS_START: %ld (%ld bytes skipped)\n", metaFileInfo->fileBlocksStart,metaFileInfo->fileBlocksStart - metaFileInfo->originalFileBlocksStart);
    printf("FILE_BLOCKS_END  : %ld (%ld bytes more than the expected)\n\n", metaFileInfo->fileBlocksEnd,metaFileInfo->fileBlocksEnd - metaFileInfo->originalFileBlocksEnd);

    if (metaFileInfo->fileBlocksCount == 0)
    {
        printf("Error: No valid File Blocks detected.\n");
        system("PAUSE");
    }

     // Seek to file block end
    fseek(metaFile,metaFileInfo->fileBlocksEnd,SEEK_SET);

    printf("Decrypting Folder Names...\n");
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
    }

    const uint8_t *decrypction_key = (const uint8_t *)"\x51\xF3\x0F\x11\x04\x24\x6A\x00"; // The Black Desert ICE decryption key

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
    char** folderNamesArray = (char**) malloc(metaFileInfo->filesCount * sizeof(char*));

    // Until the \0 is found, copy the letters to the folderNamesArray[i]

    int j = 0; // The current byte of the decrypted text
    int k = 0; // The current letter of the folderNamesArray[i]
    printf("Saving Folder Names...\n");

    i = 0; // Used to advance the fileNamesArray
    long folderNameLength = 0;
    for(j = (2 * sizeof(long)) /* Skips the first 2 numbers */; j < folders_part_length; j++)
    {
        // Prevents it from going out of the fileNamesArray allocated memory
        if (i + 1 > metaFileInfo->filesCount)
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
            folderNamesArray[i] = (char*) malloc((folderNameLength * sizeof(char)) + 1);
            j -= folderNameLength;
            for(k = 0; k < folderNameLength; k++)
            {
                folderNamesArray[i][k] = ptext[j];
                //printf("%c", folderNamesArray[i][k]);
                j++;
            }

            folderNamesArray[i][k] = '\0'; // Ends the string

            i++; // Advances the fileNameArray position
            folderNameLength = 0;
            j += (2 * sizeof(long)); /* Skips the first 2 numbers */
        }
    }
    printf("Folder Names saved: %d\n\n", i);

    // Assigns the right File Name from the fileNameArray to the right File Block, based on the File Num
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {

       // Allocates FolderName memory
        fileBlocks[i].folderName = (char*) malloc((strlen(folderNamesArray[fileBlocks[i].folderNum]) * sizeof(char)) + 1);

        // Goes to the folderNum position indicated in "fileBlocks[i].folderNum" and copies the folder name to the File Block
        strcpy(fileBlocks[i].folderName,folderNamesArray[fileBlocks[i].folderNum]);

        // Ends the string
        fileBlocks[i].folderName[strlen(folderNamesArray[fileBlocks[i].folderNum])] = '\0';
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
    char** fileNamesArray = (char**) malloc(metaFileInfo->filesCount * sizeof(char*));

    // Until the \0 is found, copy the letters to the fileName[i]

    j = 0; // The current byte of the decrypted text
    k = 0; // The current letter of the fileName[i]
    printf("Saving file names...\n");

    i = 0; // Used to advance the fileNamesArray
    long fileNameLength = 0;
    for(j = 0; j < filenames_part_length; j++)
    {

        // Prevents it from going out of the fileNamesArray allocated memory
        if (i + 1 > metaFileInfo->filesCount)
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

    // Assigns the right File Name from the fileNameArray to the right File Block, based on the File Num
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {

       // Allocates fileName memory
        fileBlocks[i].fileName = (char*) malloc((strlen(fileNamesArray[fileBlocks[i].fileNum]) * sizeof(char)) + 1);

        // Goes to the fileNum position indicated in "fileBlocks[i].fileNum" and copies the file name to the File Block
        strcpy(fileBlocks[i].fileName,fileNamesArray[fileBlocks[i].fileNum]);

        // Ends the string
        fileBlocks[i].fileName[strlen(fileNamesArray[fileBlocks[i].fileNum])] = '\0';
    }
    printf("File Names saved: %d (%ld less than expected)\n\n", i, metaFileInfo->fileBlocksCount - i);

    //printf("\nQuick sorting File Blocks...\n");
    // QUICK SORT
    qsort(fileBlocks,metaFileInfo->fileBlocksCount,sizeof(FileBlock),compare);
    //printf("Done.\n\n");


    fclose(metaFile);


    /*for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        printf("%ld|%ld|%ld|%ld|%s|%s\n",fileBlocks[i].hash,fileBlocks[i].folderNum,fileBlocks[i].fileNum,fileBlocks[i].pazNum,fileBlocks[i].folderName,fileBlocks[i].fileName);
    }*/



    free(ctext_backup_pointer);
    free(ptext_backup_pointer);


    for(i = 0; i < metaFileInfo->filesCount ; i++)
    {
        free(fileNamesArray[i]);
    }
    free(fileNamesArray);
    for(i = 0; i < metaFileInfo->filesCount ; i++)
    {
        free(folderNamesArray[i]);
    }
    free(folderNamesArray);



    return fileBlocks;
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
    printf("\nfilesCount: %ld", metaFileInfo->filesCount);
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

FileBlock** getFilteredFileBlocks(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, char* folderNameFilter, int* filteredFilesCount)
{
    int i = 0;
    (*filteredFilesCount) = 0;
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (strstr(fileBlocks[i].folderName,folderNameFilter))
        {
            (*filteredFilesCount)++;
        }
    }

    FileBlock** filteredFiles = (FileBlock**) malloc((*filteredFilesCount) * sizeof(FileBlock*));

    int j = 0;
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (strstr(fileBlocks[i].folderName,folderNameFilter))
        {
            filteredFiles[j] = &fileBlocks[i];
            j++;
        }
    }
    return filteredFiles;
}



