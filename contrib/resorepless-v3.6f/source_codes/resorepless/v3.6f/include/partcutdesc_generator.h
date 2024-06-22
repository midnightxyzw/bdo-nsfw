#ifndef PARTCUTDESC_GENERATOR_H
#define PARTCUTDESC_GENERATOR_H
#include <stdio.h>
#include <stdlib.h>
#include "../include/data_types.h"
#include "../include/utilities.h"
#include "../include/patcher.h"

typedef struct
{
    char* fileName;
    long lastCloseTag;
    long lastSlash;
    long fileNameEnd;
} PartCutDescEntry;

extern int removedCount;

void addToPartCutDesc(FileBlock* fileBlock, FILE* partcutdesc);
void createCutTypeBlock(char* blockName,char* compareFolderName,int* classAllowed, FILE* partcutdesc, int fileBlockCount, FileBlock** fileBlocks);
void generatePartcutdesc(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo,PatcherState* patcherState);
void writestr(char* str,FILE* fp);
char* getFileBlockFullPath(FileBlock* fileBlock);
void modifyPartcutdesc(char* pathToPartcutdesc, FileBlock** fileBlocks, int fileBlocksCount);
int markCensorshipRemoval(FileBlock** fileBlocks, int fileBlocksCount,PatcherState* patcherState);


#endif // PARTCUTDESC_GENERATOR_H
