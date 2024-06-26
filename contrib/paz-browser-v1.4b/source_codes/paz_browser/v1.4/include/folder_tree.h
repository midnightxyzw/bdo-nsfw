#ifndef FOLDER_TREE_H
#define FOLDER_TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/global.h"

struct Folder
{
  char* folderName;
  struct Folder *subfolder;
  struct Folder* next;
  struct Folder* prev;
};

void insertFolder(char* folderNameToInsert, struct Folder **node);
void insertSubFolder(char* folderNameToInsert, struct Folder **node);
void printSubFolders(struct Folder *folder);
void printFolderTree(struct Folder *rootFolder);
struct Folder* getFolder(char* folderName, struct Folder* rootFolder);
void printFolderTreeDetailed(struct Folder *rootFolder);
void printSubFoldersDetailed(struct Folder *folder);
#endif // TREE_H
