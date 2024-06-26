#include "../include/paz_browser.h"
int askExtractOtherParts()
{
    char extractOtherParts = '\0';
    printf("\nDo you want to extract the other parts as well? (lb,sho,cloak,hel,hand,floot,etc..)");
    printf("\n(y/n): ");
    extractOtherParts = getch();

    if (extractOtherParts == 'Y' || extractOtherParts == 'y')
    {
        return 1;
    }
    return 0;
}

int fileExplorer(int* cursor, FileBlock** filesOnMenu, int nOptions, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo,long* previousBackupHashes, long previousBackupHashesCount , char* pathSoFar, char* previousExtractLocation, int* sortingType,int* sortingOrder, int operation, int* displayRealNames)
{
    int pageStart = 0;
    int pageEnd = MAX_LINES* MAX_ROWS;
    int longestLength = 0;
    int selectedCount = 0;
    char input = '\0';
    int i = 0;

    if (previousBackupHashes != NULL && previousBackupHashesCount > 0)
    {
        int j = 0;
        //printf("Checking for new files...");
        for (j = 0; j < nOptions; j++)
        {
            if (!filesOnMenu[j]->isAFolder)
            {
                if (!binarySearchByHash(filesOnMenu[j]->hash,previousBackupHashes,previousBackupHashesCount))
                {
                     filesOnMenu[j]->isNew = 1;
                }
            }
        }
    }

    int biggerHeight = notRecommendedSettings(filesOnMenu,nOptions,(*sortingType),(*sortingOrder));
    recalculateSettings(filesOnMenu,nOptions,pageStart,biggerHeight,displayRealNames);
    applyFileSorting(filesOnMenu,nOptions,(*sortingType),(*sortingOrder),pageStart,pageEnd, metaFileInfo,displayRealNames);

    while(1)
    {
        if (input >= 0)
        {
            displayFiles_fileExplorer(filesOnMenu, nOptions, (*cursor), pageStart, pageEnd, selectedCount, pathSoFar, (*sortingType), (*sortingOrder),operation,displayRealNames);
        }

        input = getch();

        if (input >= 0)
        {
            if (nOptions == 0)
            {
                if (input == ESC)
                {
                    return BACK;
                }
            }
            else
            {
               if (input == ENTER)
                {
                    if (filesOnMenu[(*cursor)]->isAFolder)
                    {
                        return NEXT_FOLDER;
                    }
                    else
                    {
                        if (selectedCount == 0)
                        {
                            char* extractLocation = askExtractLocation(&previousExtractLocation);
                            int extractOtherParts = 0;


                            if (extractLocation != NULL)
                            {
                                if (strstr(pathSoFar,"9_upperbody"))
                                {
                                    extractOtherParts = askExtractOtherParts();
                                }

                                if (fileHasExt(filesOnMenu[(*cursor)]->fileName,"pac"))
                                {
                                    if (filesOnMenu[(*cursor)]->hasRealName && (*displayRealNames))
                                    {
                                        extractLocation = concatenate3(extractLocation,getClassAsString(filesOnMenu[(*cursor)]),"-");

                                        char* RealName = NULL;
                                        RealName = concatenate(RealName,filesOnMenu[(*cursor)]->RealName);
                                        charReplace(RealName,'(','[');
                                        charReplace(RealName,')',']');
                                        charReplace(RealName,'/','-');
                                        charReplace(RealName,' ','_');

                                        extractLocation = concatenate3(extractLocation,RealName,"\\");
                                    }
                                    else
                                    {
                                        extractLocation = concatenate3(extractLocation,removeFileExt(filesOnMenu[(*cursor)]->fileName),"\\");
                                    }
                                 }

                                 if (extractOtherParts)
                                 {
                                    char* selectedFileName = filesOnMenu[(*cursor)]->fileName;
                                    char* fileNameAnyType = (char*) malloc(strlen(selectedFileName)+ 1);
                                    sprintf(fileNameAnyType,"%s_%s_*_%s", getClassPrefix(selectedFileName),getTwoDigits(selectedFileName),getFourDigits(selectedFileName));

                                    for (i = metaFileInfo->pacFileBlocksStart; i <= metaFileInfo->pacFileBlocksEnd; i++)
                                    {
                                        if (contains(fileBlocks[i].fileName,fileNameAnyType) && strstr(fileBlocks[i].folderName,"armor") && !strstr(fileBlocks[i].folderName,"38_underwear"))
                                        {
                                            char* thisFileExtractLocation = removeFileExt(fileBlocks[i].fileName);
                                            thisFileExtractLocation = concatenate3(extractLocation,thisFileExtractLocation,"\\");

                                            extractFileCustomPath(&fileBlocks[i],thisFileExtractLocation,1);
                                            extractTexturesFrom(&fileBlocks[i],fileBlocks,metaFileInfo,thisFileExtractLocation,0);
                                            free(thisFileExtractLocation);
                                        }
                                    }
                                    openFolder(extractLocation);
                                 }
                                 else
                                 {
                                    if (extractFileCustomPath(filesOnMenu[(*cursor)],extractLocation,1))
                                    {
                                        if (fileHasExt(filesOnMenu[(*cursor)]->fileName,"pac"))
                                        {
                                            extractTexturesFrom(filesOnMenu[(*cursor)],fileBlocks,metaFileInfo,extractLocation,1);
                                        }
                                        Sleep(500);

                                        // Open Extracted File
                                        char* filePath = concatenate(extractLocation,filesOnMenu[(*cursor)]->fileName);
                                        if (fileHasExt(filesOnMenu[(*cursor)]->fileName,"pac"))
                                        {
                                            filePath = concatenate("patcher_resources\\3d_converter\\3dconverter.exe ",filePath);
                                        }
                                        system(filePath);
                                        free(filePath);
                                    }
                                }
                            }
                        }
                        else if (selectedCount > 0)
                        {
                            char* extractLocation = askExtractLocation(&previousExtractLocation);
                            char* originalExtractLocation = extractLocation;
                            int extractOtherParts = 0;

                            if (extractLocation != NULL)
                            {
                                 if (strstr(pathSoFar,"9_upperbody"))
                                {
                                    extractOtherParts = askExtractOtherParts();
                                }

                                int atLeastOneSuccess = 0;
                                int success = 0;
                                for (i = 0; i < nOptions; i++)
                                {
                                    if (filesOnMenu[i]->isSelected)
                                    {
                                        printColor("\n\n\n----------------------- Extracting files related to : ",YELLOW);printf("%s",filesOnMenu[i]->fileName);printColor(" -----------------------",YELLOW);
                                        extractLocation = originalExtractLocation;
                                        if (filesOnMenu[i]->hasRealName && (*displayRealNames))
                                        {
                                            extractLocation = concatenate3(extractLocation,getClassAsString(filesOnMenu[i]),"-");

                                            char* RealName = NULL;
                                            RealName = concatenate(RealName,filesOnMenu[i]->RealName);
                                            charReplace(RealName,'(','[');
                                            charReplace(RealName,')',']');
                                            charReplace(RealName,'/','-');
                                            charReplace(RealName,' ','_');

                                            extractLocation = concatenate3(extractLocation,RealName,"\\");

                                        }
                                        else
                                        {
                                            extractLocation = concatenate3(extractLocation,removeFileExt(filesOnMenu[i]->fileName),"\\");
                                        }
                                        if (extractOtherParts)
                                        {
                                            char* selectedFileName = filesOnMenu[i]->fileName;
                                            char* fileNameAnyType = (char*) malloc(strlen(selectedFileName)+ 1);
                                            sprintf(fileNameAnyType,"%s_%s_*_%s", getClassPrefix(selectedFileName),getTwoDigits(selectedFileName),getFourDigits(selectedFileName));

                                            int j = 0;
                                            for (j = metaFileInfo->pacFileBlocksStart; j <= metaFileInfo->pacFileBlocksEnd; j++)
                                            {
                                                if (contains(fileBlocks[j].fileName,fileNameAnyType) && strstr(fileBlocks[j].folderName,"armor") && !strstr(fileBlocks[j].folderName,"38_underwear"))
                                                {
                                                    char* thisFileExtractLocation = removeFileExt(fileBlocks[j].fileName);
                                                    thisFileExtractLocation = concatenate3(extractLocation,thisFileExtractLocation,"\\");

                                                    success = extractFileCustomPath(&fileBlocks[j],thisFileExtractLocation,1);
                                                    extractTexturesFrom(&fileBlocks[j],fileBlocks,metaFileInfo,thisFileExtractLocation,0);
                                                    charReplace(extractLocation,'/','-');
                                                    free(thisFileExtractLocation);
                                                }
                                            }
                                            free(fileNameAnyType);
                                        }
                                        else if (fileHasExt(filesOnMenu[i]->fileName,"pac"))
                                        {

                                            success = extractFileCustomPath(filesOnMenu[i],extractLocation,1);
                                            extractTexturesFrom(filesOnMenu[i],fileBlocks,metaFileInfo,extractLocation,0);
                                            charReplace(extractLocation,'/','-');
                                        }
                                        else
                                        {
                                            success = extractFileCustomPath(filesOnMenu[i],originalExtractLocation,1);
                                        }
                                        atLeastOneSuccess += success;
                                        if (!success)
                                        {
                                            printf("\nNo files extracted.");
                                            PAUSE();
                                        }
                                    }
                                }

                                if (atLeastOneSuccess)
                                {
                                    openFolder(originalExtractLocation);
                                    PAUSE();
                                }
                            }
                        }
                    }
                }
                else if (input == 'W' || input == 'w')
                {
                    if ((*cursor) - 1 >= 0)
                    {
                        unselectFile(filesOnMenu[(*cursor)],&selectedCount);
                        (*cursor)--;
                    }
                    if ((*cursor) < pageStart)
                    {
                        unselectFile(filesOnMenu[(*cursor)],&selectedCount);
                        previousPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo, biggerHeight,displayRealNames);
                    }
                }
                else if (input == 'S' || input == 's')
                {
                    if( (*cursor) + 1 < nOptions)
                    {
                        selectFile(filesOnMenu[(*cursor)],&selectedCount);
                        (*cursor)++;
                        selectFile(filesOnMenu[(*cursor)],&selectedCount);
                    }
                    if ((*cursor) >= pageEnd)
                    {
                        selectFile(filesOnMenu[(*cursor)],&selectedCount);
                        nextPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo, biggerHeight,displayRealNames);
                        selectFile(filesOnMenu[(*cursor)],&selectedCount);
                    }
                }
                else if (input == 'F' || input == 'f')
                {
                   menuSortingType(sortingType,sortingOrder);
                   applyFileSorting(filesOnMenu,nOptions,(*sortingType),(*sortingOrder),pageStart,pageEnd, metaFileInfo,displayRealNames);

                }
                else if (input == SPACE)
                {
                    if (!filesOnMenu[(*cursor)]->isSelected)
                    {
                        selectFile(filesOnMenu[(*cursor)],&selectedCount);
                    }
                    else
                    {
                        unselectFile(filesOnMenu[(*cursor)],&selectedCount);
                    }
                }
                else if (input == 'A' || input == 'a' )
                {
                    if (selectedCount > 0)
                    {
                        for (i = 0; i < nOptions; i++)
                        {
                            filesOnMenu[i]->isSelected = 0;
                        }
                        selectedCount = 0;
                    }
                    else if (selectedCount == 0)
                    {
                        for (i = 0; i < nOptions; i++)
                        {
                            if (!filesOnMenu[i]->isAFolder)
                            {
                                filesOnMenu[i]->isSelected = 1;
                                selectedCount++;
                            }
                        }
                    }
                }
                else if (input == 'N' || input == 'n')
                {
                   (*displayRealNames) = !(*displayRealNames);

                   recalculateSettings(filesOnMenu,nOptions,pageStart,biggerHeight,displayRealNames);
                }
                else if (input == ESC)
                {
                    return BACK;
                }

                else if (input == PAGE_UP)
                {
                    nextPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo, biggerHeight,displayRealNames);
                    (*cursor) = pageStart;
                }
                else if (input == PAGE_DOWN)
                {
                    previousPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo, biggerHeight,displayRealNames);
                    (*cursor) = pageStart;
                }
                else if (input == ARROW_DOWN)
                {
                    if( (*cursor) + 1 < nOptions)
                    {
                        (*cursor)++;
                    }
                    // Allows loop cursor when only one ROW
                    else if (nOptions <= MAX_LINES)
                    {
                        (*cursor) = pageStart;
                    }
                    if ((*cursor) >= pageEnd)
                    {
                        nextPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo, biggerHeight,displayRealNames);
                    }
                }
                else if (input == ARROW_UP)
                {
                    if ((*cursor) - 1 >= 0)
                    {
                        (*cursor)--;
                    }
                    // Allows loop cursor when only one ROW
                    else if (nOptions <= MAX_LINES)
                    {
                        (*cursor) = nOptions - 1;
                    }
                    if ((*cursor) < pageStart)
                    {
                        previousPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo, biggerHeight,displayRealNames);
                    }
                }
                else if (input == ARROW_RIGHT)
                {
                    if ((*cursor) + MAX_LINES < nOptions)
                    {
                        (*cursor) += MAX_LINES;
                    }
                    if ((*cursor) >= pageEnd)
                    {
                        nextPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo, biggerHeight,displayRealNames);
                    }
                }
                else if (input == ARROW_LEFT)
                {
                    if ((*cursor) - MAX_LINES >= 0)
                    {
                        (*cursor) -= MAX_LINES;
                    }
                    if ((*cursor) < pageStart)
                    {
                        previousPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo, biggerHeight,displayRealNames);
                    }
                }
            }
        }
    }
    resizeWindow(120,30);
}

