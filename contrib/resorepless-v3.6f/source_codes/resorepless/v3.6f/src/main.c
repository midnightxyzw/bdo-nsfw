#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "../include/utilities.h"
#include "../include/patcher.h"
#include "../include/list.h"



void checkFiles();

int mainMenu();
void menuCustomize(PatcherState* patcherState);
void menuBreastSize(PatcherState* patcherState);
void menuBreastSize2(PatcherState* patcherState);
void menuMinMaxDefault(PatcherState* patcherState,int BODY_PART);
void menuWeapons(PatcherState* patcherState);
void kibeliusMenu(PatcherState* patcherState);
int toolsMenu();
int chooseToolInputType(int operation, char* operationName);
void manualInputMenu(int operation, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, PatcherState* patcherState);
void filesFromTextureMenu(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);
void menuSearchTextures();
void menuPubicHair(PatcherState* patcherState);
void menuCustomizePenises(PatcherState* patcherState);
void menuSearchFile(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);
int uninstallMenu();
void printStateCustomizeMenu(int* patcherStateOption, int type);
void printStatePubicHairMenu(int* pubicHairOption);
void printPenisState(int penisState);
void printWeaponOptionStatus(char* className, int CLASS_MACRO,char* primaryWeaponName, char* secundaryWeaponName,PatcherState* patcherState, int cursor);
char* getBodyPartName(int BODY_PART_MACRO);
void listPatchedFiles(FileBlock* fileBlocks,MetaFileInfo* metaFileInfo);
void menuClassesToAply(PatcherState* patcherState);
void printMenuOption(char* optionName, int optionNumber, int cursor);
void printCursor(int cursor, int valueToCompare);
void printOnOffSwitch(int variable);
void deleteEverything();
char* searchMenu();
void searchResults(char* searchQuery, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);

int main()
{
    PatcherState* patcherState = loadPatcherState(PATCHER_STATE_FILE_NAME);

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



    checkFiles();




    /*if (!backupExists())
    {
        createBackup();
    }*/

    metaFileInfo = getMetaFileInfo(getLatestBackup());
    fileBlocks = fillFileBlocks(metaFileInfo);

    //filesPatched = countPatchedFiles(fileBlocks,metaFileInfo);

    int mainMenuChosenOption = -1;


    while (1)
    {
         resizeWindow(120,33);

        mainMenuChosenOption = mainMenu();

        if (mainMenuChosenOption == EXIT)
        {
            break;
        }
        else if (mainMenuChosenOption == 1)
        {
            savePatcherState(patcherState,PATCHER_STATE_FILE_NAME);
            runPatcher(fileBlocks, metaFileInfo, patcherState);
        }
        else if(mainMenuChosenOption == CUSTOMIZE)
        {
            menuCustomize(patcherState);
        }
        else if (mainMenuChosenOption == CHANGE_SIZES)
        {
            menuBreastSize2(patcherState);
        }
        else if (mainMenuChosenOption == KIBELIUS)
        {
            kibeliusMenu(patcherState);
        }
        else if (mainMenuChosenOption == TOOLS)
        {
            while (1)
            {
                int operation = toolsMenu();
                char* operationName = getOperationName(operation);

                if (operation == BACK)
                {
                    break;
                }
                if (operation == BROWSE_GAME_FILES)
                {
                     //int browsingStyle = 1/*chooseBrowsingStyle()*/;
                     runPazBrowser(fileBlocks,metaFileInfo,operationName, operation,patcherState);
                }
                else if (operation == GET_FILES_FROM_TEXTURE)
                {
                     filesFromTextureMenu(fileBlocks, metaFileInfo);
                }
                else
                {
                    int inputType = chooseToolInputType(operation, operationName);

                    if (inputType != BACK)
                    {
                        if (inputType == BACK)
                        {
                            break;
                        }

                        if (inputType == BROWSE_FILES)
                        {
                            runPazBrowser(fileBlocks,metaFileInfo,operationName, operation,patcherState);
                        }
                        else if (inputType == MANUAL_INSERT)
                        {
                             manualInputMenu(operation,fileBlocks,metaFileInfo,patcherState);
                        }
                        else if (inputType == PATCH_FROM_FILE)
                        {
                             patchFromFile(operation,fileBlocks,metaFileInfo,patcherState);
                        }
                    }
                }
            }
        }
        else if (mainMenuChosenOption == UNINSTALL)
        {
            int uninstallOption = uninstallMenu();
            if (uninstallOption == 1)
            {
                backupMenu();
            }
            else if (uninstallOption == 2)
            {
                deleteEverything();
                free(patcherState);
                patcherState = loadPatcherState(PATCHER_STATE_FILE_NAME);
            }
            else if (uninstallOption == 3)
            {
                preventFileRecheck(1);
                PAUSE();
            }
        }
        /*else if (mainMenuChosenOption == LIST_PATCHED_FILES)
        {
            char* searchQuery = "";
            while (strcmpi(searchQuery,"/BACK") != 0)
            {
                searchQuery = searchMenu();

                if (searchQuery[0] == '/' && strlen(searchQuery) == 1)
                {
                    break;
                }
                else if (strcmpi(searchQuery,"LIST") == 0)
                {
                    listPatchedFiles(fileBlocks,metaFileInfo);
                }
                else if (strcmpi(searchQuery,"/BACK") != 0)
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
        }*/

    }
    free(patcherState);

    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        free(fileBlocks[i].fileName);
        free(fileBlocks[i].folderName);
    }
    free(fileBlocks);
    free(metaFileInfo);
    return 0;
}


int mainMenu()
{
    char input = '\0';
    int nOptions = 7;
    int cursor = 1;

     // Display the menu to select the region
    while(1) // This condition makes the menu repeat itself until a valid input is entered
    {
        if (input >= 0)
        {
             system("cls"); // clears the screen
            printMainHeader();
            /*printf("Meta file status: ");
            if (filesPatched == 0)
            {
                printColor("Clean", GREEN);
            }
            else
            {
                printf("%ld files currently patched", filesPatched);
            }*/
            printf("\n\nWhat do you want to do?\n");
            printCursor(cursor,1);printf("1 - Install");
            printCursor(cursor,2);printf("2 - Customize");
            printCursor(cursor,3);printf("3 - Change body parts max size");
            printCursor(cursor,4);printf("4 - Kibelius Armor Options");
            printCursor(cursor,5);printf("5 - Tools");
            printCursor(cursor,6);printf("6 - Uninstall Options");
            printf("\n");
            printCursor(cursor,7);printf("0 - Exit\n");

            printf("\n");
            printf("[UP ARROW]   - Move cursor up\n");
            printf("[DOWN ARROW] - Move cursor down\n\n");

            printf("[ENTER] - Confirm\n");
        }

        input = getch();

        if (input >= 0)
        {
            if (input == 10 || input == ENTER) // ENTER
            {
                if (cursor == 7)
                {
                    return EXIT;
                }
                return cursor;
            }
            else if (input == ARROW_DOWN)
            {
                if (cursor + 1 <= nOptions)
                {
                    cursor++;
                }
                else
                {
                    cursor = 1;
                }
            }

            else if (input == ARROW_UP)
            {
                if (cursor - 1 >= 1)
                {
                    cursor--;
                }
                else
                {
                    cursor = nOptions;
                }
            }
            if (input == '0')
            {
                return EXIT;
            }
            if (input >= '0' && input <= '6')
            {
                return input - '0';
            }
        }
    }
    return cursor;
}

void printMenuOption(char* optionName, int optionNumber, int cursor)
{
    if (cursor == optionNumber)
    {
        printf("\n -> ");
    }
    else
    {
        printf("\n    ");
    }
    printf("%s",optionName);
}

