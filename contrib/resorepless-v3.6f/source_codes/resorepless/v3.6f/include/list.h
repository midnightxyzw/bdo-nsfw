#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/global.h"
#include "../include/data_types.h"

typedef struct ListNode
{
  FileBlock* fileBlock;
  struct ListNode* next;
} List;

void insertList(FileBlock* fileBlockToInsert, List** list);
void printList(List* list);
List* getListElement(FileBlock* fileBlock, List* list);
void printListDetailed(List *list);
int getListSize(List* list);
FileBlock** getListAsArray(List* list, int listSize);
void freeList(List* list);
#endif // TREE_H
