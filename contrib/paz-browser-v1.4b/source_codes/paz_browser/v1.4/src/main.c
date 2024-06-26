#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "../include/paz_browser.h"

int mainMenu();
char* searchMenu();
char* startingPathMenu();

int main()
{
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long t;
    void *status;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    remove("patcher_resources\\NULL");
    remove("patcher_resources\\NULL2");

    FileBlock* fileBlocks = NULL;
    MetaFileInfo* metaFileInfo = NULL;

    int i = 0;

    if (getWindowsVersion() == 10)
    {
        isWindows10 = 1;
    }
    else
    {
        isWindows10 = 0;
    }

    resizeWindow(120,30);

    if (!backupExists())
    {
        createBackup();
    }

    printf("\nOPENED META FILE:");
    metaFileInfo = getMetaFileInfo(getLatestBackup());
    fileBlocks = fillFileBlocks(metaFileInfo,getLatestBackup(),1);

    long* previousBackupHashes = NULL;
    long previousBackupHashesCount = 0;

    long backupCount = 0;
    char** backups = getBackupList(&backupCount);

    if (backupCount >= 2)
    {
        printf("\nGetting backup hashes...");
        previousBackupHashes = getFileBlocksHashes(backups[backupCount-2],&previousBackupHashesCount);
        printf("\nDone\n");
    }

    int mainMenuChosenOption = -1;

    while(mainMenuChosenOption != EXIT)
    {
        mainMenuChosenOption = mainMenu();
        switch (mainMenuChosenOption)
        {
            case (1):
            {
                char* startingPath = startingPathMenu();
                while (strcmp(startingPath,"BACK") != 0)
                {
                    int fileExplorerReturn = folderMenu(startingPath,metaFileInfo->folderNames,metaFileInfo->folderNamesCount,fileBlocks, metaFileInfo,previousBackupHashes, previousBackupHashesCount);
                    if (fileExplorerReturn == EXIT)
                    {
                        startingPath = startingPathMenu();
                    }
                }
            } break;
            case (2):
            {
                char* searchQuery = "";
                while (strcmpi(searchQuery,"/BACK") != 0)
                {
                    searchQuery = searchMenu();

                    if (strcmpi(searchQuery,"/BACK") != 0)
                    {
                        searchResults(searchQuery, fileBlocks, metaFileInfo);
                        free(searchQuery);
                    }
                    else
                    {
                        free(searchQuery);
                        break;
                    }
                }
            } break;
            case (3):
            {
                char* previousExtractLocation = NULL;
                char* extractionLocation = askExtractLocation(&previousExtractLocation);

                if (extractionLocation != NULL)
                {
                    extractAllGameFiles(extractionLocation,metaFileInfo->fileBlocksCount,thread,&attr);
                    /* Free attribute and wait for the other threads */
                    pthread_attr_destroy(&attr);
                    for (t = 0; t < NUM_THREADS; t++)
                    {
                        rc = pthread_join(thread[t], &status);
                        if (rc)
                        {
                            printf("\n\nERROR; return code from pthread_join() is %d\n", rc);
                            PAUSE();
                        }
                    }
                    PAUSE();
                }
            }
            default:;
        }
    }

    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        free(fileBlocks[i].fileName);
        free(fileBlocks[i].folderName);
    }
    free(fileBlocks);
    free(metaFileInfo);

    if (previousBackupHashes != NULL)
    {
        free(previousBackupHashes);
    }

    //pthread_exit(NULL);
    return 0;
}


int mainMenu()
{
    char input = '\0';
    int nOptions = 3;
    int optionSelected = -1;

     // Display the menu to select the region
    while(1) // This condition makes the menu repeat itself until a valid input is entered
    {
        system("cls"); // clears the screen
        printMainHeader();
        printf("What do you want to do?\n");
        printf("1 - Browse Files\n");
        printf("2 - Search for a file\n");
        printf("3 - Extract All Game Files\n");
        printf("0 - Exit\n");

        printf("\nEnter your choice: ");
        fflush(stdin);
        scanf("%c",&input);
        fflush(stdin);

        if (input == 10 || input == ENTER) // ENTER
        {
            return 1;
        }

        // Converts the read string to int
        optionSelected = input - '0';

        // This variable also tells the program later which one of the "moddedBytes" the program should use when replacing the bytes

        if (optionSelected == 0)
        {
            return 0;
        }


         // If the user entered an invalid option, displays a error message
        if(optionSelected < 1 || optionSelected > nOptions)
        {
            printf("\nInvalid option! %c(%d)\n\n", input, input);
            Sleep(500);
        }
        else
        {
            break;
        }
    }
    return optionSelected;
}

char* startingPathMenu()
{
    char input = '\0';
    int cursor = 0;

    while(1)
    {
        system("cls");
        printMainHeader();
        printf("\nWhere do you want to start browsing from?\n\n");

        if (cursor == 0) {printf(" --> ");}else {printf("     ");}
        printf("Root Folder\n");

        if (cursor == 1) {printf(" --> ");}else {printf("     ");}
        printf("Characters Folder\n");

        printf("\nARROWS - Change Selected Option");
        printf("\nENTER  - Confirm");
        printf("\nESC  - Go Back\n");
        input = getch();

        if (input == ARROW_DOWN)
        {
            if (cursor + 1 <= 1)
            {
                cursor++;
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
            }
            else
            {
                cursor = 1;
            }
        }
        else if (input == ENTER)
        {
            if (cursor == 1)
            {
                return "character/model/1_pc/";
            }
            else
            {
                return "";
            }
        }
         else if (input == ESC)
         {
             return "BACK";
         }
    }
    return "BACK";
}

char* searchMenu()
{
    char* input = (char*) malloc(1024);
    while(strcmpi(input,"BACK") != 0)
    {
        system("cls"); // clears the screen
        printMainHeader();
        printf("Type the files you want to search\n");

        printf("\n");
        printf("For multiple files, separate them by commas.\n");
        printf("    phw_00_ub_0004.pac,multiplemodeldesc.xml,pew_00_lb_0053_dec.dds\n");

        printf("\n");
        printf("If you want to list all the files from from an extension, do it like this:\n");
        printf("    *.xml,*dds,*pac - Will give you all the files with the extension .xml or .dds or .pac\n");

        printf("\n");
        printf("If you want to list only the files that are in a specific folder, do this:\n");
        printf("     character/*.xml");
        printf("  This will give you all the files .xml located in the folder \"character/\"\n");

        printf("\n");
        printf("You can search for parts of the file name, doing this:\n");
        printf("     *_00_ub_0001.pac");
        printf("  This will give you all the files that ends with \"_00_ub_0001.pac\"\n");

        printf("\n");
        printf("You can also do this to find all the armor parts:\n");
        printf("     phw_00_*_0001.pac");
        printf("  This will give you: phw_00_ub_0001.pac,phw_00_lb_0001.pac,phw_00_hand_0001.pac,...\n");
        printf("     (Basically the character * makes the program ignore what's between the words it separates)\n");
        printf("\n");
        printf("Type: /BACK to go back to the main menu\n");
        printf("\nSearch: ");
        fflush(stdin);
        fgets(input,1024,stdin);
        fflush(stdin);

        if (input[strlen(input ) - 1] == '\n')
        {
            input[strlen(input ) - 1] = '\0';
            return input;
        }
    }

    return input;
}