int folderMenu(char* startingPath, char** folderNamesList,int folderNamesCount, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, long* previousBackupHashes, long previousBackupHashesCount)
{
    char* previousExtractLocation = "";
    int sortingType = SORT_BY_FILE_NAME;
    int sortingOrder = ASCENDANT;
    int displayRealNames = 1;

    return recalculateFilesAndFoldersToDisplay(folderNamesList, folderNamesCount, startingPath, fileBlocks, metaFileInfo, previousBackupHashes, previousBackupHashesCount, previousExtractLocation, &sortingType, &sortingOrder, &displayRealNames);
}

int recalculateFilesAndFoldersToDisplay(char** folderNamesList,int folderNamesCount, char* pathSoFar, FileBlock* fileBlocks,  MetaFileInfo* metaFileInfo, long* previousBackupHashes, long previousBackupHashesCount, char* previousExtractLocation, int* sortingType,int* sortingOrder, int* displayRealNames)
{
    int i = 0;
    int cursor = 0;
    int shouldReadArmorNamesFile = 0;


    int foldersToDisplayCount = 0;
    FileBlock** foldersToDisplay = getFoldersToDisplay(folderNamesList,folderNamesCount, &foldersToDisplayCount, pathSoFar);

    int filesToDisplayCount = 0;
    FileBlock** filesToDisplay = getFilesToDisplay(&filesToDisplayCount, pathSoFar,fileBlocks,metaFileInfo, (*sortingType),(*sortingOrder));

    if (folderNamesCount == 0 && filesToDisplayCount == 0)
    {
        printf("\nNo files or folders to display");
        PAUSE();
        return 0;
    }

    //printf("\nMerging them together...");
    FileBlock** filesOnMenu = (FileBlock**) calloc((folderNamesCount + filesToDisplayCount) + 1, sizeof(FileBlock*));

    int nOptions = 0;
    for (i = 0; i < foldersToDisplayCount; i++)
    {
        char* formatedFolderName = (char*) malloc(strlen(foldersToDisplay[i]->folderName) + strlen("[/]") + 1);
        if (isWindows10)
        {
            sprintf(formatedFolderName,"%s/", foldersToDisplay[i]->folderName);
        }
        else
        {
            sprintf(formatedFolderName,"[%s/]", foldersToDisplay[i]->folderName);
        }

        filesOnMenu[nOptions] = foldersToDisplay[i];
        filesOnMenu[nOptions]->fileName = formatedFolderName;
        filesOnMenu[nOptions]->isSelected = 0;
        filesOnMenu[nOptions]->isAFolder = 1;
        nOptions++;

    }
    for (i = 0; i < filesToDisplayCount; i++)
    {
        if (filesToDisplay[i]->fileName[0] == 'p' && (fileHasExt(filesToDisplay[i]->fileName,"pac") || fileHasExt(filesToDisplay[i]->fileName,"dds")))
        {
            shouldReadArmorNamesFile = 1;
        }
        filesOnMenu[nOptions] = filesToDisplay[i];
        filesOnMenu[nOptions]->isAFolder = 0;
        nOptions++;
    }
   // printf("\nDone.\n");

    if (filesToDisplayCount > 0 && shouldReadArmorNamesFile)
    {
        readRealNamesFile(filesOnMenu, nOptions);
    }
    int fileExplorerReturn = -1;

    while(fileExplorerReturn != EXIT)
    {
        fileExplorerReturn = fileExplorer(&cursor, filesOnMenu, nOptions, fileBlocks, metaFileInfo, previousBackupHashes, previousBackupHashesCount, pathSoFar, previousExtractLocation, sortingType, sortingOrder, BROWSE,displayRealNames);

        if (fileExplorerReturn == NEXT_FOLDER)
        {
            recalculateFilesAndFoldersToDisplay(folderNamesList, folderNamesCount, concatenate3(pathSoFar,foldersToDisplay[cursor]->folderName,"/"), fileBlocks, metaFileInfo, previousBackupHashes, previousBackupHashesCount, previousExtractLocation, sortingType, sortingOrder, displayRealNames);
        }
    }
    return fileExplorerReturn;
}

