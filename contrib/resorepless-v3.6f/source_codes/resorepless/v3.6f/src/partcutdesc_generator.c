#include "../include/partcutdesc_generator.h"

int removedCount = 0;

void generatePartcutdesc(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, PatcherState* patcherState)
{
    removedCount = 0;
    int j = 0;
    printf("\n\nGenerating partcutdesc.xml file...");

    int filteredFileBlocksCount = 0;
    FileBlock** filteredFileBlocks = getFilteredFileBlocks(fileBlocks,metaFileInfo,"character/model/1_pc/",&filteredFileBlocksCount);

    int needRemoveCenshorshipFiles = markCensorshipRemoval(filteredFileBlocks, filteredFileBlocksCount, patcherState);

    int filesToPatchCount = 0;
    for (j = 0; j < metaFileInfo->fileBlocksCount; j++)
    {
        if (fileBlocks[j].needPatch != 0)
        {
            filesToPatchCount++;
            break;
        }
    }

    if (filesToPatchCount == 0 && !needToPatchPartcutdesc(patcherState))
    {
        printf("\nNo need.");
        return;
    }

    char* root = resoreplessFolder();
    char* characterFolderPath = concatenate(root,"character\\");

    createPath(characterFolderPath);
    char* pathToPartcutdesc = concatenate(characterFolderPath,"partcutdesc.xml");

    FILE* partcutdesc = openFile(pathToPartcutdesc, "wb");

    free(root);
    free(characterFolderPath);
    free(pathToPartcutdesc);

    char* UnderwearRelation = "<Relation CutType=\"Underwear\">\n</Relation>\n\n";
    fwrite(UnderwearRelation,strlen(UnderwearRelation),sizeof(char),partcutdesc);



    // If the partcut desc is the only file marked to patch, means we only generated this to remove the cut from the underwear and we don't need to do the following
    if (filesToPatchCount > 0 || needRemoveCenshorshipFiles)
    {
        int classAllowed[TOTAL_CLASSES];
        for (j = 0; j < TOTAL_CLASSES; j++)
        {
            classAllowed[j] = 1;
        }

        createCutTypeBlock("Underwear","38_Underwear",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);
        createCutTypeBlock("Event","Event_costume",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);

        classAllowed[PEW] = 0;
        classAllowed[PWM] = 0;
        classAllowed[PWW] = 0;
        classAllowed[PHW] = 0;
        classAllowed[PBW] = 0;
        createCutTypeBlock("Upperbody","9_Upperbody",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);

        for (j = 0; j < TOTAL_CLASSES; j++)
        {
            classAllowed[j] = 0;
        }
        classAllowed[PEW] = 1;
        classAllowed[PWM] = 1;
        classAllowed[PWW] = 1;
        createCutTypeBlock("PEW_Upperbody","9_Upperbody",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);

        for (j = 0; j < TOTAL_CLASSES; j++)
        {
            classAllowed[j] = 0;
        }
        classAllowed[PHW] = 1;
        createCutTypeBlock("PHW_Upperbody02","9_Upperbody",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);

        for (j = 0; j < TOTAL_CLASSES; j++)
        {
            classAllowed[j] = 0;
        }
        classAllowed[PBW] = 1;
        createCutTypeBlock("PBW_Upperbody05","9_Upperbody",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);


        for (j = 0; j < TOTAL_CLASSES; j++)
        {
            classAllowed[j] = 1;
        }
        classAllowed[PBW] = 0;
        classAllowed[PWM] = 0;
        classAllowed[PWW] = 0;
        createCutTypeBlock("Hand","11_Hand",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);

        for (j = 0; j < TOTAL_CLASSES; j++)
        {
            classAllowed[j] = 0;
        }
        classAllowed[PBW] = 1;
        classAllowed[PWM] = 1;
        classAllowed[PWW] = 1;
        createCutTypeBlock("PBW_Hand05","11_Hand",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);

        for (j = 0; j < TOTAL_CLASSES; j++)
        {
            classAllowed[j] = 1;
        }
        createCutTypeBlock("Lowerbody","10_Lowerbody",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);
        createCutTypeBlock("Foot","12_Foot",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);
        createCutTypeBlock("Hel","13_Hel",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);
        createCutTypeBlock("Hair","Hair",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);


        for (j = 0; j < TOTAL_CLASSES; j++)
        {
            classAllowed[j] = 0;
        }
        classAllowed[PHM] = 1;
        classAllowed[PNM] = 1;
        createCutTypeBlock("Sho","14_Sho",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);

        for (j = 0; j < TOTAL_CLASSES; j++)
        {
            classAllowed[j] = 1;
        }
        createCutTypeBlock("EnchantArm","28_EnchantArm",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);

        for (j = 0; j < TOTAL_CLASSES; j++)
        {
            classAllowed[j] = 0;
        }
        classAllowed[PHW] = 1;
        createCutTypeBlock("PHW_Sho02","14_Sho",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);

        for (j = 0; j < TOTAL_CLASSES; j++)
        {
            classAllowed[j] = 0;
        }
        classAllowed[PBW] = 1;
        classAllowed[PEW] = 1;
        classAllowed[PVW] = 1;
        classAllowed[PWW] = 1;
        classAllowed[PDW] = 1;
        classAllowed[PKWW] = 1;
        classAllowed[PNW] = 1;
        createCutTypeBlock("PBW_Sho","14_Sho",classAllowed,partcutdesc,filteredFileBlocksCount,filteredFileBlocks);
    }

    printf("\nDone.\n");
    float generatedFileSize = (float)ftell(partcutdesc);
    printf("\nGenerated file size: ");
    if (generatedFileSize > 1000)
    {
        printf("%.1f KBytes\n",(generatedFileSize/1000.0));
    }
    else
    {
        printf("%d bytes\n",(int)generatedFileSize);
    }


    printf("\nExtracting original partcutdesc file:\n");
    char* command = (char*) malloc (strlen("cd patcher_resources & quickbms.exe -Y -f \"*partcutdesc.xml*\" blackdesert.bms ../PAD00001.PAZ ./ 1>NULL1 2>NULL2") + 1);
    sprintf(command,"cd patcher_resources & quickbms.exe -Y -f \"*partcutdesc.xml*\" blackdesert.bms ../PAD00001.PAZ ./ 1>NULL1 2>NULL2");
    system(command);
    free(command);
    remove("patcher_resources\\NULL");
    remove("patcher_resources\\NULL1");
    remove("patcher_resources\\NULL2");
    if(!fileExists("patcher_resources\\character\\partcutdesc.xml"))
    {
        printf("\nFailed to extract original partcutdesc.xml");
        PAUSE();
    }
    else
    {
        printf("Success.");
    }

    char* pathToOriginalPartcutdesc = "patcher_resources\\character\\partcutdesc.xml";

    modifyPartcutdesc(pathToOriginalPartcutdesc,filteredFileBlocks,filteredFileBlocksCount);

    printf("\n\nMerging original and created partcutdesc.xml files...");

    FILE* originalPartCutDescFile = fopen(pathToOriginalPartcutdesc,"rb");

    char c = '\0';

    while(fread(&c,1,1,originalPartCutDescFile))
    {
        fwrite(&c,1,1,partcutdesc);
    }

    fclose(originalPartCutDescFile);
    fclose(partcutdesc);

    free(filteredFileBlocks);

    /*FileBlock* partcutdescFileBlock = searchFileBlock("partcutdesc.xml",fileBlocks,metaFileInfo->fileBlocksCount);

    if (!partcutdescFileBlock)
    {
        printf("\nWarning: Could not find \"partcutdesc.xml\" in your pad00000.meta file.");
        PAUSE();
    }
    else
    {
        partcutdescFileBlock->needPatch = 1;
        FILE* metaFile = openFile("pad00000.meta", "rb");
        if (metaFile)
        {
            if (!isPatched(partcutdescFileBlock,metaFileInfo,metaFile))
            {
                patchMetaFile(partcutdescFileBlock,1,REMOVE_A_FILE,metaFileInfo,1);
            }
            fclose(metaFile);
        }


    }*/

    printf("\nDone.\n");


}

