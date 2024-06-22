#ifndef META_EXPLORER_H
#define META_EXPLORER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/ice.h"
#include "../include/global.h"
#include "../include/utilities.h"
#include "../include/file_operations.h"



void readBlock(FileBlock* fileBlock, FILE* fp);
void printBlock(FileBlock* fileBlock);
long getPazCount(char* metaFileName);
long getFileCount(char* metaFileName);
MetaFileInfo* getMetaFileInfo(char* metaFileName);
void printMetaFileInfo(MetaFileInfo* metaFileInfo);
FileBlock* fillFileBlocks(MetaFileInfo* metaFileInfo);
FileBlock** getFilteredFileBlocks(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, char* folderNameFilter, int* filteredFilesCount);
#endif