void menuCustomize(PatcherState* patcherState)
{

    int cursor = 0;
    int nOptionsMenu = 12;

    char input = '\0';

    while(1)
    {
        if (input >= 0)
        {
            /*int windowHeight = 30;
            if (cursor == 2 && patcherState->option[CENSORSHIP] !=  0)
            {
                windowHeight += 3;
            }
            if (patcherState->option[USE_PENISES] && patcherState->option[GENDER] != FEMALE_ONLY)
            {
                windowHeight += 1;
            }
            if (patcherState->option[USE_3D_VAGINAS] && patcherState->option[GENDER] != MALE_ONLY)
            {
                windowHeight += 1;
            }*/
            //resizeWindow(120,windowHeight);

            system("cls");
            printMainHeader();
            printf("\n");
            printf("[F1] - Pubic Hair Options [F3] - Customize Penises \n");
            printf("[F2] - Customize Weapons  [F4] - Choose classes the mod applies\n");

            //typedef enum {ARMOR,UNDERWEAR,LOWERBODY,GLOVES,BOOTS,HELMETS,STOCKINGS,LIFE_SKILL_ARMORS,GENDER,NPC,USE_3D_VAGINAS,USE_PENISES} PatcherOption;

            printCursor(cursor,ARMOR);              printf("Armor              : "); printStateCustomizeMenu(&patcherState->option[ARMOR],ARMOR);
            printCursor(cursor,UNDERWEAR);          printf("Underwear          : "); printStateCustomizeMenu(&patcherState->option[UNDERWEAR],UNDERWEAR);
            printCursor(cursor,LOWERBODY);          printf("Censorship Removal : "); printStateCustomizeMenu(&patcherState->option[LOWERBODY],LOWERBODY);
            printCursor(cursor,GLOVES);             printf("Gloves             : "); printStateCustomizeMenu(&patcherState->option[GLOVES],GLOVES);
            printCursor(cursor,BOOTS);              printf("Boots              : "); printStateCustomizeMenu(&patcherState->option[BOOTS],BOOTS);
            printCursor(cursor,HELMETS);            printf("Helmets            : "); printStateCustomizeMenu(&patcherState->option[HELMETS],HELMETS);
            printCursor(cursor,STOCKINGS);          printf("Stockings          : "); printStateCustomizeMenu(&patcherState->option[STOCKINGS],STOCKINGS);
            printCursor(cursor,LIFE_SKILL_ARMORS);  printf("Life Skill Armors  : "); printStateCustomizeMenu(&patcherState->option[LIFE_SKILL_ARMORS],LIFE_SKILL_ARMORS);
            printCursor(cursor,GENDER);             printf("Gender             : "); printStateCustomizeMenu(&patcherState->option[GENDER],GENDER);
           // printCursor(cursor,NPC);                printf("NPC                : "); printStateCustomizeMenu(&patcherState->option[NPC],NPC);
            printf("\n");
            printCursor(cursor,USE_3D_VAGINAS);     printf("Use 3D vaginas : "); printStateCustomizeMenu(&patcherState->option[USE_3D_VAGINAS],USE_3D_VAGINAS);
            printCursor(cursor,USE_PENISES);        printf("Penises        : "); printStateCustomizeMenu(&patcherState->option[USE_PENISES],USE_PENISES);
            printf("\n\n");


            if (patcherState->option[USE_3D_VAGINAS] && patcherState->option[GENDER] != MALE_ONLY)
            {
                 printColor("Warning: ", YELLOW);
                printf("Using 3D vaginas can cause some body issues.\n");
                printf("         If you are having trouble with body textures or deformations, turn this option off\n\n");
            }
            if (patcherState->option[USE_PENISES] && patcherState->option[GENDER] != FEMALE_ONLY)
            {
                 printColor("Warning: ", YELLOW);
                printf("The penis mod is restricted to the default skin color.\n");
                printf("         If your character has a different skin color, the penis skin color won't match\n\n");
            }

            if (cursor == 2)
            {
                if (patcherState->option[CENSORSHIP] !=  0)
                {
                    printColor("Censorship Removal Description:\n", YELLOW);
                }

                if (patcherState->option[CENSORSHIP] == MINIMAL_CENSORSHIP_REMOVAL)
                {
                    printf("Low - Removes the panties from some armors that have built-in\n");
                    printf("      panties such as most of Tamer's and Ranger's armors");
                    printf("\n\n");
                }
                if (patcherState->option[CENSORSHIP] == MEDIUM_CENSORSHIP_REMOVAL)
                {
                    printf("Medium - Also removes the upper body covers underneath the armors\n");
                    printf("         such as the Dark Knight and Maehwa costumes");
                    printf("\n\n");
                }
                if (patcherState->option[CENSORSHIP] == HIGH_CENSORSHIP_REMOVAL)
                {
                    printf("High - Also removes the pants from most of the Witch armors\n");
                    printf("       and some of the Sorceress armors as well");
                    printf("\n\n");
                }
            }

            printf("[UP ARROW]  : Move cursor up       [RIGHT ARROW]: Increase value\n");
            printf("[DOWN ARROW]: Move cursor down     [LEFT ARROW] : Decrease value\n\n");


            printf("[ENTER] - Confirm current settings   [ESC] - Back to main menu");
        }

        input = getch();

        if (input >= 0)
        {
            if (input == ESC)
            {
                break;
            }
            else if (input == ENTER || input == 10)
            {
                savePatcherState(patcherState,PATCHER_STATE_FILE_NAME);
                break;
            }
            else if (input == ARROW_DOWN)
            {
                if (cursor + 1 < nOptionsMenu)
                {
                    cursor++;
                    if (cursor == 9 /* NPC */)
                    {
                        cursor++;
                    }
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
                    if (cursor == 9 /* NPC */)
                    {
                        cursor--;
                    }
                }
                else
                {
                    cursor = nOptionsMenu - 1;
                }
            }
            else if (input == ARROW_RIGHT)
            {
                patcherState->option[cursor]++;
            }
            else if (input == ARROW_LEFT)
            {
                patcherState->option[cursor]--;
            }

            else if (input == F1)
            {
                menuPubicHair(patcherState);
            }
            else if (input == F2)
            {
                menuWeapons(patcherState);
            }
            else if (input == F3)
            {
                menuCustomizePenises(patcherState);
            }
            else if (input == F4)
            {
                menuClassesToAply(patcherState);
            }
        }
    }
//    3(120,30);
}

void menuPubicHair(PatcherState* patcherState)
{
    int cursor = 0;
    int nOptionsMenu = 5;
    char input = '\0';
    int optionSelected = -1;

    int totalPubicHairOptions = 13;


    while(1)
    {
         system("cls");
        printMainHeader();
        printf("Press the corresponding numbers to change the current settings, or use the arrows:\n");
        printCursor(cursor,RANGER_PUBIC_HAIR);      printf("Ranger      : "); printStatePubicHairMenu(&patcherState->pubicHair[RANGER_PUBIC_HAIR]);
        printCursor(cursor,WITCH_PUBIC_HAIR);       printf("Witch       : "); printStatePubicHairMenu(&patcherState->pubicHair[WITCH_PUBIC_HAIR]);
        printCursor(cursor,TAMER_PUBIC_HAIR);       printf("Tamer       : "); printStatePubicHairMenu(&patcherState->pubicHair[TAMER_PUBIC_HAIR]);
        printCursor(cursor,DARK_KNIGHT_PUBIC_HAIR); printf("Dark Knight : "); printStatePubicHairMenu(&patcherState->pubicHair[DARK_KNIGHT_PUBIC_HAIR]);
        printCursor(cursor,SORCERESS_PUBIC_HAIR);   printf("All Others  : "); printStatePubicHairMenu(&patcherState->pubicHair[SORCERESS_PUBIC_HAIR]);
        /*printCursor(cursor,VALKYRIE_PUBIC_HAIR);    printf("Valkyrie Texture    : "); printStatePubicHairMenu(&patcherState->pubicHair[VALKYRIE_PUBIC_HAIR]);
        printCursor(cursor,MAEHWA_PUBIC_HAIR);      printf("Maehwa Texture      : "); printStatePubicHairMenu(&patcherState->pubicHair[MAEHWA_PUBIC_HAIR]);
        printCursor(cursor,KUNOICHI_PUBIC_HAIR);    printf("Kunoichi Texture    : "); printStatePubicHairMenu(&patcherState->pubicHair[KUNOICHI_PUBIC_HAIR]);
        printCursor(cursor,MYSTIC_PUBIC_HAIR);      printf("Mystic Texture      : "); printStatePubicHairMenu(&patcherState->pubicHair[MYSTIC_PUBIC_HAIR]);*/

        printf("\n\n   [L] - Open Preview Picture\n\n");

        printf("[UP ARROW]  : Move cursor up       [RIGHT ARROW]: Increase value\n");
        printf("[DOWN ARROW]: Move cursor down     [LEFT ARROW] : Decrease value\n\n");

        printf("[ENTER] - Confirm\n");
        printf("[ESC]   - Go Back\n");
        input = getch();

        optionSelected = (int)input - 48;


         if (input == 'L' || input == 'l')
         {
             system("patcher_resources\\texture\\pubic_hair\\preview.jpg");
         }

        if (input == ENTER|| input == 10 || input == ESC)
        {
            break;
        }
        else if (input == ARROW_DOWN)
        {
            if (cursor + 1 < nOptionsMenu)
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
                cursor = nOptionsMenu - 1;
            }
        }
        else if (input == ARROW_RIGHT)
        {
            if (patcherState->pubicHair[cursor] + 1 < totalPubicHairOptions)
            {
                patcherState->pubicHair[cursor]++;
            }
            else
            {
                patcherState->pubicHair[cursor] = 0;
            }

        }
        else if (input == ARROW_LEFT)
        {
             if ( patcherState->pubicHair[cursor] - 1 >= 0)
            {
                patcherState->pubicHair[cursor]--;
            }
            else
            {
                patcherState->pubicHair[cursor] = totalPubicHairOptions - 1;
            }
        }
        else if(optionSelected >= 1 && optionSelected <= nOptionsMenu)
        {
            patcherState->pubicHair[(optionSelected-1)]++;
        }
    }
}