FileBlock** getFoldersToDisplay(char** folderNamesList, int folderNamesCount, int* foldersToDisplayCount, char* pathSoFar)
{
    //printf("\nGetting Folders to Display...");
    List* foldersToDisplayList = NULL;
    (*foldersToDisplayCount) = 0;

    char* previousFolderName = "\0";
    int i = 0;

    for (i = 0; i < folderNamesCount; i++)
    {
        char* folderPath = (char*) malloc(strlen(folderNamesList[i]) + 1);
        strcpy(folderPath,folderNamesList[i]);

        if (((strlen(pathSoFar) > 1) && contains(folderNamesList[i],pathSoFar)))
        {
            if (beginsWith(folderPath,pathSoFar) && strcmpi(folderPath,pathSoFar) != 0)
            {
                folderPath = substr(folderPath,strlen(pathSoFar),strlen(folderPath));
                char* folderName = strtok(folderPath,"/");

                if (strcmp(folderName,previousFolderName) != 0)
                {
                    FileBlock* fileBlock = (FileBlock*) calloc(1 , sizeof(FileBlock));
                    fileBlock->folderName = folderName;
                    fileBlock->isAFolder = 1;

                    /*fileBlock->fileName = (char*) malloc(strlen("FOLDER NAME ONLY") + 1);
                    strcpy(fileBlock->fileName,"FOLDER NAME ONLY");*/

                    insertList(fileBlock,&foldersToDisplayList);
                    (*foldersToDisplayCount)++;

                    previousFolderName = folderName;
                }
            }
        }

        if (strlen(pathSoFar) <= 1)
        {
            char* folderName = strtok(folderPath,"/");
            if (strcmp(folderName,previousFolderName) != 0)
            {
                FileBlock* fileBlock = (FileBlock*) calloc(1 , sizeof(FileBlock));
                fileBlock->folderName = folderName;
                fileBlock->isAFolder = 1;

                /*fileBlock->fileName = (char*) malloc(strlen("FOLDER NAME ONLY") + 1);
                strcpy(fileBlock->fileName,"FOLDER NAME ONLY");*/

                insertList(fileBlock,&foldersToDisplayList);
                (*foldersToDisplayCount)++;

                previousFolderName = folderName;
            }
         }
    }
    FileBlock** foldersToDisplay = getListAsArray(foldersToDisplayList,(*foldersToDisplayCount));

    qsort(foldersToDisplay,(*foldersToDisplayCount),sizeof(FileBlock**),sort_by_folder_name_asc);
    //printf("\nDone.\n");

    return foldersToDisplay;
}

FileBlock** getFilesToDisplay(int* filesToDisplayCount, char* pathSoFar, FileBlock* fileBlocks, MetaFileInfo* metafileInfo, int sortingType, int sortingOrder)
{
    //printf("\nGetting Files to Display...");
    FileBlock** filesToDisplay = NULL;
    int i = 0;
    int run = 0;
    int currentFile = 0;
    for (run = 0; run < 2; run++)
    {
        if (run == 1)
        {
            filesToDisplay = (FileBlock**) malloc(((*filesToDisplayCount) + 1) * sizeof(FileBlock));
        }
        for (i = 0; i < metafileInfo->fileBlocksCount; i++)
        {
            if (strcmpi(fileBlocks[i].folderName,pathSoFar) == 0)
            {
                if (run == 0)
                {
                    (*filesToDisplayCount)++;
                }
                else
                {
                    filesToDisplay[currentFile] = &fileBlocks[i];
                    currentFile++;
                }
            }
        }
    }

    sortFileNames(filesToDisplay,(*filesToDisplayCount),sortingType,sortingOrder);

    //printf("\nDone.\n");
    return filesToDisplay;
}



int recalculateSettings(FileBlock** filesOnMenu, int nOptions, int pageStart, int biggerHeight, int* diplayRealNames)
{
        int necessaryWindowWidth = getRowsTotalLength(filesOnMenu,nOptions,pageStart,(*diplayRealNames));

        if (!isWindows10)
        {
            necessaryWindowWidth += ((MAX_ROWS-1) * strlen("*")) + strlen("[]");
        }

        if (necessaryWindowWidth > (120 - 4))
        {
            if (biggerHeight)
            {
                resizeWindow(necessaryWindowWidth + 4,36);
            }
            else
            {
                resizeWindow(necessaryWindowWidth + 4,33);
            }

        }
        else
        {
            if (biggerHeight)
            {
                resizeWindow(120,36);
            }
            else
            {
                resizeWindow(120,33);
            }
        }
        return necessaryWindowWidth;
}

int getRowsTotalLength(FileBlock** filesOnMenu,int nOptions,int pageStart, int displayRealFileNames)
{
    int totalLength = 0;
    int i = 0;
    int row = 0;
    for (row = 0; row < MAX_ROWS; row++)
    {
        int rowStartIDX = row * MAX_LINES + pageStart;
        int rowEndIDX = ((row+1) * MAX_LINES)  - 1 + pageStart;
        if (rowEndIDX > nOptions - 1)
        {
            rowEndIDX = nOptions - 1;
        }
        int longestLength = 0;

        for (i = rowStartIDX; i <= rowEndIDX; i++)
        {
            if (filesOnMenu[i]->hasRealName && displayRealFileNames)
            {
                if (longestLength < strlen(filesOnMenu[i]->RealName))
                {
                    longestLength = strlen(filesOnMenu[i]->RealName);
                }
            }
            else
            {
                if (longestLength < strlen(filesOnMenu[i]->fileName))
                {
                    longestLength = strlen(filesOnMenu[i]->fileName);
                }
            }
        }
        totalLength += (1 * strlen(" -->")) + longestLength;
       // printf("\ntotalLength += %d  = %d", longestLength,totalLength);
    }
    if (totalLength < 97)
    {
        totalLength = 97;
    }
   // printf("\ntotalLength: %d", totalLength);
   // PAUSE();
    return totalLength;
}

void compensateLength(FileBlock** filesOnMenu,int nOptions, int currentFileIDX, int displayRealFileNames)
{
    int row = (int)((float)currentFileIDX/(float)MAX_LINES);


    int rowStartIDX = row * MAX_LINES;
    int rowEndIDX = ((row+1) * MAX_LINES)  - 1;
    if (rowEndIDX > nOptions - 1)
    {
        rowEndIDX = nOptions - 1;
    }

    char* nameToDisplay = filesOnMenu[currentFileIDX]->fileName;

    if (filesOnMenu[currentFileIDX]->hasRealName && displayRealFileNames)
    {
        nameToDisplay = filesOnMenu[currentFileIDX]->RealName;
    }

    int longestLength = 0;


    int i = 0;
    for (i = rowStartIDX; i <= rowEndIDX; i++)
    {
        if (displayRealFileNames && filesOnMenu[i]->hasRealName)
        {
            if (strlen(filesOnMenu[i]->RealName) > longestLength)
            {
                longestLength = strlen(filesOnMenu[i]->RealName);
            }
        }
        else
        {
            if (strlen(filesOnMenu[i]->fileName) > longestLength)
            {
                longestLength = strlen(filesOnMenu[i]->fileName);
            }
        }
    }

    /*if (strlen(nameToDisplay) == longestLength)
    {
        char *strToPrint = (char*)malloc(32);
        sprintf(strToPrint," %d - %d", strlen(nameToDisplay),longestLength);
        printColor(strToPrint,YELLOW);
    }
    else
    {
        printf(" %d - %d", strlen(nameToDisplay),longestLength);
    }*/

    for(i = 0; i < (longestLength - strlen(nameToDisplay)); i++)
    {
        printf(" ");
    }
}


