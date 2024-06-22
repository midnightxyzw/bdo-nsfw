#ifndef PAZ_BROWSER_H
#define PAZ_BROWSER_H
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/utilities.h"
#include "../include/meta_explorer.h"
#include "../include/partcutdesc_generator.h"

typedef struct
{
    char* fileName;
    char* RealName;
} Armor;

extern int MAX_FILE_NAME_LENGTH;
extern int MAX_LINES;
extern int MAX_ROWS;

extern int displayRealFileNames;

void readRealNamesFile(FileBlock** filesOnMenu, int nOptions);

void compensateLength(FileBlock** filesOnMenu,int nOptions, int currentFileIDX);
int getCursorRow(int cursorPos);
int getCursorLine(int cursorPos);
void nextPage(int* pageStart, int* pageEnd,int* longestLength, FileBlock** filesOnMenu,int nOptions,MetaFileInfo*  metaFileInfo);
void previousPage(int* pageStart, int* pageEnd,int* longestLength, FileBlock** filesOnMenu,int nOptions,MetaFileInfo*  metaFileInfo);
void goToPage(int pageNum, int* pageStart, int* pageEnd, int nOptions);
void goToLastPage(int* pageStart, int* pageEnd, int nOptions);
int getCurrentPage(int cursor,int nOptions);
int getTotalPages(int nOptions);
char* getPartFolderPrefix(char* partSelected);
char* getClassPrefixFromMenuOption(char* classSelected);
char* getPartPrefixFromMenuOption(char* partSelected);
char* menuSelectClass(int* classSelected_idx);
char* menuSelectPart(int* partSelected_idx);
FileBlock* menuSelectFile(char* classSelected,char* partSelected,char* fileExt, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, int operation,PatcherState* patcherState);
void previewFile(FileBlock* fileBlockSelected, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, int openAfterExtracted);
int runPazBrowser(FileBlock* fileBlocks, MetaFileInfo *metaFileInfo, char* operationName, int operation,PatcherState* patcherState);
void uiMenuSelectFile(FileBlock** filesOnMenu,int nOptions, int cursor, int pageStart, int pageEnd, int longestLength, int operation, int selectAll, MetaFileInfo* metaFileInfo);
void uiMenuSelectFileColor(FileBlock** filesOnMenu,int nOptions, int cursor, int pageStart, int pageEnd, int longestLength, int operation, int selectAll, MetaFileInfo* metaFileInfo);
void loadMarkedFiles(FileBlock* fileBlocks, MetaFileInfo *metaFileInfo, int operation);
int recalculateSettings(FileBlock** filesOnMenu, int nOptions, int pageStart, MetaFileInfo* metaFileInfo);
int getRowsTotalLength(FileBlock** filesOnMenu,int nOptions,int pageStart);
#endif // PAZ_BROWSER_H