void menuCustomizePenises(PatcherState* patcherState)
{
    int cursor = 0;
    int nOptionsMenu = 6;
    char input = '\0';

    while(1)
    {
        if (input >= 0)
        {
            system("cls");
            printMainHeader();
            printf("Press the corresponding numbers to change the current settings, or use the arrows:\n");
            printCursor(cursor,WARRIOR_PENIS);   printf("Warrior Penis       : "); printPenisState(patcherState->penisType[WARRIOR_PENIS]);
            printCursor(cursor,BERSERKER_PENIS); printf("Berserker Penis     : "); printPenisState(patcherState->penisType[BERSERKER_PENIS]);
            printCursor(cursor,BLADER_PENIS);    printf("Blader (Musa) Penis : "); printPenisState(patcherState->penisType[BLADER_PENIS]);
            printCursor(cursor,WIZARD_PENIS);    printf("Wizard Penis        : "); printPenisState(patcherState->penisType[WIZARD_PENIS]);
            printCursor(cursor,NINJA_PENIS);     printf("Ninja Penis         : "); printPenisState(patcherState->penisType[NINJA_PENIS]);
            printCursor(cursor,STRIKER_PENIS);   printf("Striker Penis       : "); printPenisState(patcherState->penisType[STRIKER_PENIS]);

            printf("\n\n   [L] - Open Preview Picture");
            printf("\n\n");
            printf("[UP ARROW]  : Move cursor up       [RIGHT ARROW]: Increase value\n");
            printf("[DOWN ARROW]: Move cursor down     [LEFT ARROW] : Decrease value\n\n");

            printf("[ENTER] - Confirm\n");
            printf("[ESC]   - Go Back\n");
        }

        input = getch();

        if (input >= 0)
        {
             if (input == 'L' || input == 'l')
             {
                 system("patcher_resources\\texture\\pubic_hair\\preview_male.jpg");
             }

            if (input == ENTER|| input == 10 || input == ESC)
            {
                break;
            }
            else if (input == ARROW_DOWN)
            {
                if (cursor + 1 < nOptionsMenu)
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
                    cursor = nOptionsMenu - 1;
                }
            }
            else if (input == ARROW_RIGHT)
            {
                if (patcherState->penisType[cursor] + 1 <= HARD)
                {
                    patcherState->penisType[cursor]++;
                }
                else
                {
                    patcherState->penisType[cursor] = NONE;
                }

            }
            else if (input == ARROW_LEFT)
            {
                 if ( patcherState->penisType[cursor] - 1 >= 0)
                {
                    patcherState->penisType[cursor]--;
                }
                else
                {
                    patcherState->penisType[cursor] = HARD;
                }
            }
        }
    }
}

void menuBreastSize2(PatcherState* patcherState)
{
      char input = '\0';
    int cursor = 0;
    while (1)
    {
        if (input >= 0)
        {
            system("cls");
            printMainHeader();
            printf("Select the body part you want to change the size values:\n");
            printf("--------------------------------------------------------");
            printCursor(cursor,0);printf("Breast ");
            printCursor(cursor,1);printf("Butt   ");
            printCursor(cursor,2);printf("Thighs ");
            printCursor(cursor,3);printf("Arms  ");
            printCursor(cursor,4);printf("Legs   ");
            printCursor(cursor,5);printf("Front Hips and Pelvis ");
            printCursor(cursor,6);printf("Lower Back and Belly  ");

            printf("\n\n");
            printf("[UP ARROW]  : Move cursor up\n");
            printf("[DOWN ARROW]: Move cursor down\n\n");
            printf("    [R] - Reset all values from all body parts\n");
            printf("[ENTER] - Enter change values menu for this body part\n");
            printf("  [ESC] - Save and go back\n\n");
        }
        input = getch();
        if (input >= 0)
        {
            if (input == ENTER || input == 10)
            {
                menuMinMaxDefault(patcherState, cursor);
            }
            else if (input == ESC)
            {
                savePatcherState(patcherState,PATCHER_STATE_FILE_NAME);
                break;
            }
            else if (input == ARROW_DOWN)
            {
                if (cursor + 1 <= BODY_SIZE_N_OPTIONS - 1)
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
                if (cursor -1 >= 0)
                {
                    cursor--;
                }
                else
                {
                    cursor = BODY_SIZE_N_OPTIONS - 1;
                }
            }
            else if (input == 'R' || input == 'r')
            {
                int i = 0;
                for (i = 0; i < BODY_SIZE_N_OPTIONS; i++)
                {
                    patcherState->bodyPartSize[i].min_size      = MIN_DEFAULT_SIZE;
                    patcherState->bodyPartSize[i].default_size  = DEFAULT_DEFAULT_SIZE;
                    patcherState->bodyPartSize[i].max_size      = MAX_DEFAULT_SIZE;
                }
                printf("\n All values Restored!");
                Sleep(1500);
            }
        }
    }
}

void menuMinMaxDefault(PatcherState* patcherState,int BODY_PART)
{
    int cursor = 0;
    char input = '\0';
    char* bodyPartName = getBodyPartName(BODY_PART);

    while(1)
    {
        system("cls");
        printMainHeader();
        printf("Use the arrows keys to change the values of the %s size\n(Max Recommended: 2.5, Min Recommended: 0.7)\n", bodyPartName);
        printCursor(cursor,0);printf(" Min size    : %.2f ", patcherState->bodyPartSize[BODY_PART].min_size);
        printCursor(cursor,1);printf(" Default size: %.2f ", patcherState->bodyPartSize[BODY_PART].default_size);
        printCursor(cursor,2);printf(" Max size    : %.2f ", patcherState->bodyPartSize[BODY_PART].max_size);

        printf("\n\n");
        printColor(" Rule: ", YELLOW);
        printf("Min size < Default size + 0.10 < Max size\n\n");
        printColor(" Warning:", RED);
        printf(" - You won't see any changes unless you create a new character or edit your current one.\n");
        if (BODY_PART == BREASTS)
        {
            printf("          - Breast size change doesn't work for Tamer.");
        }
        printf("\n\n");
        printf("[UP ARROW]  : Move cursor up       [RIGHT ARROW]: Increase value\n");
        printf("[DOWN ARROW]: Move cursor down     [LEFT ARROW] : Decrease value\n\n");

        printf("    [R] - Reset values of this body part\n");
        printf("[ENTER] - Confirm\n");
        printf("  [ESC] - Go Back\n\n");
        input = getch();




        if (input == ENTER || input == 10)
        {
            savePatcherState(patcherState,PATCHER_STATE_FILE_NAME);
            break;
        }
        else if (input == ESC)
        {
            break;
        }
        else if (input == ARROW_DOWN)
        {
            if (cursor + 1 < 3)
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
                cursor = 2;
            }
        }
        else if (input == ARROW_RIGHT)
        {
            if (cursor == 0)
            {


                if (patcherState->bodyPartSize[BODY_PART].min_size <= -0.95)
                {
                    patcherState->bodyPartSize[BODY_PART].min_size = -1.0;
                }

                 patcherState->bodyPartSize[BODY_PART].min_size += 0.05;

                if (patcherState->bodyPartSize[BODY_PART].min_size> (patcherState->bodyPartSize[BODY_PART].default_size - 0.10))
                {
                    patcherState->bodyPartSize[BODY_PART].min_size = patcherState->bodyPartSize[BODY_PART].default_size - 0.10;
                }
            }
            if (cursor == 1)
            {
                patcherState->bodyPartSize[BODY_PART].default_size += 0.05;
                if (patcherState->bodyPartSize[BODY_PART].default_size >(patcherState->bodyPartSize[BODY_PART].max_size - 0.10))
                {
                    patcherState->bodyPartSize[BODY_PART].default_size = patcherState->bodyPartSize[BODY_PART].max_size - 0.10;
                }
            }
            if (cursor == 2)
            {
                patcherState->bodyPartSize[BODY_PART].max_size += 0.05;

                if (patcherState->bodyPartSize[BODY_PART].max_size >= 5.00)
                {
                    patcherState->bodyPartSize[BODY_PART].max_size = 5.00;
                }
            }
        }
        else if (input == ARROW_LEFT)
        {
            if (cursor == 0)
            {
                patcherState->bodyPartSize[BODY_PART].min_size -= 0.05;


                if (patcherState->bodyPartSize[BODY_PART].min_size <= -1.00)
                {
                    patcherState->bodyPartSize[BODY_PART].min_size = -1.00;
                }


               /* if (patcherState->bodyPartSize[BODY_PART].min_size < 0)
                {
                   patcherState->bodyPartSize[BODY_PART].min_size = 0.00;
                }*/
            }
            if (cursor == 1)
            {

                patcherState->bodyPartSize[BODY_PART].default_size -= 0.05;

                if(patcherState->bodyPartSize[BODY_PART].default_size < (patcherState->bodyPartSize[BODY_PART].min_size + 0.10))
                {
                   patcherState->bodyPartSize[BODY_PART].default_size = patcherState->bodyPartSize[BODY_PART].min_size + 0.10;
                }
            }
            if (cursor == 2)
            {
                patcherState->bodyPartSize[BODY_PART].max_size -= 0.05;
                if (patcherState->bodyPartSize[BODY_PART].max_size < (patcherState->bodyPartSize[BODY_PART].default_size + 0.10))
                {
                    patcherState->bodyPartSize[BODY_PART].max_size = patcherState->bodyPartSize[BODY_PART].default_size + 0.10;
                }
            }


        }
        else if (input == 'R' || input == 'r')
        {
            patcherState->bodyPartSize[BODY_PART].min_size      = MIN_DEFAULT_SIZE;
            patcherState->bodyPartSize[BODY_PART].default_size  = DEFAULT_DEFAULT_SIZE;
            patcherState->bodyPartSize[BODY_PART].max_size      = MAX_DEFAULT_SIZE;
        }

        // Float imprecision fix
        if (patcherState->bodyPartSize[BODY_PART].min_size < 0.05 && patcherState->bodyPartSize[BODY_PART].min_size > -0.05)
        {
            patcherState->bodyPartSize[BODY_PART].min_size = 0.00;
        }
        if (patcherState->bodyPartSize[BODY_PART].default_size < 0.05 && patcherState->bodyPartSize[BODY_PART].default_size > -0.05)
        {
            patcherState->bodyPartSize[BODY_PART].default_size = 0.00;
        }
        if (patcherState->bodyPartSize[BODY_PART].max_size < 0.05 && patcherState->bodyPartSize[BODY_PART].max_size > -0.05)
        {
            patcherState->bodyPartSize[BODY_PART].max_size = 0.00;
        }
    }
}