void applyFileSorting(FileBlock** filesOnMenu, int nOptions, int sortingType,int sortingOrder, int pageStart, int pageEnd, MetaFileInfo* metaFileInfo, int* displayRealNames)
{
   FileBlock** filesToDisplay = (FileBlock**) malloc(nOptions * sizeof(FileBlock*));
   int filesToDisplayCount = 0;
   int i = 0;
   for (i = 0; i < nOptions; i++)
   {
       if (!filesOnMenu[i]->isAFolder)
       {
            filesToDisplay[filesToDisplayCount] = filesOnMenu[i];
            filesToDisplayCount++;
       }
   }

   sortFileNames(filesToDisplay,filesToDisplayCount,sortingType,sortingOrder);

   int j = 0;
   for (i = 0; i < nOptions; i++)
   {
       if (!filesOnMenu[i]->isAFolder)
       {
            filesOnMenu[i] = filesToDisplay[j];
            j++;
       }
   }
   int biggerHeight = notRecommendedSettings(filesOnMenu,nOptions,sortingType,sortingOrder);
   recalculateSettings(filesOnMenu,nOptions,pageStart,biggerHeight,displayRealNames);
   free(filesToDisplay);
}

void sortFileNames(FileBlock** filesToDisplay, int filesToDisplayCount, int sortingType, int sortingOrder)
{
    if (sortingType == SORT_BY_FILE_NAME)
    {
        if (sortingOrder == ASCENDANT)
        {
            qsort(filesToDisplay,filesToDisplayCount,sizeof(FileBlock**),sort_by_file_name_asc);
        }
        else
        {
            qsort(filesToDisplay,filesToDisplayCount,sizeof(FileBlock**),sort_by_file_name_desc);
        }
    }
    else if (sortingType == SORT_BY_REAL_NAME)
    {
        if (sortingOrder == ASCENDANT)
        {
            qsort(filesToDisplay,filesToDisplayCount,sizeof(FileBlock**),sort_by_real_name_asc);
        }
        else
        {
            qsort(filesToDisplay,filesToDisplayCount,sizeof(FileBlock**),sort_by_real_name_desc);
        }
    }
    else if (sortingType == SORT_BY_FILE_NUM)
    {
        if (sortingOrder == ASCENDANT)
        {
            qsort(filesToDisplay,filesToDisplayCount,sizeof(FileBlock**),sort_by_file_num_asc);
        }
        else
        {
            qsort(filesToDisplay,filesToDisplayCount,sizeof(FileBlock**),sort_by_file_num_desc);
        }
    }
    else if (sortingType == SORT_BY_EXTENSION)
    {
        if (sortingOrder == ASCENDANT)
        {
            qsort(filesToDisplay,filesToDisplayCount,sizeof(FileBlock**),sort_by_extension_asc);
        }
        else
        {
            qsort(filesToDisplay,filesToDisplayCount,sizeof(FileBlock**),sort_by_extension_desc);
        }
    }
}

void selectFile(FileBlock* fileToSelect, int* selectedCount)
{
    if(!fileToSelect->isAFolder)
    {
        if (!fileToSelect->isSelected)
        {
            (*selectedCount)++;
        }
        fileToSelect->isSelected = 1;
    }
}

void unselectFile(FileBlock* fileToUnselect, int* selectedCount)
{
    if(!fileToUnselect->isAFolder)
    {
        if (fileToUnselect->isSelected && (*selectedCount) - 1 >= 0)
        {
            (*selectedCount)--;
        }
        fileToUnselect->isSelected = 0;
    }
}





void menuSortingType(int* sortingType, int* sortingOrder)
{
    int cursor = (*sortingType);
    char input = '\0';

    int changedOption = 0; // If the user changed the sorting order at least once

    while(1)
    {
        system("cls");
        printMainHeader();
        printf("\nHow do you want the files to be sorted?\n\n");

        if (cursor == SORT_BY_FILE_NAME) {printf(" --> ");}else {printf("     ");}
        printf("File Name\t");
        if (cursor == SORT_BY_FILE_NAME) {if((*sortingOrder) == ASCENDANT){printColor("Ascendant",GREEN);}else{printColor("Descendant",RED);}}
        printf("\n");

        if (cursor == SORT_BY_REAL_NAME) {printf(" --> ");}else {printf("     ");}
        printf("Real Name\t");
        if (cursor == SORT_BY_REAL_NAME) {if((*sortingOrder) == ASCENDANT){printColor("Ascendant",GREEN);}else{printColor("Descendant",RED);}}
        printf("\n");

        if (cursor  == SORT_BY_FILE_NUM) {printf(" --> ");}else {printf("     ");}
        printf("Most Recent\t");
        if (cursor  == SORT_BY_FILE_NUM) {if((*sortingOrder) == ASCENDANT){printColor("Ascendant ",GREEN);printf("(Newest Last)");}else{printColor("Descendant ",RED);printf("(Newest First)");}}
        printf("\n");

        if (cursor == SORT_BY_EXTENSION) {printf(" --> ");}else {printf("     ");}
        printf("Extension\t");
        if (cursor  == SORT_BY_EXTENSION) {if((*sortingOrder) == ASCENDANT){printColor("Ascendant",GREEN);}else{printColor("Descendant",RED);}}
        printf("\n");



        printf("\nUP/DOWN    - Change Selected Option");
        printf("\nLEFT/RIGHT - Change Sorting Order\n");
        printf("\nENTER  - Confirm");
        printf("\nSPACE  - Change Sorting Order");
        printf("\nESC    - Go Back\n");
        input = getch();

        if (input == ARROW_DOWN)
        {
            if (cursor + 1 <= 3)
            {
                cursor++;
                if (cursor == SORT_BY_FILE_NUM)
                {
                    if (changedOption == 0)
                    {
                        (*sortingOrder) = DESCENDANT;
                    }

                }
                else
                {
                    if (changedOption == 0)
                    {
                        (*sortingOrder) = ASCENDANT;
                    }
                }
            }
            else
            {
                cursor = 0;
            }
        }
        else if (input == ARROW_UP)
        {
            if (cursor - 1 >= 0)
            {
                cursor--;
                if (cursor == SORT_BY_FILE_NUM)
                {
                    if (changedOption == 0)
                    {
                        (*sortingOrder) = DESCENDANT;
                    }

                }
                else
                {
                    if (changedOption == 0)
                    {
                        (*sortingOrder) = ASCENDANT;
                    }
                }
            }
            else
            {
                cursor = 3;
            }
        }
        else if (input == ENTER)
        {
            (*sortingType) = cursor;
            return;
        }
         else if (input == ESC)
         {
             return;
         }
         else if (input == SPACE || input == ARROW_LEFT || input == ARROW_RIGHT)
         {
             (*sortingOrder) = !(*sortingOrder);
             changedOption = 1;
         }
    }
}

void printBar(int longestLength)
{
    int i = 0;
    /*int subtitlesLength = strlen("Remember: All the files you extract are saved to: \"\\Paz\\patcher_resources\\extracted_files\\\"");
    if (longestLength < subtitlesLength)
    {
        longestLength = subtitlesLength;
    }*/
    for (i = 0; i < longestLength; i++)
    {
        printf("-");
    }
    //printf("(%d)",longestLength);
}