void modifyPartcutdesc(char* pathToPartcutdesc, FileBlock** fileBlocks, int fileBlocksCount)
{
    printf("\n\nModifying %s...\n", pathToPartcutdesc);
    int filesRemoved = 0;
    FILE* originalPartCutDescFile = openFile(pathToPartcutdesc,"rb+");

    PartCutDescEntry* partCutDescEntries = NULL;

    char c = '\0';
    char prev_c = '\0';

    // Counts how many file names entries we have
    int entriesCount = 0;
    while(fread(&c,1,1,originalPartCutDescFile))
    {
        // Look for .pac</File>
        if (c == '<' && prev_c == 'c')
        {
            entriesCount++;
        }
        prev_c = c;
    }

    // Each position will hold a file name found between the <File>PATH_TO_FILE/fileName</File>
    partCutDescEntries = (PartCutDescEntry*) calloc(entriesCount*10,sizeof(PartCutDescEntry));

    int currentEntry = 0;
    prev_c = '\0'; // Resets

    // Goes back to the beginning of the file
    rewind(originalPartCutDescFile);

    // Now we are going to find the file names, and fill the partCutDescEntries array
    while(fread(&c,1,1,originalPartCutDescFile))
    {
        if (c == '/')
        {
            // Keeps updating the starting point so when it finds .pac</File> later, it will have where the last '/' was
            partCutDescEntries[currentEntry].lastSlash = ftell(originalPartCutDescFile);
        }
        else if (c == '>' && prev_c != '-')
        {
             partCutDescEntries[currentEntry].lastCloseTag = ftell(originalPartCutDescFile);
        }
        if (prev_c == 'U' && c == 'n') // If it found .pac</File>
        {
        }
        else if (c == '<' && prev_c == 'c') // If it found .pac</File>
        {
            // Marks the position before the < from </File>
            partCutDescEntries[currentEntry].fileNameEnd = ftell(originalPartCutDescFile) - 1;

           // Calculates the length of the file name
            long fileNameLength = (partCutDescEntries[currentEntry].fileNameEnd - partCutDescEntries[currentEntry].lastSlash);

            // Allocates the memory
            partCutDescEntries[currentEntry].fileName = (char*) malloc((fileNameLength + 1) * sizeof(char));

            // Gos back to where the name starts (after the last /)
            fseek(originalPartCutDescFile, partCutDescEntries[currentEntry].lastSlash,SEEK_SET);

            // Reads the file name from the file
            fread(partCutDescEntries[currentEntry].fileName,fileNameLength,sizeof(char),originalPartCutDescFile);

            // Ends the string
            partCutDescEntries[currentEntry].fileName[fileNameLength] = '\0';

            // Skips the < so we don't read it again
            fseek(originalPartCutDescFile, 1,SEEK_CUR);

            currentEntry++;

            if (currentEntry > entriesCount)
            {
                entriesCount = currentEntry;
                printf("\nWarning: currentEntry > entriesCount\n");
                break;
            }
        }

        prev_c = c;
    }


    // Now we are going to compare each file block with the entries we have, if they match, add a comment tag.
    int i = 0;
    int j = 0;

    for (i = 0; i < fileBlocksCount; i++)
    {
        // If I need to remove this file
        if (fileBlocks[i]->needPatch)
        {
            for (j = 0; j < entriesCount; j++)
            {
                if (strcmpi(fileBlocks[i]->fileName,partCutDescEntries[j].fileName) == 0)
                {
                    removedCount++;
                    printf("\n%.3d - ",removedCount);
                    printColor("Remove",YELLOW);
                    printf(" - %s", fileBlocks[i]->fileName);
                    fseek(originalPartCutDescFile,partCutDescEntries[j].lastCloseTag,SEEK_SET);

                    char* commentTagOpen = "<!--";
                    // Put it right after the <File> tag opens
                    fwrite(commentTagOpen,strlen(commentTagOpen),sizeof(char),originalPartCutDescFile);


                    char* commentTagClose = "-->";

                    // Put it right before the </File> tag closes
                    fseek(originalPartCutDescFile,partCutDescEntries[j].fileNameEnd - strlen(commentTagClose),SEEK_SET);
                    fwrite(commentTagClose,strlen(commentTagClose),sizeof(char),originalPartCutDescFile);

                    filesRemoved++;
                }
            }

        }
    }

    if (filesRemoved == 0)
    {
        printf("No Files Removed.");
    }

    fclose(originalPartCutDescFile);

    if (partCutDescEntries != NULL)
    {
       free(partCutDescEntries);
    }
}