char* getBodyPartName(int BODY_PART)
{
    if (BODY_PART == BREASTS)
    {
       return "Breasts";
    }
    else if (BODY_PART == BUTT)
    {
        return "Butt";
    }
    else if (BODY_PART == THIGHS)
    {
        return "Thighs";
    }
    else if (BODY_PART == LEGS)
    {
        return "Legs";
    }
    else if (BODY_PART == PELVIS)
    {
        return "Hips and Pelvis";
    }
    else if (BODY_PART == SPINE)
    {
        return "Lower Back and Belly";
    }
    else if (BODY_PART == ARMS)
    {
        return "Arms";
    }
   /* else if (BODY_PART == HAND)
    {
        return "Hands";
    }
    else if (BODY_PART == FOOT)
    {
        return "Foot";
    }
     else if (BODY_PART == NECK)
    {
        return "Neck";
    }
     else if (BODY_PART == CLAVICLE)
    {
        return "Clavicle";
    }*/
    return "";
}


void kibeliusMenu(PatcherState* patcherState)
{
    char input = '\0';
    int nOptions = 4;
    int cursor = patcherState->transparentKibelius;
    while (1)
    {
        system("cls");
        printMainHeader();
        printf(" Kibelius Armor:\n");
        printCursor(cursor,0);printf("Normal           ");
        printCursor(cursor,1);printf("Transparent      ");
        printCursor(cursor,2);printf("More Transparent ");
        printCursor(cursor,3);printf("Naked with wings ");
        printf("\n\n\tKeep Wings       "); printOnOffSwitch(patcherState->kibeliusWings);

        printf("\n\n");
        printf("  [SPACE]: Toggle \"Keep Wings\"\n\n");

        printf("[UP ARROW]   - Move cursor up\n");
        printf("[DOWN ARROW] - Move cursor down\n\n");

        printf("[ENTER] - Confirm [ESC] - Back to main menu\n");
        input = getch();

        if (input == ENTER || input == 10)
        {
            patcherState->transparentKibelius = cursor;
            savePatcherState(patcherState,PATCHER_STATE_FILE_NAME);
            break;
        }
        if (input == ESC)
        {
            break;
        }
        if (input == ARROW_DOWN)
        {
            if (cursor + 1 < nOptions)
            {
                cursor++;
            }
            else
            {
                cursor = 0;
            }
        }
        if (input == ARROW_UP)
        {
            if (cursor -1 >= 0)
            {
                cursor--;
            }
            else
            {
                cursor = nOptions - 1;
            }
        }
        if (input == SPACE || (cursor == 4 && (input == ARROW_DOWN || input == ARROW_RIGHT)))
        {
            patcherState->kibeliusWings = !patcherState->kibeliusWings;
        }
    }
}


int uninstallMenu()
{
    char input = '\0';
    int nOptions = 3;
    int cursor = 1;

     // Display the menu to select the region
    while(1) // This condition makes the menu repeat itself until a valid input is entered
    {
        if (input >= 0)
        {
            system("cls"); // clears the screen
            printMainHeader();
            printf("Which type of files do you want to browse?\n");
            printCursor(cursor,1);printf("Restore backup");
            printCursor(cursor,2); printf("Reset and Uninstall Everything");
            printCursor(cursor,3); printf("Prevent file re-check");

            printf("\n\n");
            printf("[UP ARROW]   - Move cursor up\n");
            printf("[DOWN ARROW] - Move cursor down\n\n");

            printf("[ENTER] - Confirm [ESC] - Exit\n");
        }

        input = getch();

        if (input >= 0)
        {
            if (input == 10 || input == ENTER) // ENTER
            {
                return cursor;
            }
            else if (input == ARROW_DOWN)
            {
                if (cursor + 1 <= nOptions)
                {
                    cursor++;
                }
                else
                {
                    cursor = 1;
                }
            }

            else if (input == ARROW_UP)
            {
                if (cursor - 1 >= 1)
                {
                    cursor--;
                }
                else
                {
                    cursor = nOptions;
                }
            }
            if (input == ESC)
            {
                return BACK;
            }
        }
    }
    return BACK;
}


int toolsMenu()
{

    char input = '\0';
    int nOptions = 5;
    int cursor = 1;

     // Display the menu to select the region
    while(1) // This condition makes the menu repeat itself until a valid input is entered
    {
        if (input >= 0)
        {
             system("cls"); // clears the screen
            printMainHeader();
            printf("Meta file status: ");
            if (filesPatched == 0)
            {
                printColor("Clean", GREEN);
            }
            else
            {
                printf("%ld files currently patched", filesPatched);
            }
            printf("\n\nWhat do you want to do?\n");
            printCursor(cursor,1);printf("1 - Browse Game Files");
            printCursor(cursor,2);printf("2 - Remove Armor");
            /*printCursor(cursor,3);printf("3 - Restore Armor");*/
            printCursor(cursor,3);printf("3 - Fix holes on character");
            printCursor(cursor,4);printf("4 - Get Textures from a file");
            printCursor(cursor,5);printf("5 - Get all files that uses a specific texture");

            printf("\n\n");
            printf("[UP ARROW]   - Move cursor up\n");
            printf("[DOWN ARROW] - Move cursor down\n\n");

            printf("[ENTER] - Confirm [ESC] - Exit\n");
        }

        input = getch();

        if (input >= 0)
        {
            if (input == 10 || input == ENTER) // ENTER
            {
                if (cursor == 10)
                {
                    return EXIT;
                }
                return cursor;
            }
            else if (input == ARROW_DOWN)
            {
                if (cursor + 1 <= nOptions)
                {

                        cursor++;


                }
                else
                {
                    cursor = 1;
                }
            }

            else if (input == ARROW_UP)
            {
                if (cursor - 1 >= 1)
                {

                        cursor--;


                }
                else
                {
                    cursor = nOptions;
                }
            }
            if (input == ESC || input == '0')
            {
                return EXIT;
            }
            if (input >= '0' && input <= '9')
            {
                if (input == '1') {
                    return input - '0';
                } else {
                    return input - '0' -2 ;
                }
            }
        }
    }
    return cursor;
}