void printSortSettings(int sortingType, int sortingOrder, char* pathSoFar,int rowsTotalLength,int operation)
{
    char title[32] = "";

    if (sortingType == SORT_BY_FILE_NAME)
    {
        strcat(title,"File Name ");
    }
    else if (sortingType == SORT_BY_REAL_NAME)
    {
        strcat(title,"Real Name ");
    }
    else if (sortingType == SORT_BY_FILE_NUM)
    {
       strcat(title,"Most Recent ");
    }
    else if (sortingType == SORT_BY_EXTENSION)
    {
        strcat(title,"Extension ");
    }
    else
    {
        strcat(title,"Unknown ");
    }
     strcat(title,"| ");
    if (sortingOrder == ASCENDANT)
    {
        strcat(title,"Ascendant");
    }
    else if (sortingOrder == DESCENDANT)
    {
        strcat(title,"Descendant");
    }
    int lengthToCompensate = 0;
    if (operation == SEARCH)
    {
        lengthToCompensate = rowsTotalLength - (strlen("Search Results: ") + strlen(pathSoFar) + strlen("Sort by: ") + strlen(title));
    }
    else if (operation == BROWSE)
    {
        lengthToCompensate = rowsTotalLength - (strlen("Current path: ") + strlen(pathSoFar) + strlen("Sort by: ") + strlen(title));
    }

    int i = 0;
    for (i = 0; i < lengthToCompensate; i++)
    {
        printf(" ");
    }
    printf("Sort by: ");
    printColor(title,CYAN);

  /* printf("\n");
    printBar(rowsTotalLength);
    printf("\n");
   printf("lengthToCompensate: %d\n", lengthToCompensate);
    printf("rowsTotalLength: %d\n", rowsTotalLength);
    printf("strlen(\"Current path: \"): %d\n", strlen("Current path: "));
    printf("strlen(pathSoFar): %d\n", strlen(pathSoFar));
    printf("strlen(\"Sort by: \"): %d\n", strlen("Sort by: "));
    printf(" strlen(title): %d\n",  strlen(title));
    PAUSE();*/

}

int notRecommendedSettings(FileBlock** filesOnMenu,int nOptions, int sortingType, int sortingOrder)
{
    if (nOptions == 0)
    {
        return 0;
    }
    if (!filesOnMenu[0]->isAFolder)
    {
        if (beginsWith(filesOnMenu[0]->folderName,"character/model/1_pc") && contains(filesOnMenu[0]->folderName,"armor"))
        {
            if (!(sortingType == SORT_BY_FILE_NUM && sortingOrder == DESCENDANT))
            {
                return 1;
            }
        }
    }
    return 0;
}
void printEquivalentClassOnUi(char* folderName)
{
    if (strstr(folderName,"1_phm/"))
    {
        printf("   (Warrior)");
    }
    else if (strstr(folderName,"2_phw/"))
    {
        printf("   (Sorceress)");
    }
    else if (strstr(folderName,"3_pew/"))
    {
        printf("   (Ranger)");
    }
    else if (strstr(folderName,"4_pgb/"))
    {
        printf("   (Berserker)");
    }
    else if (strstr(folderName,"4_pgm/"))
    {
        printf("   (Berserker)");
    }
    else if (strstr(folderName,"5_pbw/"))
    {
        printf("   (Tamer)");
    }
    else if (strstr(folderName,"6_pkm/"))
    {
        printf("   (Blader (Musa))");
    }
    else if (strstr(folderName,"7_pvw/"))
    {
        printf("   (Valkyrie)");
    }
    else if (strstr(folderName,"8_pwm/"))
    {
        printf("   (Wizard)");
    }
    else if (strstr(folderName,"8_pww/"))
    {
        printf("   (Witch)");
    }
    else if (strstr(folderName,"13_pnm/"))
    {
        printf("  (Ninja)");
    }
    else if (strstr(folderName,"13_pnw/"))
    {
        printf("  (Kunoichi)");
    }
    else if (strstr(folderName,"15_pdw/"))
    {
        printf("  (Dark Knight)");
    }
    else if (strstr(folderName,"16_pcm/"))
    {
        printf("  (Striker)");
    }
    else if (strstr(folderName,"22_pkww/"))
    {
        printf(" (Maehwa)");
    }
}

