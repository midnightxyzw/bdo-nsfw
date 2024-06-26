#ifndef FILE_EXPLORER_H
#define FILE_EXPLORER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/global.h"
#include "../include/utilities.h"
#include "../include/sorting.h"
#include "../include/list.h"

#define NEXT_FOLDER 3
#define SORT 4

#define MAX_LINES 16
#define MAX_ROWS 3

typedef struct
{
    char* fileName;
    char* RealName;
} Armor;

int fileExplorer(int* cursor, FileBlock** filesOnMenu, int nOptions, FileBlock* fileBlocks,  MetaFileInfo* metaFileInfo,long* previousBackupHashes, long previousBackupHashesCount , char* pathSoFar, char* previousExtractLocation, int* sortingType,int* sortingOrder, int operation, int* displayRealNames);
int folderMenu(char* startingPath,char** folderNamesList,int folderNamesCount, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, long* previousBackupHashes, long previousBackupHashesCount);
int recalculateFilesAndFoldersToDisplay(char** folderNamesList,int folderNamesCount, char* pathSoFar, FileBlock* fileBlocks,  MetaFileInfo* metaFileInfo, long* previousBackupHashes, long previousBackupHashesCount, char* previousExtractLocation, int* sortingType,int* sortingOrder, int* displayRealNames);
void displayFiles_fileExplorer(FileBlock** filesOnMenu,int nOptions, int cursor, int pageStart, int pageEnd, int selectedCount, char* pathSoFar, int sortingType, int sortingOrder, int operation, int* displayRealNames);
FileBlock** getFoldersToDisplay(char** folderNamesList, int folderNamesCount, int* foldersToDisplayCount, char* pathSoFar);
FileBlock** getFilesToDisplay(int* filesToDisplayCount, char* pathSoFar, FileBlock* fileBlocks, MetaFileInfo* metafileInfo, int sortingType, int sortingOrder);
void selectFile(FileBlock* fileToSelect, int* selectedCount);
void unselectFile(FileBlock* fileToUnselect, int* selectedCount);
void menuSortingType(int* sortingType, int* sortingOrder);
void sortFileNames(FileBlock** filesToDisplay, int filesToDisplayCount, int sortingType, int sortingOrder);
void printSortSettings(int sortingType, int sortingOrder, char* pathSoFar, int rowsTotalLength, int operation);
char* askExtractLocation(char** previousExtractLocation);
int notRecommendedSettings(FileBlock** filesOnMenu, int nOptions, int sortingType, int sortingOrder);
void searchResults(char* searchQuery, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);
void applyFileSorting(FileBlock** filesOnMenu, int nOptions, int sortingType,int sortingOrder, int pageStart, int pageEnd, MetaFileInfo* metaFileInfo, int* displayRealName);
int recalculateSettings(FileBlock** filesOnMenu, int nOptions, int pageStart, int biggerHeight, int* diplayRealNames);
void nextPage(int* pageStart, int* pageEnd,int* longestLength, FileBlock** filesOnMenu,int nOptions,MetaFileInfo*  metaFileInfo, int biggerHeight,int* displayRealName);
void previousPage(int* pageStart, int* pageEnd,int* longestLength, FileBlock** filesOnMenu,int nOptions,MetaFileInfo*  metaFileInfo, int biggerHeight, int* displayRealName);
int getCurrentPage(int cursor,int nOptions);
int getTotalPages(int nOptions);
int getRowsTotalLength(FileBlock** filesOnMenu,int nOptions,int pageStart, int displayRealFileNames);
void compensateLength(FileBlock** filesOnMenu,int nOptions, int currentFileIDX, int displayRealFileNames);
void readRealNamesFile(FileBlock** filesOnMenu, int nOptions);
#endif // FILE_EXPLORER_H