int chooseToolInputType(int operation, char* operationName)
{

    char input = '\0';
    int nOptions = 2;
    if (operation != GET_TEXTURES)
    {
        nOptions = 3;
    }
    int cursor = 1;

     // Display the menu to select the region
    while(1) // This condition makes the menu repeat itself until a valid input is entered
    {
        if (input >= 0)
        {
            system("cls"); // clears the screen
            printMainHeader();
            printf("How do you want to inform the file names to %s?\n", operationName);
            printCursor(cursor,1);printf("Browse files");
            printCursor(cursor,2);printf("Manual Inform file names");
            if (operation != GET_TEXTURES)
            {
                printCursor(cursor,3);printf("%s files listed in \"%s\"", captalizeFirstLetter(operationName), PAZ_BROWSER_PATCHER_STATE_FILE_NAME[operation]);
            }


            printf("\n\n");
            printf("[UP ARROW]   - Move cursor up\n");
            printf("[DOWN ARROW] - Move cursor down\n\n");

            printf("[ENTER] - Confirm [ESC] - Exit\n");
        }

        input = getch();

        if (input >= 0)
        {
            if (input == 10 || input == ENTER) // ENTER
            {
                return cursor;
            }
            else if (input == ARROW_DOWN)
            {
                if (cursor + 1 <= nOptions)
                {
                    cursor++;
                }
                else
                {
                    cursor = 1;
                }
            }

            else if (input == ARROW_UP)
            {
                if (cursor - 1 >= 1)
                {
                    cursor--;
                }
                else
                {
                    cursor = nOptions;
                }
            }
            if (input == ESC)
            {
                return BACK;
            }
        }
    }
}


void manualInputMenu(int operation, FileBlock* fileBlocks,  MetaFileInfo* metaFileInfo, PatcherState* patcherState)
{
    char* input = (char*) malloc(512000);
    char action[16] = "";
    if (operation == REMOVE_A_FILE)
    {
        strcpy(action,"remove");
    }
    else if (operation == RESTORE_A_FILE)
    {
        strcpy(action,"restore");
    }
    else if (operation == FIX_HOLES)
    {
        strcpy(action,"fix holes");
    }
    else if (operation == GET_TEXTURES)
    {
        strcpy(action,"get textures from");
    }

    while(1)
    {
        system("cls");
        printMainHeader();
        printf("Please type in files you want to %s separated by commas (,)\n\n", action);
        printf("\tExample: phw_00_ub_0004.pac,pew_01_uw_0001.dds,partcutdesc.xml\n\n");
        printf("     Note: If you don't know the file name, visit:\n");
        printf("               https://www.undertow.club/threads/8606/\n\n");
        printf("(Note:Type BACK to return to main menu)\n");
        printf("\nFiles to %s: ", action);
        fflush(stdin);
        fgets(input,512000,stdin);
        input[strlen(input) - 1] = '\0';

        if (contains(input,"BACK"))
        {
            break;
        }

        if(strcmpi(input,"\n") == 0)
        {
            printf("\nInvalid Input!\n\n");
            Sleep(500);
        }
        else
        {
            char* fileName = strtok(input,", ");
            int i = 0;

            clearFileBlocks(fileBlocks,metaFileInfo);

            while(fileName != NULL)
            {
                int found  = 0;
                printf("\n%s ", fileName);
                for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
                {
                    if (strcmpi(fileName,fileBlocks[i].fileName) == 0)
                    {
                        if (operation == FIX_HOLES|| operation == REMOVE_A_FILE)
                        {
                            fileBlocks[i].needPatch = 1;
                        }
                        else if (operation == RESTORE_A_FILE)
                        {
                            fileBlocks[i].needPatch = -1;
                        }
                        else if (operation == GET_TEXTURES)
                        {
                            extractTexturesFrom(&fileBlocks[i],fileBlocks,metaFileInfo,1);
                        }

                        found = 1;
                        break;
                    }
                }
                if (found)
                {
                     printColor("Found.", GREEN);
                }
                else
                {
                     printColor("Not Found.", RED);
                }
                fileName = strtok(NULL,", ");;
            }
            printf("\n");
            if (operation == FIX_HOLES)
            {
                generatePartcutdesc(fileBlocks,metaFileInfo,patcherState);
            }
            else if (operation == REMOVE_A_FILE)
            {
                //patchMetaFile(fileBlocks,metaFileInfo->fileBlocksCount,REMOVE_A_FILE,metaFileInfo,1);
                copyBlankFiles(fileBlocks,metaFileInfo,"files_to_patch\\");
            }
            else if (operation == RESTORE_A_FILE)
            {
                patchMetaFile(fileBlocks,metaFileInfo->fileBlocksCount,RESTORE_A_FILE,metaFileInfo,1);
            }


            PAUSE();
            break;
        }
    }
    free(input);
}

void filesFromTextureMenu(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo)
{
    char* input = (char*) malloc(64 * sizeof(char));
    int i = 0;
    for (i = 0; i < 64; i++)
    {
        input[i] = '\0';
    }

    while(1)
    {
        system("cls");
        printMainHeader();
        printf("Please type in texture name you want to search for:\n");
        printf("(Note:Type BACK to return to main menu)");
        printf("\n: ");
        fflush(stdin);
        fgets(input,64,stdin);

        if (contains(input,"BACK"))
        {
            break;
        }

        if(strcmpi(input,"\n") == 0)
        {
            printf("\nInvalid Input!\n\n");
            Sleep(500);
        }
        else
        {
            getFilesFromTexture(input, fileBlocks, metaFileInfo);
        }
    }
    free(input);
}


void menuWeapons(PatcherState* patcherState)
{
    char input = '\0';
    int cursor = 0;

    while(1)
    {
        system("cls");

        printMainHeader();
        printf("Choose which weapons for which class you want to remove\n");
        printf("                                                      | Remove only when not in combat");
        printWeaponOptionStatus("Warrior",WARRIOR,"Swords","Shields",patcherState,cursor);
        printWeaponOptionStatus("Sorceress",SORCERESS,"Amulets","Talismans",patcherState,cursor);
        printWeaponOptionStatus("Ranger",RANGER,"Bows","Daggers",patcherState,cursor);
        printWeaponOptionStatus("Berserker",BERSERKER,"Axes","Ornamental Knots",patcherState,cursor);
        printWeaponOptionStatus("Tamer",TAMER,"Short Swords","Trinket",patcherState,cursor);
        printWeaponOptionStatus("Blader(Musa)",BLADER,"Blades","Short Bows",patcherState,cursor);
        printWeaponOptionStatus("Valkyrie",VALKYRIE,"Swords","Shields",patcherState,cursor);
        printWeaponOptionStatus("Wizard",WIZARD,"Staffs","Daggers",patcherState,cursor);
        printWeaponOptionStatus("Witch",WITCH,"Staffs","Daggers",patcherState,cursor);
        printWeaponOptionStatus("Kunoichi",KUNOICHI,"Blades","Shuriken",patcherState,cursor);
        printWeaponOptionStatus("Ninja",NINJA,"Blades","Shuriken",patcherState,cursor);
        printWeaponOptionStatus("Dark Knight",DARK_KNIGHT,"Swords","Talismans",patcherState,cursor);
        printWeaponOptionStatus("Maehwa",MAEHWA,"Blades","Short Bows",patcherState,cursor);
        printWeaponOptionStatus("Striker",STRIKER,"Primary ","Secundary",patcherState,cursor);
        printWeaponOptionStatus("Mystic ",MYSTIC,"Primary ","Secundary",patcherState,cursor);
        printWeaponOptionStatus("Ran ",RAN,"Primary ","Secundary",patcherState,cursor);


        printf("\n\n");

        printf("\t[SPACE] - Change \"Remove only when not in combat\"\n\n");

        printf("[ARROW UP]   - Move cursor up  \t[ARROW LEFT]  - Change weapon option\t\n");
        printf("[ARROW DOWN] - Move cursor down\t[ARROW RIGHT] - Change weapon option\n\n");

        printf("ENTER] - Apply current settings\n");
        printf(" [ESC] - Back to main menu");
        input = getch();

        if (input == ARROW_RIGHT)
        {
            if (patcherState->weaponToRemove[cursor] + 1 <= AWAKENING_WEAPON)
            {
                patcherState->weaponToRemove[cursor]++;
            }
            else
            {
                patcherState->weaponToRemove[cursor] = REMOVE_ALL;
            }
        }
        else if (input == ARROW_LEFT)
        {
            if (patcherState->weaponToRemove[cursor] - 1 >= REMOVE_ALL)
            {
                patcherState->weaponToRemove[cursor]--;

            }
            else
            {
                patcherState->weaponToRemove[cursor] = AWAKENING_WEAPON;
            }
        }

        else if (input == ARROW_DOWN)
        {
            if ((cursor + 1) < TOTAL_CLASSES)
            {
                cursor++;
            }
            else
            {
                cursor = WARRIOR;
            }
        }
        else if (input == ARROW_UP)
        {
            if ((cursor - 1) >= WARRIOR)
            {
                cursor--;
            }
            else
            {
                cursor = TOTAL_CLASSES - 1;
            }
        }
        else if (input == SPACE)
        {
            patcherState->removeWeaponInOnly[cursor] = !patcherState->removeWeaponInOnly[cursor];
        }


        if (input == ESC)
        {
            break;
        }
        else if (input == ENTER ||  input == 10)
        {
            savePatcherState(patcherState,PATCHER_STATE_FILE_NAME);
            break;
        }
    }
}