void displayFiles_fileExplorer(FileBlock** filesOnMenu,int nOptions, int cursor, int pageStart, int pageEnd, int selectedCount, char* pathSoFar, int sortingType, int sortingOrder, int operation, int* displayRealNames)
{
        int i = 0;
        int j = 0;
        int optionToDisplay = 0;
        int atLeastOneYellow = 0;
        int atLeastOneRealName = 0;
        int atLeastOneFolder = 0;
        int atLeastOneClassExclusive = 0;
        char* nameToDisplay = (char*) malloc(1);
        int rowsTotalLength = getRowsTotalLength(filesOnMenu,nOptions,pageStart,(*displayRealNames));

        int end = pageEnd;
        if (nOptions < pageEnd)
        {
            end = nOptions;
        }

        for (i = pageStart; i < end; i++)
        {
            if (filesOnMenu[i]->hasRealName)
            {
                atLeastOneRealName = 1;
            }
            if (filesOnMenu[i]->isNew)
            {
                atLeastOneYellow = 1;
            }
            if (filesOnMenu[i]->isAFolder)
            {
                atLeastOneFolder = 1;
            }
            if (filesOnMenu[i]->isClassExclusive)
            {
                atLeastOneClassExclusive = 1;
            }
        }


        system("cls");
        printMainHeader();
        if (isWindows10 && (atLeastOneRealName || atLeastOneFolder || atLeastOneClassExclusive || atLeastOneYellow))
        {
            /*printBar(rowsTotalLength);
            printf("\n");*/

            printf("\tColor meaning: ");

            if (atLeastOneRealName && (*displayRealNames))
            {
                printColor("Cash-Shop Item  ", MAGENTA);
                printColor("Non-Cash-Shop Item  ", BLUE);
            }
            if (atLeastOneFolder)
            {
                printColor("Folder  ", GREEN);
                printf("Regular File  ");
            }
            else if (atLeastOneClassExclusive)
            {
                if ((*displayRealNames))
                {
                    printColor("Class-Exclusive Item  ", GREEN);
                }
                else
                {
                    printColor("Class-Exclusive Item  ", DARK_GREEN);
                }
            }
            if(atLeastOneYellow)
            {
                printColor("New File  ", YELLOW);
            }
            printf("\n");
            /*printBar(rowsTotalLength);
            printf("\n");*/
        }
        if (operation == BROWSE)
        {
            printf("\nCurrent path: %s",pathSoFar);
        }
        else if (operation == SEARCH)
        {
            pathSoFar = (char*) malloc(strlen("%d files") + 1);
            sprintf(pathSoFar,"%d files",nOptions);
            printf("Search Results: %s", pathSoFar);
        }

        printSortSettings(sortingType, sortingOrder,pathSoFar, rowsTotalLength,operation);
        printf("\n");
        printBar(rowsTotalLength);

        if(isWindows10)
        {
            printf("\nPage \033[33;1m%d\033[0m - %d ", getCurrentPage(cursor,nOptions),getTotalPages(nOptions));
        }
        else
        {
            printf("\nPage %d - %d", getCurrentPage(cursor,nOptions),getTotalPages(nOptions));
        }

        printf("\n");
        if (nOptions == 0)
        {
            printf("\n  No files to display.\n\n");
        }
        else
        {
            for(i = pageStart; i < nOptions; i++)
            {
                if (filesOnMenu[i]->hasRealName && (*displayRealNames))
                {
                    nameToDisplay = filesOnMenu[i]->RealName;
                }
                else
                {
                    nameToDisplay = filesOnMenu[i]->fileName;
                }

                if (cursor == i)
                {
                    printf(" -->");
                }
                else
                {
                    printf("    ");
                }
                if (filesOnMenu[i]->isSelected)
                {
                    printColorBackground(nameToDisplay,YELLOW);
                }
                else
                {
                    if (filesOnMenu[i]->isAFolder)
                    {
                        printColor(filesOnMenu[i]->fileName,GREEN);
                        printEquivalentClassOnUi(filesOnMenu[i]->fileName);
                    }
                    else
                    {
                        if (filesOnMenu[i]->fileNum == 0)
                        {
                            printf("ai.xml                ");
                        }
                        else
                        {
                            if (filesOnMenu[i]->isNew)
                            {
                                printColor(nameToDisplay,YELLOW);
                            }
                            else
                            {
                                if (filesOnMenu[i]->hasRealName && (*displayRealNames))
                                {
                                    if (filesOnMenu[i]->isClassExclusive)
                                    {
                                        printColor(nameToDisplay,GREEN);
                                    }
                                    else if (filesOnMenu[i]->isNonCashShopItem)
                                    {
                                        printColor(nameToDisplay,BLUE);
                                    }
                                    else
                                    {
                                        printColor(nameToDisplay,MAGENTA);
                                    }
                                }
                                else
                                {
                                     if (filesOnMenu[i]->isClassExclusive)
                                    {
                                        printColor(nameToDisplay,DARK_GREEN);
                                    }
                                    else
                                    {
                                        printf("%s",nameToDisplay);
                                    }
                                }
                            }
                        }
                    }
                }

                compensateLength(filesOnMenu,nOptions,i,(*displayRealNames));

                for (j = 1; j < MAX_ROWS; j++)
                {
                    if (nOptions > (i + (j * MAX_LINES)))
                    {
                        optionToDisplay = i + (j * MAX_LINES);

                        if (filesOnMenu[optionToDisplay]->hasRealName && (*displayRealNames))
                        {
                            nameToDisplay = filesOnMenu[optionToDisplay]->RealName;
                        }
                        else
                        {
                            nameToDisplay = filesOnMenu[optionToDisplay]->fileName;
                        }

                        if (cursor == optionToDisplay)
                        {
                            printf(" -->");
                        }
                        else
                        {
                            printf("    ");
                        }
                        if (filesOnMenu[optionToDisplay]->isSelected)
                        {
                            printColorBackground(nameToDisplay,YELLOW);
                        }
                        else
                        {
                            if (filesOnMenu[optionToDisplay]->isAFolder)
                            {
                                printColor(filesOnMenu[optionToDisplay]->fileName,GREEN);
                            }
                            else
                            {
                                if (filesOnMenu[optionToDisplay]->isNew)
                                {
                                    printColor(nameToDisplay,YELLOW);
                                }
                                else
                                {
                                    if (filesOnMenu[optionToDisplay]->hasRealName && (*displayRealNames))
                                    {
                                        if (filesOnMenu[optionToDisplay]->isClassExclusive)
                                        {
                                            printColor(nameToDisplay,GREEN);
                                        }
                                        else if (filesOnMenu[optionToDisplay]->isNonCashShopItem)
                                        {
                                            printColor(nameToDisplay,BLUE);
                                        }
                                        else
                                        {
                                            printColor(nameToDisplay,MAGENTA);
                                        }
                                    }
                                    else
                                    {
                                        if (filesOnMenu[optionToDisplay]->isClassExclusive)
                                        {
                                            printColor(nameToDisplay,DARK_GREEN);
                                        }
                                        else
                                        {
                                            printf("%s",nameToDisplay);
                                        }
                                    }
                                }
                            }
                        }

                        if (j < MAX_ROWS - 1)
                        {
                            compensateLength(filesOnMenu,nOptions,optionToDisplay,(*displayRealNames));
                        }
                        else
                        {
                            compensateLength(filesOnMenu,nOptions,optionToDisplay,(*displayRealNames));
                        }

                    }
                }
                printf("\n");


                if ((i + 1) >= (pageEnd - ((MAX_ROWS-1)*MAX_LINES)))
                {
                    break;
                }
            }
        }
        printBar(rowsTotalLength);
        if (nOptions == 0)
        {
            printf("\n  ESC - Go back\n");
        }
        else if (filesOnMenu[cursor]->isAFolder)
        {
            printf("\n    ENTER - Go Inside Folder            ESC - Go back");
        }
        else
        {
            if (selectedCount == 0)
            {
                if (isWindows10)
                {
                    printf("\n    ENTER -  Extract File               \033[33;1mSPACE - Select File/Unselect\033[0m          ESC - Go back");
                }
                else
                {
                    printf("\n    ENTER -  Extract File               SPACE - Select File/Unselect          ESC - Go back");
                }
            }
            else
            {
                if (isWindows10)
                {
                    printf("\n    ENTER -  Extract \033[33;1mSelected File(s)\033[0m   SPACE - Select/Unselect File          ESC - Go back");
                }
                else
                {

                    printf("\n    ENTER -  Extract Selected File(s)   SPACE - Select/Unselect File          ESC - Go back");

                }
            }

        }
        if (nOptions != 0)
        {
            printf("\n    ARROWS - Change Selected Option     ");
            if (selectedCount == 0)
            {
                printf("A - Select All from this list");
            }
            else
            {
                if (isWindows10)
                {
                    printf("A - \033[33;1mUnselect All\033[0m from this list");
                }
                else
                {
                    printf("A - Unselect All from this list");
                }
            }
            printf("\n    S - Select this and the one below   W - Unselect this and move one up");
            printColor("\n    F - Change Sorting Type",CYAN);;if (atLeastOneRealName) { printColor("             N - Toggle Real Names/File Names",MAGENTA);}
            if (atLeastOneRealName)
            {
                if (isWindows10)
                {
                    printf("\n\n   All the \"In-Game Names\" are stored in \"patcher_resources\\");printColor("real_names.txt",YELLOW);printf("\", you can add more if you want");
                }
                else
                {
                    printf("\n\nAll the \"In-Game Names\" are stored in \"patcher_resources\\real_names.txt\", you can add more if you want");
                }
            }

            //printf("\n\nRemember: All the files you extract are saved to: \"\\Paz\\patcher_resources\\extracted_files\\\"");
            printf("\n");
        }
}


void searchResults(char* searchQuery, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo)
{
    int i = 0;
    int filesFoundCount = 0;

    List* filesToSearch = NULL;

    char* element = strtok(searchQuery," ,");
    while (element != NULL)
    {
        if (strlen(element) > 0)
        {
            int lastSlashIdx = 0;
            for (i = 0; i < strlen(element); i++)
            {
                if (element[i] == '/' || element[i] == '\\')
                {
                    lastSlashIdx = i;
                }
            }
            if (lastSlashIdx != 0)
            {
                FileBlock* fileBlock = (FileBlock*) malloc(sizeof(FileBlock));

                fileBlock->folderName = substr(element,0,lastSlashIdx+1);
                charReplace(fileBlock->folderName,'\\','/');

                fileBlock->fileName = substr(element,lastSlashIdx+1,strlen(element));

                insertList(fileBlock,&filesToSearch);
            }
            else
            {
                FileBlock* fileBlock = (FileBlock*) malloc(sizeof(FileBlock));
                fileBlock->fileName = element;
                fileBlock->folderName = NULL;

                insertList(fileBlock,&filesToSearch);
            }
        }
        element = strtok(NULL," ,");
    }

    printf("\nSearching...");
    int run = 0;
    int currentFile = 0;
    FileBlock** searchResultFiles = NULL;

    for (run = 0;  run < 2; run++)
    {
        if (run == 1)
        {
            searchResultFiles = (FileBlock**) malloc((filesFoundCount + 1) * sizeof(FileBlock*));
        }

        for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
        {
            struct ListNode* fileToSearchNode = filesToSearch;
            while(fileToSearchNode != NULL)
            {
                if (containsStrict(fileBlocks[i].fileName,fileToSearchNode->fileBlock->fileName))
                {
                    if (fileToSearchNode->fileBlock->folderName != NULL)
                    {
                        if (contains(fileBlocks[i].folderName,fileToSearchNode->fileBlock->folderName))
                        {
                            if (run == 0)
                            {
                                filesFoundCount++;
                            }
                            else
                            {
                                searchResultFiles[currentFile] = &fileBlocks[i];
                                currentFile++;
                            }
                        }
                    }
                    else
                    {
                        if (run == 0)
                        {
                            filesFoundCount++;
                        }
                        else
                        {
                            searchResultFiles[currentFile] = &fileBlocks[i];
                            currentFile++;
                        }
                    }
                }

                fileToSearchNode = fileToSearchNode->next;
            }
        }
    }
    printf("\nDone\n");


    char* pathSoFar = "";
    char* previousExtractLocation = "";
    int sortingType = SORT_BY_FILE_NAME;
    int sortingOrder = ASCENDANT;
    int cursor = 0;

    int displayRealNames = 0;
    fileExplorer(&cursor,searchResultFiles,filesFoundCount,fileBlocks,metaFileInfo,NULL,0,pathSoFar,previousExtractLocation,&sortingType,&sortingOrder,SEARCH,&displayRealNames);
}

