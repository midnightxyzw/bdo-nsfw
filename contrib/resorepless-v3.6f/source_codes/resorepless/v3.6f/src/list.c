#include "../include/list.h"

void insertList(FileBlock* fileBlockToInsert, List** list)
{
    if((*list) == NULL )
    {
        (*list) = (List*) malloc(sizeof(List));

        (*list)->fileBlock = fileBlockToInsert;

        (*list)->next = NULL;
    }
    else if ((*list)->next == NULL)
    {
        (*list)->next = (List*) malloc(sizeof(List));

        (*list)->next->fileBlock = fileBlockToInsert;
        (*list)->next->next = NULL;
    }
    else if ((*list)->next != NULL)
    {
        insertList(fileBlockToInsert, &(*list)->next);
    }
}

void printList(List *list)
{
    List* node = list;
    while(node != NULL)
    {
        if (node->fileBlock != NULL)
        {
            printf("\n\n%s:", node->fileBlock->fileName);
        }
        else
        {
            printf("\nNULL");
        }
        node = node->next;
    }
}

List* getListElement(FileBlock* fileBlock, List* list)
{
    List* node = list;

    if (0)
    {
        if (list == NULL)
        {
            printf("\nroot folder null");
        }

        printf("\nEvaluating: ");
        if (node == NULL)
        {
            printf("NULL");
        }
        else
        {
            printf("%s", node->fileBlock->fileName);
        }
    }
    while(node != NULL)
    {
        if (node->fileBlock != NULL && node->fileBlock->fileName != NULL)
        {
            if (0)
            printf("\ncomparing %s with %s : %d", node->fileBlock->fileName,fileBlock->fileName,strcmpi(node->fileBlock->fileName,fileBlock->fileName));
            if (strcmpi(node->fileBlock->fileName,fileBlock->fileName) == 0)
            {
                return node;
            }
        }
        else
        {
            printf("\nFolder name null");
        }
        if (0)
        printf("\n\nnode(%s) = node->next", node->fileBlock->fileName);

        node = node->next;

        if (0)
        printf("\nDone");

        if (0)
        {
            if (node == NULL)
            {
                printf("\nNode is NULL");
            }
            else
            {
                printf("\nNode is %s", node->fileBlock->fileName);
            }
         }
    }
    if (0)
    printf("\n\nExited, not found");

    return NULL;
}


int getListSize(List* list)
{
    struct ListNode* node = list;
    int size = 0;
    while(node != NULL)
    {
        if (node->fileBlock != NULL)
        {
            size++;
        }
        node = node->next;
    }
    return size;
}

void freeList(List* list)
{
    struct ListNode* node = list;
    struct ListNode* backup = NULL;

    while(node != NULL)
    {
        if (node->fileBlock != NULL)
        {
            if (node->fileBlock->fileName != NULL)
            {
                free(node->fileBlock->fileName);
            }
            if (node->fileBlock->folderName != NULL)
            {
                free(node->fileBlock->folderName);
            }
            free(node->fileBlock);
        }
        backup = node;
        node = node->next;
        free(backup);
    }
}


FileBlock** getListAsArray(List* list, int listSize)
{
    FileBlock** listElements = (FileBlock**) malloc((listSize + 1) * sizeof(FileBlock*));

    struct ListNode* node = list;
    int i = 0;
    while(node != NULL)
    {
        if (node->fileBlock != NULL)
        {
            listElements[i] = node->fileBlock;
            i++;
        }
        node = node->next;
    }
    return listElements;
}

void printListDetailed(List *list)
{
    List* node = list;
    while(node != NULL)
    {
        if (node->fileBlock != NULL)
        {
            printf("\n\n%s:", node->fileBlock->fileName);
            printf("\nnext: ");
            if (node->next == NULL)
            {
                printf("NULL");
            }
            else
            {
                printf("%s", node->next->fileBlock->fileName);
            }
        }
        else
        {
            printf("\nNULL");
        }
        node = node->next;
    }
}