void menuClassesToAply(PatcherState* patcherState)
{
    char input = '\0';
    int cursor = 0;

    // typedef enum {WARRIOR,SORCERESS,RANGER,BERSERKER,TAMER,BLADER,VALKYRIE,WIZARD,WITCH,KUNOICHI,NINJA,DARK_KNIGHT,PLUM,STRIKER} Class;
    while(1)
    {
        system("cls");
        printMainHeader();
        printf("Classes the mod will be applied to:\n");
        printMenuOption("Warrior     :",0,cursor);printOnOffSwitch(patcherState->modAppliesToClass[WARRIOR]);
        printMenuOption("Sorceress   :",1,cursor);printOnOffSwitch(patcherState->modAppliesToClass[SORCERESS]);
        printMenuOption("Ranger      :",2,cursor);printOnOffSwitch(patcherState->modAppliesToClass[RANGER]);
        printMenuOption("Berserker   :",3,cursor);printOnOffSwitch(patcherState->modAppliesToClass[BERSERKER]);
        printMenuOption("Tamer       :",4,cursor);printOnOffSwitch(patcherState->modAppliesToClass[TAMER]);
        printMenuOption("Blader(Musa):",5,cursor);printOnOffSwitch(patcherState->modAppliesToClass[BLADER]);
        printMenuOption("Valkyrie    :",6,cursor);printOnOffSwitch(patcherState->modAppliesToClass[VALKYRIE]);
        printMenuOption("Wizard      :",7,cursor);printOnOffSwitch(patcherState->modAppliesToClass[WIZARD]);
        printMenuOption("Witch       :",8,cursor);printOnOffSwitch(patcherState->modAppliesToClass[WITCH]);
        printMenuOption("Kunoichi    :",9,cursor);printOnOffSwitch(patcherState->modAppliesToClass[KUNOICHI]);
        printMenuOption("Ninja       :",10,cursor);printOnOffSwitch(patcherState->modAppliesToClass[NINJA]);
        printMenuOption("Dark Knight :",11,cursor);printOnOffSwitch(patcherState->modAppliesToClass[DARK_KNIGHT]);
        printMenuOption("Maehwa      :",12,cursor);printOnOffSwitch(patcherState->modAppliesToClass[MAEHWA]);
        printMenuOption("Striker     :",13,cursor);printOnOffSwitch(patcherState->modAppliesToClass[STRIKER]);
        printMenuOption("Mystic      :",14,cursor);printOnOffSwitch(patcherState->modAppliesToClass[MYSTIC]);
        printMenuOption("Ran         :",15,cursor);printOnOffSwitch(patcherState->modAppliesToClass[RAN]);

        printf("\n\n");

        printf("\t[SPACE] - Toggle \"On/Off\"\n\n");

        printf("[ARROW UP]   - Move cursor up\n");
        printf("[ARROW DOWN] - Move cursor down\n\n");

        printf("[ENTER] - Confirm settings\t[ESC] - Go back\n");
        input = getch();

        if (input == ARROW_DOWN)
        {
            if ((cursor + 1) < TOTAL_CLASSES)
            {
                cursor++;
            }
            else
            {
                cursor = WARRIOR;
            }
        }
        else if (input == ARROW_UP)
        {
            if ((cursor - 1) >= 0)
            {
                cursor--;
            }
            else
            {
                cursor = TOTAL_CLASSES - 1;
            }
        }
        else if (input == SPACE || input == ARROW_LEFT || input == ARROW_RIGHT)
        {
            patcherState->modAppliesToClass[cursor] = !patcherState->modAppliesToClass[cursor];
        }


        if (input == ESC)
        {
            break;
        }
        else if (input == ENTER ||  input == 10)
        {
            savePatcherState(patcherState,PATCHER_STATE_FILE_NAME);
            break;
        }
    }
}



void printStateCustomizeMenu(int* patcherOption, int type)
{
    if (type == ARMOR)
    {
        if ((*patcherOption) == REMOVE_STARTER_ARMORS)
        {
            printf("Remove Starter");
        }
        else if ((*patcherOption) == REMOVE_REGULAR_ARMORS)
        {
            printf("Remove Non-Cash-shop Armors only");
        }
        else if ((*patcherOption) == KEEP_ALL_ARMORS)
        {
            printf("Keep All");
        }
        else if ((*patcherOption) == -1)
        {
            (*patcherOption) = REMOVE_REGULAR_ARMORS;
             printf("Remove Non-Cash-shop Armors only");
        }
        else
        {
            (*patcherOption) = REMOVE_ALL_ARMORS;
            printf("Remove All");
        }
    }
    else if (type == UNDERWEAR)
    {
        if ((*patcherOption) == REMOVE_STARTER_UNDERWEAR)
        {
            printf("Remove Starter");
        }
        else if ((*patcherOption) == KEEP_ALL_UNDERWEAR)
        {
            printf("Keep All");
        }
        else if ((*patcherOption) == -1)
        {
            (*patcherOption) = REMOVE_STARTER_UNDERWEAR;
            printf("Remove Starter");
        }
        else
        {
            (*patcherOption) = REMOVE_ALL_UNDERWEAR;
            printf("Remove All ");
        }
    }
    else if (type == STOCKINGS)
    {
        if ((*patcherOption) == KEEP_ALL_STOCKINGS)
        {
            printf("Keep All");
        }
        else if ((*patcherOption) == REMOVE_STARTER_STOCKINGS)
        {
            printf("Remove Starters");
        }
        else if ((*patcherOption) == -1)
        {
            (*patcherOption) = REMOVE_STARTER_STOCKINGS;
            printf("Remove Starters");
        }
        else
        {
            (*patcherOption) = REMOVE_ALL_STOCKINGS;
             printf("Remove All");
        }
    }
    else if (type == CENSORSHIP)
    {
        if ((*patcherOption) == MINIMAL_CENSORSHIP_REMOVAL)
        {
            printf("Low");
        }
        else if ((*patcherOption) == MEDIUM_CENSORSHIP_REMOVAL)
        {
            printf("Medium");
        }
        else if ((*patcherOption) == HIGH_CENSORSHIP_REMOVAL)
        {
            printf("High");
        }
        else
        {
            if ((*patcherOption) == -1)
            {
                (*patcherOption) = HIGH_CENSORSHIP_REMOVAL;
                printf("High");
            }
            else
            {
                 (*patcherOption) = KEEP_CENSORSHIP;
                printf("Keep All");
            }
        }


    }
     else if (type == GLOVES)
    {
        if ((*patcherOption) == REMOVE_STARTER_GLOVES)
        {
            printf("Remove Starter Gloves only");
        }
        else if ((*patcherOption) == REMOVE_REGULAR_GLOVES)
        {
            printf("Remove Free Gloves only");
        }
        else if ((*patcherOption) == KEEP_ALL_GLOVES)
        {
            printf("Keep All");
        }
        else if ((*patcherOption) == -1)
        {
            (*patcherOption) = REMOVE_REGULAR_GLOVES;
            printf("Remove Free Gloves only");
        }
        else
        {
            (*patcherOption) = REMOVE_ALL_GLOVES;
             printf("Remove All");
        }
    }
    else if (type == BOOTS)
    {
         if ((*patcherOption) == REMOVE_STARTER_BOOTS)
        {
            printf("Remove Starter Boots only");
        }
        else if ((*patcherOption) == REMOVE_REGULAR_BOOTS)
        {
            printf("Remove Free Boots only");
        }
        else if ((*patcherOption) == KEEP_ALL_BOOTS)
        {
            printf("Keep All");
        }
        else if ((*patcherOption) == -1)
        {
            (*patcherOption) = REMOVE_REGULAR_BOOTS;
            printf("Remove Free Boots only");
        }
        else
        {
            (*patcherOption) = REMOVE_ALL_BOOTS;
             printf("Remove All");
        }
    }
    else if (type == HELMETS)
    {
        if ((*patcherOption) == REMOVE_ALL_HELMETS)
        {
            printf("Remove some Female Helmets");
        }
        else if ((*patcherOption) == -1)
        {
            (*patcherOption) = REMOVE_ALL_HELMETS;
            printf("Remove some Female Helmets");
        }
        else
        {
            (*patcherOption) = 0;
            printf("Keep All");
        }
    }
    else if (type == LIFE_SKILL_ARMORS)
    {
        if ((*patcherOption) == KEEP_ALL_LIFE_SKILL)
        {
            printf("Keep All");
        }
         else if ((*patcherOption) == -1)
        {
            (*patcherOption) = KEEP_ALL_LIFE_SKILL;
              printf("Keep All");
        }
        else
        {
            (*patcherOption) = 0;
            printf("Remove all except Treant Camouflage");
        }
    }

    else if (type == GENDER)
    {
        if ((*patcherOption) == MALE_ONLY)
        {
            printf("Male only");
        }
        else if ((*patcherOption) == BOTH_GENDERS)
        {
            printf("Both genders");
        }
         else if ((*patcherOption) == -1)
        {
            (*patcherOption) = BOTH_GENDERS;
             printf("Both genders");
        }
        else
        {
            (*patcherOption) = FEMALE_ONLY;
             printf("Female only");
        }
    }
    /*else if (type == NPC)
    {
        if ((*patcherOption) == KEEP_ALL)
        {
            printf("Keep All");
        }
        else if ((*patcherOption) < 0)
        {
            (*patcherOption) = KEEP_ALL;
            printf("Keep All");
        }
        else
        {
            (*patcherOption) = REMOVE_NPC_CLOTHING;
            printf("Remove Stable Female NPC Clothing");
        }
    }*/
    else if (type == USE_PENISES || type == USE_3D_VAGINAS)
    {
        if ((*patcherOption) == TRUE)
        {
            printf("Yes");
        }
        else if ((*patcherOption) < 0)
        {
            (*patcherOption) = TRUE;
             printf("Yes");
        }
        else
        {
            (*patcherOption) = FALSE;
            printf("No");
        }
    }
}