void nextPage(int* pageStart, int* pageEnd,int* longestLength, FileBlock** filesOnMenu,int nOptions,MetaFileInfo*  metaFileInfo, int biggerHeight,int* displayRealName)
{
    if ((*pageEnd) < nOptions)
    {
        (*pageStart) = (*pageEnd);
        (*pageEnd) += MAX_LINES* MAX_ROWS;
    }
    recalculateSettings(filesOnMenu, nOptions,(*pageStart), biggerHeight,displayRealName);
}

void previousPage(int* pageStart, int* pageEnd,int* longestLength, FileBlock** filesOnMenu,int nOptions,MetaFileInfo*  metaFileInfo, int biggerHeight, int* displayRealName)
{
    if ((*pageStart) - (MAX_LINES* MAX_ROWS) >= 0)
    {
        (*pageStart) -= (MAX_LINES* MAX_ROWS);
        (*pageEnd) -= MAX_LINES* MAX_ROWS;
    }
    recalculateSettings(filesOnMenu, nOptions,(*pageStart),biggerHeight,displayRealName);
}

int getCurrentPage(int cursor,int nOptions)
{
    return ((int)(cursor/(MAX_LINES*MAX_ROWS)) + 1);
}

int getTotalPages(int nOptions)
{
    int result = (int)(nOptions/(MAX_LINES*MAX_ROWS));
    if ((nOptions % (MAX_LINES*MAX_ROWS)) != 0)
    {
        result += 1;
    }
    return (result);
}