int markCensorshipRemoval(FileBlock** fileBlocks, int  fileBlocksCount,PatcherState* patcherState)
 {
    int i = 0;
    int needToRemoveCenshorshipFiles = 0;
    for (i = 0; i < fileBlocksCount; i++)
    {
        if (strstr(fileBlocks[i]->folderName,"armor"))
        {
            if (strstr(fileBlocks[i]->folderName,"5_pbw") || strstr(fileBlocks[i]->folderName,"3_pew") || strstr(fileBlocks[i]->folderName,"15_pdw"))
            {
                if (patcherState->option[CENSORSHIP] >= MINIMAL_CENSORSHIP_REMOVAL)
                {
                     if (patcherState->option[CENSORSHIP] == MINIMAL_CENSORSHIP_REMOVAL)
                     {
                           if ((strstr(fileBlocks[i]->fileName,"pbw_00_lb_0018"))
                            || (strstr(fileBlocks[i]->fileName,"pbw_00_lb_0054")))
                           {
                               fileBlocks[i]->needPatch = 1;
                               needToRemoveCenshorshipFiles = 1;
                           }
                     }
                    if (patcherState->option[CENSORSHIP] >= MEDIUM_CENSORSHIP_REMOVAL)
                    {
                           if ((strstr(fileBlocks[i]->fileName,"pdw_03_ub_0001"))
                            || (strstr(fileBlocks[i]->fileName,"pdw_03_lb_0001"))
                            || (strstr(fileBlocks[i]->fileName,"pdw_02_ub_0006"))
                            || (strstr(fileBlocks[i]->fileName,"pdw_00_sho_0002"))
                            || (strstr(fileBlocks[i]->fileName,"pdw_02_lb_0002"))
                            || (strstr(fileBlocks[i]->fileName,"pdw_02_lb_0006"))
                            || (strstr(fileBlocks[i]->fileName,"pdw_02_lb_0004"))
                            || (strstr(fileBlocks[i]->fileName,"pdw_02_lb_0005"))
                            || (strstr(fileBlocks[i]->fileName,"pew_02_lb_0005"))
                            || (strstr(fileBlocks[i]->fileName,"pbw_00_lb_0054"))
                            || (strstr(fileBlocks[i]->fileName,"pbw_00_lb_0018")))
                           {
                               fileBlocks[i]->needPatch = 1;
                               needToRemoveCenshorshipFiles = 1;
                           }
                    }
                }
            }
        }
    }
    return needToRemoveCenshorshipFiles;
 }