void printStatePubicHairMenu(int* pubicHairOption)
{
     switch ((*pubicHairOption))
     {
         case(FULL_BUSH):
         {
            printf("Full Bush");
         }break;
         case(FULL_BUSH_2):
         {
            printf("Full Bush 2");
         }break;
         case(FULL_BUSH_3):
         {
            printf("Full Bush 3");
         }break;
         case(MEDIUM_BUSH):
         {
            printf("Medium Bush");
         }break;
         case(MEDIUM_BUSH_2):
         {
            printf("Medium Bush 2");
         }break;
         case(SMALL_BUSH):
         {
            printf("Small Bush");
         }break;
         case(SMALL_BUSH_2):
         {
            printf("Small Bush 2");
         }break;
         case(THIN_LANDING_STRIP):
         {
            printf("Thin Landing Strip");
         }break;
         case(WIDE_LANDING_STRIP):
         {
            printf("Wide Landing Strip");
         }break;
         case(TRIMMED):
         {
            printf("Trimmed");
         }break;
         case(WIDER_TRIMMED):
         {
            printf("Wider Trimmed");
         }break;
         case(SHAVED_INNIE):
         {
            printf("Shaved Innie");
         }break;
         default:
         {
             (*pubicHairOption) = SHAVED;
             printf("Shaved");
         };
     }
}

void printPenisState(int penisState)
{
    if (penisState == NONE)
    {
        printf("None");
    }
    else if (penisState == NORMAL)
    {
        printf("Flaccid");
    }
    else if (penisState == HARD)
    {
        printf("Erect");
    }
}


void printWeaponOptionStatus(char* className, int CLASS_MACRO,char* primaryWeaponName, char* secundaryWeaponName,PatcherState* patcherState, int cursor)
{
    char label[64];
    int i = 0;
    int diff =0;

    if (cursor == CLASS_MACRO)
    {
        printf("\n --> ");
    }
    else
    {
        printf("\n     ");
    }
    printf("%s", className);

    diff = 15 - strlen(className);
    if (diff < 0)
    {
        diff = 0;
    }
    for (i = 0; i < diff ; i++)
    {
        printf(" ");
    }
    printf(": ");

    if (patcherState->weaponToRemove[CLASS_MACRO] == REMOVE_ALL)
    {
        sprintf(label,"Remove All");
    }
    else if (patcherState->weaponToRemove[CLASS_MACRO] == KEEP_ALL)
    {
       sprintf(label,"Keep All");
    }
    else if (patcherState->weaponToRemove[CLASS_MACRO] == PRIMARY_WEAPON)
    {
        sprintf(label,"Remove %s", primaryWeaponName);
    }
    else if (patcherState->weaponToRemove[CLASS_MACRO] == SECUNDARY_WEAPON)
    {
       sprintf(label,"Remove %s",secundaryWeaponName);
    }
    else if (patcherState->weaponToRemove[CLASS_MACRO] == BOTH_WEAPONS)
    {
        sprintf(label,"Remove %s and %s", primaryWeaponName, secundaryWeaponName);
    }
    else if (patcherState->weaponToRemove[CLASS_MACRO] == AWAKENING_WEAPON)
    {
        sprintf(label,"Remove Awakening Weapon");
    }
    printf("%s", label);


    diff = 32 - strlen(label);

    if (diff < 0)
    {
        diff = 0;
    }
    for (i = 0; i < diff ; i++)
    {
        printf(" ");
    }
    printf("| ");
    if (patcherState->removeWeaponInOnly[CLASS_MACRO])
    {
        printColor("True", GREEN);
    }
    else
    {
        printColor("False", RED);
    }
}





void checkFiles()
{
    char* currentPath = getCurrentPath();
    long totalFiles = 0;
    int metaFileFound = 0;
    char** pazFolderFiles = getFilesSingleFolder(currentPath,"meta",&totalFiles);
    free(currentPath);

    int i = 0;
    for (i = 0; i < totalFiles; i++)
    {
        if (strcmp(pazFolderFiles[i],"pad00000.meta") == 0)
        {
            metaFileFound = 1;
            break;
        }
    }
    free(pazFolderFiles);

    if (!metaFileFound && !fileExists("pad00000.meta"))
    {
        printf("\n\"pad00000.meta\" file not found.");
        printf("\nYou probably didn't extract the program to the correct folder");
        printf("\nIt should be in your \"Paz\" folder, where the \"pad0000.meta\" file is located\n");

        printf("\nThis program will not work without that file.");
        PAUSE();
    }

    if (!fileExists("Meta Injector.exe"))
    {
        if (!fileExists("MetaInjector.exe"))
        {
            printf("\nError: could not locate Meta Injector\n");
            printf("\nFor this version of the tool, you need to run MetaInjector after you install this mod.\n");
            printf("\n  Download it from here: https://www.undertow.club/threads/black-desert-online-modding-tools.9197/\n");
            printf("\nand extract it the same folder as this program\n\n");
            system("PAUSE");
        }
    }

    /*if (!fileExists("patcher_resources/texture/pbw_00_nude_0001.dds"))
    {
        printf("\nError: could not locate patcher_resources/texture/pbw_00_nude_0001.dds\n");
        printf("\nMake sure you have extracted ALL the files from the zip file\n\n");
        system("PAUSE");
    }

    if (!fileExists("patcher_resources/texture/pew_01_nude_0001.dds"))
    {
        printf("\nError: could not locate patcher_resources/texture/pew_01_nude_0001.dds\n");
        printf("\nMake sure you have extracted ALL the files from the zip file\n\n");
        system("PAUSE");
    }

    if (!fileExists("patcher_resources/texture/phw_01_nude_0001.dds"))
    {
        printf("\nError: could not locate patcher_resources/texture/phw_01_nude_0001.dds\n");
        printf("\nMake sure you have extracted ALL the files from the zip file\n\n");
        system("PAUSE");
    }

    if (!fileExists("patcher_resources/texture/pww_01_nude_0001.dds"))
    {
        printf("\nError: could not locate patcher_resources/texture/pww_01_nude_0001.dds\n");
        printf("\nMake sure you have extracted ALL the files from the zip file\n\n");
        system("PAUSE");
    }

     if (!fileExists("patcher_resources/texture/blank_dxt1.dds"))
    {
        printf("\nError: could not locate patcher_resources/texture/blank_dxt1.dds\n");
        printf("\nMake sure you have extracted ALL the files from the zip file\n\n");
        system("PAUSE");
    }
    if (!fileExists("patcher_resources/texture/blank_dxt5.dds"))
    {
        printf("\nError: could not locate patcher_resources/texture/blank_dxt5.dds\n");
        printf("\nMake sure you have extracted ALL the files from the zip file\n\n");
        system("PAUSE");
    }
    if (!fileExists("patcher_resources/texture/blank_ao_683.dds"))
    {
        printf("\nError: could not locate patcher_resources/texture/blank_ao_683.dds\n");
        printf("\nMake sure you have extracted ALL the files from the zip file\n\n");
        system("PAUSE");
    }
    if (!fileExists("patcher_resources/texture/blank_ao_171.dds"))
    {
        printf("\nError: could not locate patcher_resources/texture/blank_ao_171.dds\n");
        printf("\nMake sure you have extracted ALL the files from the zip file\n\n");
        system("PAUSE");
    }
    if (!fileExists("patcher_resources/texture/blank_ao_43.dds"))
    {
        printf("\nError: could not locate patcher_resources/texture/blank_ao_43.dds\n");
        printf("\nMake sure you have extracted ALL the files from the zip file\n\n");
        system("PAUSE");
    }*/
}