void readRealNamesFile(FileBlock** filesOnMenu, int nOptions)
{
    FILE* armor_to_swap_file = openFile("patcher_resources\\real_names.txt","rb");
    char c;
    int i = 0;
    int arrowFound = 0;
    int readingArmorNumber = 1;
    int result = 0;
    Armor* armor = NULL;

    int linesCount = 0;
    int lineLength = 1;
    while(fread(&c,1,1,armor_to_swap_file))
    {
        //Skip comments
        if (c == '#')
        {
            while(c!= '\n')
            {
                if (!fread(&c,1,1,armor_to_swap_file))
                {
                    break;
                }
            }
        }
        else if (c == '\n')
        {
            if (lineLength > 1)
            {
                linesCount++;
            }
            lineLength = 0;
        }
        else
        {
            lineLength++;
        }
    }

    rewind(armor_to_swap_file);


    armor = (Armor*) malloc((linesCount + 1) * sizeof(Armor));
    for(i = 0; i < linesCount; i++)
    {
        armor[i].fileName   = (char*) malloc(128);
        armor[i].RealName = (char*) malloc(128);
    }
    i = 0;
    int currentArmor = 0;
    while (1)
    {
        result = fread(&c,1,1,armor_to_swap_file);

        //Skip comments
        if (c == '#')
        {
            while(c != '\n')
            {
                if (!fread(&c,1,1,armor_to_swap_file))
                {
                    break;
                }
            }
        }
        else if (readingArmorNumber == 1 && c != '\n')
        {
            armor[currentArmor].fileName[i] = c;
            i++;
        }
        else if (readingArmorNumber == 2 && c != '\n')
        {
            armor[currentArmor].RealName[i] = c;
            i++;
        }

        if (c == '\n' && arrowFound == 0)
        {
            //printf("\nSymbol \"->\" not found");
            i = 0;
        }
        if (c == '-')
        {
            fread(&c,1,1,armor_to_swap_file);
            if (c == '>')
            {
                armor[currentArmor].fileName[i-1] = '\0';
                arrowFound = 1;
                readingArmorNumber = 2;

            }
            i = 0;
        }
        if ((c == '\n' && arrowFound == 1) || (result == 0 && arrowFound == 1))
        {
            armor[currentArmor].RealName[i-1] = '\0';
           /* printf("\narmor1: %s", armor[currentArmor].fileName);
            printf("\narmor2: %s\n\n", armor[currentArmor].RealName);*/
            arrowFound = 0;
            readingArmorNumber = 1;
            i = 0;
            currentArmor++;
        }
        if (result == 0)
        {
            break;
        }
    }

    /*for(i = 0; i < linesCount; i++)
    {
        printf("\n%s->%s",armor[i].fileName ,armor[i].RealName);
    }
    PAUSE();*/

    int j = 0;
    for (i = 0; i < nOptions; i++)
    {
        // Makes sure this doesn't run again if is already filled
        if (!filesOnMenu[i]->triedToUpdateRealName)
        {
            filesOnMenu[i]->triedToUpdateRealName = 1;

            if (isRegular(filesOnMenu[i]->fileName)) // If it's NOT a Cash Shop Item
            {
                filesOnMenu[i]->isNonCashShopItem = 1;
            }

            for(j = 0; j < linesCount; j++)
            {
                //printf("\n%s  - %s (%d)", armor[j].fileName,filesOnMenu[i]->fileName,contains(filesOnMenu[i]->fileName,armor[j].fileName));

                if (contains(filesOnMenu[i]->fileName,armor[j].fileName))
                {
                    if (strstr(filesOnMenu[i]->fileName,"uw") && strstr(armor[j].fileName,"uw"))
                    {
                        char* fourDigits = getFourDigits(filesOnMenu[i]->fileName);
                        if ((fourDigits[0] == '1') && !strstr(armor[j].RealName,"Sileshi"))
                        {
                            filesOnMenu[i]->RealName = concatenate(armor[j].RealName," Underwear (No Stockings)");
                        }
                        else
                        {
                            filesOnMenu[i]->RealName = concatenate(armor[j].RealName," Underwear");
                        }
                        filesOnMenu[i]->hasRealName = 1;

                       /* printf("\n%s", filesOnMenu[i]->RealName);
                        PAUSE();*/
                    }
                    else if (!strstr(filesOnMenu[i]->fileName,"uw") && filesOnMenu[i]->fileName[0] == 'p' && (strstr(filesOnMenu[i]->folderName,"armor") || strstr(filesOnMenu[i]->folderName,"weapon") || contains(filesOnMenu[i]->fileName,"*_10_*_0004*")))
                    {
                            if (isRegular(filesOnMenu[i]->fileName)) // If it's NOT a Cash Shop Item
                            {
                                filesOnMenu[i]->isNonCashShopItem = 1;

                                // If it has no name yet
                                if ((filesOnMenu[i]->RealName == NULL) || ((filesOnMenu[i]->RealName != NULL) && (strlen(filesOnMenu[i]->RealName) < 4)))
                                {
                                     if (!strstr(armor[j].RealName,"Guessed"))
                                     {
                                        filesOnMenu[i]->RealName = armor[j].RealName;
                                        filesOnMenu[i]->hasRealName = 1;
                                     }
                                    else // If it's a guessed name
                                    {
                                        char* classPrefix = getClassPrefix(filesOnMenu[i]->fileName);
                                        if (!beginsWith(classPrefix,"phw")
                                        && !beginsWith(classPrefix,"pew")
                                        && !beginsWith(classPrefix,"pvw")
                                        && !beginsWith(classPrefix,"pbw")
                                        && !beginsWith(classPrefix,"pww")
                                        && !beginsWith(classPrefix,"phm")
                                        && !beginsWith(classPrefix,"pgm")
                                        && !beginsWith(classPrefix,"pkww")
                                        && !beginsWith(classPrefix,"pkm")
                                        )
                                        {
                                            filesOnMenu[i]->RealName = armor[j].RealName;
                                             filesOnMenu[i]->hasRealName = 1;
                                        }
                                    }


                                }
                                else // If a name was already assigned
                                {   // and this isn't a guessed name
                                    if (!strstr(armor[j].RealName,"Guessed"))
                                    {
                                        filesOnMenu[i]->RealName = concatenate3(filesOnMenu[i]->RealName,"/",armor[j].RealName);
                                        filesOnMenu[i]->hasRealName = 1;
                                    }
                                }
                            }
                            else // Case is IS a Cash Shop item
                            {
                                filesOnMenu[i]->RealName = armor[j].RealName;
                                filesOnMenu[i]->hasRealName = 1;
                            }

                    }
                }
            }

            for(j = 0; j < linesCount; j++)
            {
                if (filesOnMenu[i]->hasRealName)
                {
                    char* partName = NULL;

                    if (strstr(filesOnMenu[i]->folderName,"weapon"))
                    {
                        if (strstr(filesOnMenu[i]->folderName,"1_onehandsword"))
                        {
                            partName = concatenate(partName," One Hand Sword");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"8_shield"))
                        {
                            partName = concatenate(partName," Shield");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"3_twohandsword"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }

                        if (strstr(filesOnMenu[i]->folderName,"28_enchantarm"))
                        {
                            partName = concatenate(partName," Amulet");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"33_talisman"))
                        {
                            partName = concatenate(partName," Talisman");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"38_scythe"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }




                        if (strstr(filesOnMenu[i]->folderName,"31_onehandbow") || strstr(filesOnMenu[i]->fileName,"bow"))
                        {
                            partName = concatenate(partName," Bow");
                        }
                        else if (strstr(filesOnMenu[i]->fileName,"arw"))
                        {
                            partName = concatenate(partName," Arrow");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"32_dagger"))
                        {
                            partName = concatenate(partName," Dagger");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"43_twinsword"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }



                        if (strstr(filesOnMenu[i]->folderName,"29_doubleax"))
                        {
                            partName = concatenate(partName," Axe");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"34_aiguillette"))
                        {
                            partName = concatenate(partName," Aiguillette");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"42_handcanon"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }


                        if (strstr(filesOnMenu[i]->folderName,"37_norigae"))
                        {
                            partName = concatenate(partName," Trinket");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"44_stick"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }



                        if (strstr(filesOnMenu[i]->folderName,"35_blade"))
                        {
                            partName = concatenate(partName," Blade");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"36_shotbow"))
                        {
                            partName = concatenate(partName," Short Bow");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"47_glaive"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }




                        if (strstr(filesOnMenu[i]->folderName,"45_spear"))
                        {
                            partName = concatenate(partName," Spear");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"46_spearshield"))
                        {
                            partName = concatenate(partName," Spear Shield");
                        }


                        if (strstr(filesOnMenu[i]->folderName,"6_staff"))
                        {
                            partName = concatenate(partName," Staff");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"32_dagger"))
                        {
                            partName = concatenate(partName," Dagger");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"59_orb"))
                        {
                            partName = concatenate(partName," Orbs");
                        }



                        if (strstr(filesOnMenu[i]->folderName,"30_elblade"))
                        {
                            partName = concatenate(partName," Blade");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"55_shuriken"))
                        {
                            partName = concatenate(partName," Shuriken");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"56_darts"))
                        {
                            partName = concatenate(partName," Dart");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"57_chakram"))
                        {
                            partName = concatenate(partName," Chakram");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"58_katana"))
                        {
                            partName = concatenate(partName," Katana");
                        }


                        if (strstr(filesOnMenu[i]->folderName,"63_longblade"))
                        {
                            partName = concatenate(partName," Long Blade");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"64_darksword"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }


                         if (strstr(filesOnMenu[i]->folderName,"65_gauntlet"))
                        {
                            partName = concatenate(partName," Gauntlet");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"66_armlet"))
                        {
                            partName = concatenate(partName," Armlet");
                        }


                        if (strstr(filesOnMenu[i]->folderName,"47_glaive"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }

                        if (strstr(filesOnMenu[i]->folderName,"71_godgauntlet"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }


                        if (strstr(filesOnMenu[i]->fileName,"_in.pac"))
                        {
                            partName = concatenate(partName," (Not Withdrawn)");
                        }
                    }
                    else
                    {
                        if (strstr(filesOnMenu[i]->fileName,"_hand_"))
                        {
                            partName = concatenate(partName," Gloves");
                        }
                        else if (strstr(filesOnMenu[i]->fileName,"_foot_"))
                        {
                            partName = concatenate(partName," Boots");
                        }
                        else if (strstr(filesOnMenu[i]->fileName,"_cloak_"))
                        {
                            partName = concatenate(partName," Cape");
                        }
                        else if (strstr(filesOnMenu[i]->fileName,"_hel_"))
                        {
                            partName = concatenate(partName," Helmet");
                        }
                        else
                        {
                            if (filesOnMenu[i]->isNonCashShopItem)
                            {
                                partName = concatenate(partName," Armor");
                            }
                            else if (strstr(filesOnMenu[i]->RealName,"Shark")
                                     || strstr(filesOnMenu[i]->RealName,"Desert")
                                     || strstr(filesOnMenu[i]->RealName,"Fish")
                                     || strstr(filesOnMenu[i]->RealName,"Treant")
                                     || strstr(filesOnMenu[i]->RealName,"Riding")
                                     || strstr(filesOnMenu[i]->RealName,"Maid")
                                     || strstr(filesOnMenu[i]->RealName,"Canape")
                                     || strstr(filesOnMenu[i]->RealName,"Masquerade")
                                     )
                            {
                                partName = concatenate(partName," Costume");
                            }
                            // If it's not an underwear
                            else if (!strstr(filesOnMenu[i]->fileName,"uw"))
                            {
                                partName = concatenate(partName," Outfit");
                            }

                        }
                    }
                    if (filesOnMenu[i]->hasRealName)
                    {
                        if (strstr(filesOnMenu[i]->fileName,"_01.pac")
                         || strstr(filesOnMenu[i]->fileName,"_02.pac")
                         || strstr(filesOnMenu[i]->fileName,"_03.pac")
                         || strstr(filesOnMenu[i]->fileName,"_04.pac")
                         || strstr(filesOnMenu[i]->fileName,"_06.pac")
                         || strstr(filesOnMenu[i]->fileName,"_07.pac")
                         || strstr(filesOnMenu[i]->fileName,"_08.pac")
                         || strstr(filesOnMenu[i]->fileName,"_09.pac")
                         || strstr(filesOnMenu[i]->fileName,"_10.pac")
                         )
                        {
                            if (strstr(filesOnMenu[i]->fileName,"_ub_0034_01.pac"))
                            {
                                partName = concatenate(partName," (Non-Winged)");
                            }
                            else
                            {
                                char variationName[16] = "";
                                sprintf(variationName," Variation %c",filesOnMenu[i]->fileName[strlen(filesOnMenu[i]->fileName) - 5]);
                                partName = concatenate(partName,variationName);
                            }
                        }
                        if (strstr(filesOnMenu[i]->fileName,"_01_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_02_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_03_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_04_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_06_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_07_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_08_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_09_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_10_dm.pac"))
                         {
                            if (strstr(filesOnMenu[i]->fileName,"_ub_0034_01_dm.pac"))
                            {
                                partName = concatenate(partName," (Non-Winged)");
                            }
                            else
                            {
                                char variationName[16] = "";
                                sprintf(variationName," Variation %c",filesOnMenu[i]->fileName[strlen(filesOnMenu[i]->fileName) - 8]);
                                partName = concatenate(partName,variationName);
                            }
                         }

                        if (strstr(filesOnMenu[i]->fileName,"_dm"))
                        {
                            partName = concatenate(partName," Damaged");
                        }

                         if (strstr(filesOnMenu[i]->fileName,"_mul"))
                         {
                            partName = concatenate(partName," (Awakening Weapon Withdrawn)");
                         }
                         if (strstr(filesOnMenu[i]->fileName,"_na.pac"))
                         {
                            partName = concatenate(partName," NA");
                         }
                        if (strstr(filesOnMenu[i]->fileName,"_s.pac")|| strstr(filesOnMenu[i]->fileName,"_s_dm.pac"))
                         {
                            partName = concatenate(partName," S");
                         }
                    }


                    if (partName != NULL)
                    {
                        if (!filesOnMenu[i]->partNameAlreadyAssined)
                        {
                            filesOnMenu[i]->RealName = concatenate(filesOnMenu[i]->RealName,partName);
                            filesOnMenu[i]->partNameAlreadyAssined = 1;
                        }
                    }

                }
            }

        }
    }
}



