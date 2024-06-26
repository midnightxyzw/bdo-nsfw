#ifndef META_EXPLORER_H
#define META_EXPLORER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/ice.h"
#include "../include/global.h"
#include "../include/utilities.h"
#include "../include/file_operations.h"
#include "../include/folder_tree.h"
#include "../include/sorting.h"
#include "../include/list.h"




void readBlock(FileBlock* fileBlock, FILE* fp);
void printBlock(FileBlock* fileBlock);
long getPazCount(char* metaFileName);
long getFileCount(char* metaFileName);
MetaFileInfo* getMetaFileInfo(char* metaFileName);
void printMetaFileInfo(MetaFileInfo* metaFileInfo);
FileBlock* fillFileBlocks(MetaFileInfo* metaFileInfo, char* metaFileName, int useArmorNamesFile);
struct Folder* fillFolderTree(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);
char** fillFolderNamesList(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, int* folderNamesCount);
long* getFileBlocksHashes(char* metaFileName, long* hashesCount);
#endif