void deleteEverything()
{
    printColor("\nWarning:",RED);
    printf("\nThis will delete all the extra files that were created by this program.");
    printf("\nIncluding files like the \"removed_armors.txt\" and all the files in your \"files_to_patch\" folder");
    printf("\nYou will also lose all the configuration you've made.\n");


    printf("\nThis will also delete your BACKUPS");
    printf("\nand will probably make your game re-check files to re-download the pad0000.meta file when you launch it");
    printf("\n\nPlease make sure you are certain about this.");
    printf("\n\nAre you sure you want to continue? (y/n)");
    char input = '\0';
    input = getch();

    printf("\n\n");

    if (input == 'y' || input == 'Y' || input == ENTER)
    {
        char* root = bdoRootFolder();

        char* latestBackup = getLatestBackup();
        char* pazFolder = concatenate(root,"paz\\");

        printf("\n\nRestoring backup:\n");
        printf("%s -> pad00000.meta", latestBackup);
        systemCopyAndRename(latestBackup,"pad00000.meta",pazFolder,pazFolder);


        char* folderToDelete = concatenate(root,"character\\");
        deleteFolder(folderToDelete,1);
        free(folderToDelete);

        folderToDelete = concatenate(root,"object\\");
        deleteFolder(folderToDelete,0);
        free(folderToDelete);
        printf("\n");

        folderToDelete = concatenate(root,"gamecommondata\\");
        deleteFolder(folderToDelete,1);
        free(folderToDelete);

        deleteFolder("patcher_resources\\character\\",0);
        printf("\n");

        deleteFile("patcher_resources\\resorepless_latest_modifications.bin");
        deleteFile("patcher_resources\\resorepless_patcher_state.bin");
        deleteFile("patcher_resources\\removed_armors.txt");
        deleteFile("patcher_resources\\restored_armors.txt");
        deleteFile("patcher_resources\\partcutdesc_removed_armors.txt");
        deleteFile("patcher_resources\\red_markers.bin");
        deleteFile("patcher_resources\\red_markers_removed_armors.bin");
        deleteFile("patcher_resources\\red_markers_restored_armors.bin");
        deleteFile("patcher_resources\\red_markers_partcutdesc.bin");
        deleteFile("patcher_resources\\NULL");
        deleteFile("patcher_resources\\NULL1");
        deleteFile("patcher_resources\\NULL2");
        deleteFile("patcher_resources\\windows_version.txt");

        printf("\n\nDelete all your BACKUPS? (y/n)");
        char c = getch();

        if (c == 'y' || c == 'Y') {
            long backupsCount = 0;
            char** backups = getBackupList(&backupsCount);
            int i = 0;
            for (i = 0; i < backupsCount; i++)
            {
                remove(backups[i]);
                if (!fileExists(backups[i])) {
                    printf("\n%s deleted", backups[i]);
                }
            }
            free(backups);
        }
        else
        {
            printf("\nNOT deleted.");
        }


        printf("\n\nDeleting: files_to_patch\\\n");
        system("mkdir files_to_patch\\\\empty & robocopy /mir files_to_patch\\\\empty files_to_patch\\ >>patcher_resources\\NULL");

        //system("rmdir /S /Q files_to_patch\\\\empty >>patcher_resources\\NULL");
        remove("patcher_resources\\NULL");
        printf("\n");

        printf("\nDone.");

        deleteFile("NULL");
        deleteFile("NULL1");
        deleteFile("NULL2");
        deleteFile("windows_version.txt");

        char* versionDatPath = concatenate(root,"version.dat");
        remove(versionDatPath);
        free(versionDatPath);


        free(pazFolder);
        free(latestBackup);
        free(root);

        printf("\nFinished.");
        PAUSE();
    }
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
        printf("Type: / to go back to the main menu\n");
        printf("Type: LIST to list all patched files\n");
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

/*void searchResults(char* searchQuery, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo)
{
    int i = 0;
    int filesFoundCount = 0;

    List* filesToSearch = NULL;

    char* element = strtok(searchQuery," ,");
    while (element != NULL)
    {
        if (strlen(element) > 0)
        {
            int lastSlashIdx = 0;
            for (i = 0; i < strlen(element); i++)
            {
                if (element[i] == '/' || element[i] == '\\')
                {
                    lastSlashIdx = i;
                }
            }
            if (lastSlashIdx != 0)
            {
                FileBlock* fileBlock = (FileBlock*) malloc(sizeof(FileBlock));

                fileBlock->folderName = substr(element,0,lastSlashIdx+1);
                charReplace(fileBlock->folderName,'\\','/');

                fileBlock->fileName = substr(element,lastSlashIdx+1,strlen(element));

                insertList(fileBlock,&filesToSearch);
            }
            else
            {
                FileBlock* fileBlock = (FileBlock*) malloc(sizeof(FileBlock));
                fileBlock->fileName = element;
                fileBlock->folderName = NULL;

                insertList(fileBlock,&filesToSearch);
            }
        }
        element = strtok(NULL," ,");
    }

    printf("\nSearching...");
    int run = 0;
    int currentFile = 0;
    FileBlock** searchResultFiles = NULL;

    for (run = 0;  run < 2; run++)
    {
        if (run == 1)
        {
            searchResultFiles = (FileBlock**) malloc((filesFoundCount + 1) * sizeof(FileBlock*));
        }

        for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
        {
            struct ListNode* fileToSearchNode = filesToSearch;
            while(fileToSearchNode != NULL)
            {
                if (containsStrict(fileBlocks[i].fileName,fileToSearchNode->fileBlock->fileName))
                {
                    if (fileToSearchNode->fileBlock->folderName != NULL)
                    {
                        if (contains(fileBlocks[i].folderName,fileToSearchNode->fileBlock->folderName))
                        {
                            if (run == 0)
                            {
                                filesFoundCount++;
                            }
                            else
                            {
                                searchResultFiles[currentFile] = &fileBlocks[i];
                                fileToSearchNode->fileBlock->changeID = 1;
                                currentFile++;
                            }
                        }
                    }
                    else
                    {
                        if (run == 0)
                        {
                            filesFoundCount++;
                        }
                        else
                        {
                            searchResultFiles[currentFile] = &fileBlocks[i];
                            fileToSearchNode->fileBlock->changeID = 1;
                            currentFile++;
                        }
                    }
                }

                fileToSearchNode = fileToSearchNode->next;
            }
        }
    }

    struct ListNode* fileToSearchNode = filesToSearch;
    int atLeastOneNotFound = 0;
    while(fileToSearchNode != NULL)
    {
        if (fileToSearchNode->fileBlock->changeID != 1)
        {
            atLeastOneNotFound = 1;
            printf("\nNo file named \"%s\" was found. ", fileToSearchNode->fileBlock->fileName); */
            /*int highestSimilarity = 0;
            char* mostSimilarFileName = NULL;
            for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
            {
                if (similarity(fileBlocks[i].fileName,fileToSearchNode->fileBlock->fileName) > highestSimilarity)
                {
                    mostSimilarFileName = fileBlocks[i].fileName;
                }
            }
            printf("Didn't you mean: \"%s\"?", mostSimilarFileName);*/

       /* }
        fileToSearchNode = fileToSearchNode->next;
    }
    if (atLeastOneNotFound)
    {
        if (filesFoundCount > 0)
        {
            PAUSE();
        }
    }

    else if (filesFoundCount > 0)
    {
         printf("\nDone");
    }

    printf("\n\nSearch Results:\n");

    if (filesFoundCount > 0)
    {
          FILE* metaFile = openFile("pad00000.meta", "rb");
        for (i = 0; i < filesFoundCount; i++)
        {
            if (isPatched(searchResultFiles[i],metaFileInfo,metaFile))
            {
                printColor("[  Is Patched  ] ",GREEN);
            }
            else
            {
                printColor("[Is Not Patched] ",RED);
            }

            printf("%s\n", searchResultFiles[i]->fileName);
        }
        fclose(metaFile);
    }
    else
    {
        printf("No files found.");
    }

    if (searchResultFiles != NULL)
    {
        free(searchResultFiles);
    }

    PAUSE();
}*/
