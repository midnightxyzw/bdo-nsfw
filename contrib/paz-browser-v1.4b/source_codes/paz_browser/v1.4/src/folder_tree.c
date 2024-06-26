#include "../include/folder_tree.h"



void insertFolder(char* folderNameToInsert, struct Folder **node)
{
    if((*node) == NULL )
    {
        (*node) = (struct Folder*) malloc(sizeof(struct Folder));

        (*node)->folderName = (char*) malloc(strlen(folderNameToInsert));
        strcpy((*node)->folderName,folderNameToInsert);

        (*node)->next = NULL;
        (*node)->prev = NULL;
        (*node)->subfolder = NULL;
    }
    else if ((*node)->next == NULL)
    {
        (*node)->next = (struct Folder*) malloc(sizeof(struct Folder));

        (*node)->next->folderName = (char*) malloc(strlen(folderNameToInsert) + 1);
        strcpy((*node)->next->folderName,folderNameToInsert);

        (*node)->next->next = NULL;
        (*node)->next->prev = (*node);
        (*node)->next->subfolder = NULL;
    }
    else if ((*node)->next != NULL)
    {
        insertFolder(folderNameToInsert, &(*node)->next);
    }
}

void insertSubFolder(char* folderNameToInsert, struct Folder **node)
{
    //printf("\n------------");
   // printf("\nInsert %s in %s:\n\n", folderNameToInsert, (*node)->folderName);
    if ((*node)->subfolder == NULL)
    {
        (*node)->subfolder = (struct Folder*) malloc (sizeof(struct Folder));

        (*node)->subfolder->folderName = (char*) malloc(strlen(folderNameToInsert) + 1);
        strcpy((*node)->subfolder->folderName,folderNameToInsert);

        (*node)->subfolder->subfolder = NULL;
        (*node)->subfolder->next = NULL;
        (*node)->subfolder->prev = (*node);
       // printf("First insertion ok\n");
    }
    else if ((*node)->subfolder != NULL)
    {

        struct Folder* subfolderEvaluated = (*node)->subfolder;

        while(subfolderEvaluated->next != NULL)
        {
           // printf("\n%s subfolder %s->next != NULL", (*node)->folderName,subfolderEvaluated->folderName);
            subfolderEvaluated = subfolderEvaluated->next;
        }
        struct Folder** nodeToInsert = &subfolderEvaluated->next;

       // printf("\n%s subfolder %s->next is NULL", (*node)->folderName,subfolderEvaluated->folderName);

        (*nodeToInsert) = (struct Folder*) malloc (sizeof(struct Folder));

        (*nodeToInsert)->folderName = (char*) malloc(strlen(folderNameToInsert) + 1);
        strcpy((*nodeToInsert)->folderName,folderNameToInsert);

        (*nodeToInsert)->subfolder = NULL;
        (*nodeToInsert)->next = NULL;
        (*nodeToInsert)->prev = (*node);
    }
    /*printf("\ntree:");
     printFolderTree(rootFolder);*/
}

void printFolderTree(struct Folder *rootFolder)
{
    struct Folder* node = rootFolder;
    while(node != NULL)
    {
        if (node->folderName != NULL)
        {
            printf("\n\n%s:", node->folderName);
            if (node->subfolder != NULL)
            {
                printSubFolders(node);
            }
        }
        else
        {
            printf("\nNULL");
        }
        node = node->next;
    }
}


void printSubFolders(struct Folder *folder)
{
    struct Folder* subFolderNode = folder->subfolder;

    printf("\n[");
    while(subFolderNode != NULL)
    {
        if (subFolderNode->folderName != NULL)
        {
            printf("\"%s\"", subFolderNode->folderName);

            if (subFolderNode->subfolder != NULL)
            {
                printSubFolders(subFolderNode);
            }
            if (subFolderNode->next != NULL)
            {
                printf("->");
            }
        }
        else
        {
            printf("NULL");

        }
        subFolderNode = subFolderNode->next;
    }
    printf("]");
}

struct Folder* getFolder(char* folderName, struct Folder* rootFolder)
{
    struct Folder* node = rootFolder;

    if (ECHO2)
    {
        if (rootFolder == NULL)
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
            printf("%s", node->folderName);
        }
    }
    while(node != NULL)
    {
        if (node->folderName != NULL)
        {
            if (ECHO2)
            printf("\ncomparing %s with %s : %d", node->folderName,folderName,strcmpi(node->folderName,folderName));
            if (strcmpi(node->folderName,folderName) == 0)
            {
                return node;
            }
            if (node->subfolder != NULL)
            {
                if (ECHO2)
                printf("\n%s has a subfolder called %s, go to it.\n",node->folderName,node->subfolder->folderName);
                struct Folder* result = getFolder(folderName,node->subfolder);;
                if (result != NULL)
                {
                   return result;
                }
            }
           /* if (node->next != NULL)
            {
                if (ECHO2)
                    printf("\n%s has a next called %s, go to it.\n",node->folderName,node->next->folderName);
               struct Folder* result = getFolder(folderName,node->next);
               if (result != NULL)
               {
                   return result;
               }
            }*/
        }
        else
        {
            printf("\nFolder name null");
        }
        if (ECHO2)
        printf("\n\nnode(%s) = node->next", node->folderName);

        node = node->next;

        if (ECHO2)
        printf("\nDone");

        if (ECHO2)
        {
            if (node == NULL)
            {
                printf("\nNode is NULL");
            }
            else
            {
                printf("\nNode is %s", node->folderName);
            }
         }
    }
    if (ECHO2)
    printf("\n\nExited, not found");

    return NULL;
}


void printFolderTreeDetailed(struct Folder *rootFolder)
{
    struct Folder* node = rootFolder;
    while(node != NULL)
    {
        if (node->folderName != NULL)
        {
            printf("\n\n%s:", node->folderName);
            printf("\nnext: ");
            if (node->next == NULL)
            {
                printf("NULL");
            }
            else
            {
                printf("%s", node->next->folderName);
            }
            printf("\nsubfolder: ");
            if (node->subfolder == NULL)
            {
                printf("NULL");
            }
            else
            {
                printf("%s", node->subfolder->folderName);
            }
            if (node->subfolder != NULL)
            {
                printSubFoldersDetailed(node);
            }

        }
        else
        {
            printf("\nNULL");
        }
        node = node->next;
    }
}


void printSubFoldersDetailed(struct Folder *folder)
{
    struct Folder* subFolderNode = folder->subfolder;

    while(subFolderNode != NULL)
    {
        if (subFolderNode->folderName != NULL)
        {
            printf("\n\n\"%s\"", subFolderNode->folderName);
            printf("\nnext: ");
            if (subFolderNode->next == NULL)
            {
                printf("NULL");
            }
            else
            {
                printf("%s", subFolderNode->next->folderName);
            }
            printf("\nsubfolder: ");
            if (subFolderNode->subfolder == NULL)
            {
                printf("NULL");
            }
            else
            {
                printf("%s", subFolderNode->subfolder->folderName);
            }

            if (subFolderNode->subfolder != NULL)
            {
                printSubFoldersDetailed(subFolderNode);
            }
        }
        else
        {
            printf("NULL");

        }
        subFolderNode = subFolderNode->next;
    }
}