void writestr(char* str,FILE* fp)
{
    fwrite(str,strlen(str),1,fp);
}

char* cutTypeOpenTag(char* label)
{
    char* tag = (char*) malloc(strlen("<CutType Name=\"") + strlen(label) + strlen("\">\n") + 1);
    strcpy(tag,"<CutType Name=\"");
    strcat(tag,label);
    strcat(tag,"\">\n");
    return tag;
}

char* basicCutTypeOpenTag(char* label)
{
    char* tag = (char*) malloc(strlen("<BasicCutType Name=\"") + strlen(label) + strlen("\">\n") + 1);
    strcpy(tag,"<BasicCutType Name=\"");
    strcat(tag,label);
    strcat(tag,"\">\n");
    return tag;
}

char* getFileBlockFullPath(FileBlock* fileBlock)
{
    char* substring = substr(fileBlock->folderName,strlen("character/model/"),strlen(fileBlock->folderName));
    char* result = concatenate(substring,fileBlock->fileName);
    free(substring);
    return result;

}

void addToPartCutDesc(FileBlock* fileBlock, FILE* partcutdesc)
{
    char* fileOpenTag1 = "\t<File>";
    char* fileCloseTag1 = "</File>\n";
    char* fileOpenTag2 = "\t<!-- <File>";
    char* fileCloseTag2 = "</File> -->\n";

         if (fileBlock->needPatch == 1)
         {
            writestr(fileOpenTag2,partcutdesc);
         }
         else
         {
             writestr(fileOpenTag1,partcutdesc);
         }

         char* pathToFileBlock = getFileBlockFullPath(fileBlock);
         writestr(pathToFileBlock,partcutdesc);

         free(pathToFileBlock);

         if (fileBlock->needPatch == 1)
         {
            writestr(fileCloseTag2,partcutdesc);
            removedCount++;
            printf("\n%.3d - ",removedCount);printColor("Removed",YELLOW);printf(" - %s", fileBlock->fileName);
         }
         else
         {
             writestr(fileCloseTag1,partcutdesc);
         }

}
void createCutTypeBlock(char* blockName,char* compareFolderName,int* classAllowed, FILE* partcutdesc, int  fileBlocksCount,FileBlock** fileBlocks)
{
      int i = 0;


    writestr(cutTypeOpenTag(blockName), partcutdesc);
    for (i = 0; i < fileBlocksCount; i++)
    {


         if (strcmpi(getLastFolder(fileBlocks[i]->folderName),compareFolderName) == 0)
         {
            if (strstr(fileBlocks[i]->fileName,"phm"))
            {
                if (classAllowed[PHM])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"phw"))
            {
                if (classAllowed[PHW])
                {

                    addToPartCutDesc(fileBlocks[i], partcutdesc);

                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pew"))
            {
                if (classAllowed[PEW])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pgm"))
            {
                if (classAllowed[PGM])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pbw"))
            {
                if (classAllowed[PBW])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pkm") )
            {
                if (classAllowed[PKM])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pvw"))
            {
                if (classAllowed[PVW])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pwm"))
            {
                if (classAllowed[PWM])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pww"))
            {
                if (classAllowed[PWW])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pnw"))
            {
                if (classAllowed[PNW])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pnm"))
            {
                if (classAllowed[PNM])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pdw"))
            {
                if (classAllowed[PDW])
                {

                   addToPartCutDesc(fileBlocks[i], partcutdesc);

                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pkw"))
            {
                if (classAllowed[PKWW])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pcm"))
            {
                if (classAllowed[PCM])
                {
                    addToPartCutDesc(fileBlocks[i], partcutdesc);
                }
            }
            else if (strstr(fileBlocks[i]->fileName,"pcw"))
            {
                if (classAllowed[PCW])
                {

                   addToPartCutDesc(fileBlocks[i], partcutdesc);


                }
            }
            else {
                addToPartCutDesc(fileBlocks[i], partcutdesc);
            }
         }
    }
    writestr("</CutType>\n\n",partcutdesc);
}
