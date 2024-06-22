#include "../include/paz_browser.h"

int MAX_FILE_NAME_LENGTH = 29;
int MAX_LINES = 15;
int MAX_ROWS = 3;

int displayRealFileNames = 1;

void readRealNamesFile(FileBlock** filesOnMenu, int nOptions)
{
    FILE* armor_to_swap_file = openFile("patcher_resources\\real_names.txt","rb");

    char c;
    int i = 0;
    int arrowFound = 0;
    int readingArmorNumber = 1;
    int result = 0;
    Armor* armor = NULL;

    int linesCount = 0;
    int lineLength = 1;
    while(fread(&c,1,1,armor_to_swap_file))
    {
        //Skip comments
        if (c == '#')
        {
            while(c!= '\n')
            {
                if (!fread(&c,1,1,armor_to_swap_file))
                {
                    break;
                }
            }
        }
        else if (c == '\n')
        {
            if (lineLength > 1)
            {
                linesCount++;
            }
            lineLength = 0;
        }
        else
        {
            lineLength++;
        }
    }

    rewind(armor_to_swap_file);


    armor = (Armor*) malloc((linesCount + 1) * sizeof(Armor));
    for(i = 0; i < linesCount; i++)
    {
        armor[i].fileName   = (char*) malloc(128);
        armor[i].RealName = (char*) malloc(128);
    }
    i = 0;
    int currentArmor = 0;
    while (1)
    {
        result = fread(&c,1,1,armor_to_swap_file);

        //Skip comments
        if (c == '#')
        {
            while(c != '\n')
            {
                if (!fread(&c,1,1,armor_to_swap_file))
                {
                    break;
                }
            }
        }
        else if (readingArmorNumber == 1 && c != '\n')
        {
            armor[currentArmor].fileName[i] = c;
            i++;
        }
        else if (readingArmorNumber == 2 && c != '\n')
        {
            armor[currentArmor].RealName[i] = c;
            i++;
        }

        if (c == '\n' && arrowFound == 0)
        {
            //printf("\nSymbol \"->\" not found");
            i = 0;
        }
        if (c == '-')
        {
            fread(&c,1,1,armor_to_swap_file);
            if (c == '>')
            {
                armor[currentArmor].fileName[i-1] = '\0';
                arrowFound = 1;
                readingArmorNumber = 2;

            }
            i = 0;
        }
        if ((c == '\n' && arrowFound == 1) || (result == 0 && arrowFound == 1))
        {
            armor[currentArmor].RealName[i-1] = '\0';
           /* printf("\narmor1: %s", armor[currentArmor].fileName);
            printf("\narmor2: %s\n\n", armor[currentArmor].RealName);*/
            arrowFound = 0;
            readingArmorNumber = 1;
            i = 0;
            currentArmor++;
        }
        if (result == 0)
        {
            break;
        }
    }

    fclose(armor_to_swap_file);
    /*for(i = 0; i < linesCount; i++)
    {
        printf("\n%s->%s",armor[i].fileName ,armor[i].RealName);
    }
    PAUSE();*/

    int j = 0;
    for (i = 0; i < nOptions; i++)
    {
        // Makes sure this doesn't run again if is already filled
        if (!filesOnMenu[i]->triedToUpdateRealName)
        {
            filesOnMenu[i]->triedToUpdateRealName = 1;

            if (isRegular(filesOnMenu[i]->fileName)) // If it's NOT a Cash Shop Item
            {
                filesOnMenu[i]->isNonCashShopItem = 1;
            }

            for(j = 0; j < linesCount; j++)
            {
                //printf("\n%s  - %s (%d)", armor[j].fileName,filesOnMenu[i]->fileName,contains(filesOnMenu[i]->fileName,armor[j].fileName));

                if (contains(filesOnMenu[i]->fileName,armor[j].fileName))
                {
                    if (strstr(filesOnMenu[i]->fileName,"uw") && strstr(armor[j].fileName,"uw"))
                    {
                        char* fourDigits = getFourDigits(filesOnMenu[i]->fileName);
                        if ((fourDigits[0] == '1') && !strstr(armor[j].RealName,"Sileshi"))
                        {
                            filesOnMenu[i]->RealName = concatenate(armor[j].RealName," Underwear (No Stockings)");
                        }
                        else
                        {
                            filesOnMenu[i]->RealName = concatenate(armor[j].RealName," Underwear");
                        }
                        filesOnMenu[i]->hasRealName = 1;

                       /* printf("\n%s", filesOnMenu[i]->RealName);
                        PAUSE();*/
                    }
                    else if (!strstr(filesOnMenu[i]->fileName,"uw") && filesOnMenu[i]->fileName[0] == 'p' && (strstr(filesOnMenu[i]->folderName,"armor") || strstr(filesOnMenu[i]->folderName,"texture") || strstr(filesOnMenu[i]->folderName,"weapon") || contains(filesOnMenu[i]->fileName,"*_10_*_0004*")))
                    {
                            if (isRegular(filesOnMenu[i]->fileName)) // If it's NOT a Cash Shop Item
                            {
                                filesOnMenu[i]->isNonCashShopItem = 1;

                                // If it has no name yet
                                if ((filesOnMenu[i]->RealName == NULL) || ((filesOnMenu[i]->RealName != NULL) && (strlen(filesOnMenu[i]->RealName) < 4)))
                                {
                                     if (!strstr(armor[j].RealName,"Guessed"))
                                     {
                                        filesOnMenu[i]->RealName = armor[j].RealName;
                                        filesOnMenu[i]->hasRealName = 1;
                                     }
                                    else // If it's a guessed name
                                    {
                                        char* classPrefix = getClassPrefix(filesOnMenu[i]->fileName);
                                        if (!beginsWith(classPrefix,"phw")
                                        && !beginsWith(classPrefix,"pew")
                                        && !beginsWith(classPrefix,"pvw")
                                        && !beginsWith(classPrefix,"pbw")
                                        && !beginsWith(classPrefix,"pww")
                                        && !beginsWith(classPrefix,"phm")
                                        && !beginsWith(classPrefix,"pgm")
                                        && !beginsWith(classPrefix,"pkww")
                                        && !beginsWith(classPrefix,"pkm")
                                        )
                                        {
                                            filesOnMenu[i]->RealName = armor[j].RealName;
                                             filesOnMenu[i]->hasRealName = 1;
                                        }
                                    }


                                }
                                else // If a name was already assigned
                                {   // and this isn't a guessed name
                                    if (!strstr(armor[j].RealName,"Guessed"))
                                    {
                                        filesOnMenu[i]->RealName = concatenate3(filesOnMenu[i]->RealName,"/",armor[j].RealName);
                                        filesOnMenu[i]->hasRealName = 1;
                                    }
                                }
                            }
                            else // Case is IS a Cash Shop item
                            {
                                filesOnMenu[i]->RealName = armor[j].RealName;
                                filesOnMenu[i]->hasRealName = 1;
                            }

                    }
                }
            }

            for(j = 0; j < linesCount; j++)
            {
                if (filesOnMenu[i]->hasRealName)
                {
                    char* partName = NULL;

                    if (strstr(filesOnMenu[i]->folderName,"weapon"))
                    {
                        if (strstr(filesOnMenu[i]->folderName,"1_onehandsword"))
                        {
                            partName = concatenate(partName," One Hand Sword");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"8_shield"))
                        {
                            partName = concatenate(partName," Shield");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"3_twohandsword"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }

                        if (strstr(filesOnMenu[i]->folderName,"28_enchantarm"))
                        {
                            partName = concatenate(partName," Amulet");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"33_talisman"))
                        {
                            partName = concatenate(partName," Talisman");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"38_scythe"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }




                        if (strstr(filesOnMenu[i]->folderName,"31_onehandbow") || strstr(filesOnMenu[i]->fileName,"bow"))
                        {
                            partName = concatenate(partName," Bow");
                        }
                        else if (strstr(filesOnMenu[i]->fileName,"arw"))
                        {
                            partName = concatenate(partName," Arrow");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"32_dagger"))
                        {
                            partName = concatenate(partName," Dagger");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"43_twinsword"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }



                        if (strstr(filesOnMenu[i]->folderName,"29_doubleax"))
                        {
                            partName = concatenate(partName," Axe");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"34_aiguillette"))
                        {
                            partName = concatenate(partName," Aiguillette");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"42_handcanon"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }


                        if (strstr(filesOnMenu[i]->folderName,"37_norigae"))
                        {
                            partName = concatenate(partName," Trinket");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"44_stick"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }



                        if (strstr(filesOnMenu[i]->folderName,"35_blade"))
                        {
                            partName = concatenate(partName," Blade");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"36_shotbow"))
                        {
                            partName = concatenate(partName," Short Bow");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"47_glaive"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }




                        if (strstr(filesOnMenu[i]->folderName,"45_spear"))
                        {
                            partName = concatenate(partName," Spear");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"46_spearshield"))
                        {
                            partName = concatenate(partName," Spear Shield");
                        }


                        if (strstr(filesOnMenu[i]->folderName,"6_staff"))
                        {
                            partName = concatenate(partName," Staff");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"32_dagger"))
                        {
                            partName = concatenate(partName," Dagger");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"59_orb"))
                        {
                            partName = concatenate(partName," Orbs");
                        }



                        if (strstr(filesOnMenu[i]->folderName,"30_elblade"))
                        {
                            partName = concatenate(partName," Blade");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"55_shuriken"))
                        {
                            partName = concatenate(partName," Shuriken");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"56_darts"))
                        {
                            partName = concatenate(partName," Dart");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"57_chakram"))
                        {
                            partName = concatenate(partName," Chakram");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"58_katana"))
                        {
                            partName = concatenate(partName," Katana");
                        }


                        if (strstr(filesOnMenu[i]->folderName,"63_longblade"))
                        {
                            partName = concatenate(partName," Long Blade");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"64_darksword"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }


                         if (strstr(filesOnMenu[i]->folderName,"65_gauntlet"))
                        {
                            partName = concatenate(partName," Gauntlet");
                        }
                        else if (strstr(filesOnMenu[i]->folderName,"66_armlet"))
                        {
                            partName = concatenate(partName," Armlet");
                        }


                        if (strstr(filesOnMenu[i]->folderName,"47_glaive"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }

                        if (strstr(filesOnMenu[i]->folderName,"71_godgauntlet"))
                        {
                            partName = concatenate(partName," Awakening Weapon");
                        }


                        if (strstr(filesOnMenu[i]->fileName,"_in.pac"))
                        {
                            partName = concatenate(partName," (Not Withdrawn)");
                        }
                    }
                    else
                    {
                        if (strstr(filesOnMenu[i]->fileName,"_hand_"))
                        {
                            partName = concatenate(partName," Gloves");
                        }
                        else if (strstr(filesOnMenu[i]->fileName,"_foot_"))
                        {
                            partName = concatenate(partName," Boots");
                        }
                        else if (strstr(filesOnMenu[i]->fileName,"_cloak_"))
                        {
                            partName = concatenate(partName," Cape");
                        }
                        else if (strstr(filesOnMenu[i]->fileName,"_hel_"))
                        {
                            partName = concatenate(partName," Helmet");
                        }
                        else
                        {
                            if (filesOnMenu[i]->isNonCashShopItem)
                            {
                                partName = concatenate(partName," Armor");
                            }
                            else if (strstr(filesOnMenu[i]->RealName,"Shark")
                                     || strstr(filesOnMenu[i]->RealName,"Desert")
                                     || strstr(filesOnMenu[i]->RealName,"Fish")
                                     || strstr(filesOnMenu[i]->RealName,"Treant")
                                     || strstr(filesOnMenu[i]->RealName,"Riding")
                                     || strstr(filesOnMenu[i]->RealName,"Maid")
                                     || strstr(filesOnMenu[i]->RealName,"Canape")
                                     || strstr(filesOnMenu[i]->RealName,"Masquerade")
                                     )
                            {
                                partName = concatenate(partName," Costume");
                            }
                            // If it's not an underwear
                            else if (!strstr(filesOnMenu[i]->fileName,"uw"))
                            {
                                partName = concatenate(partName," Outfit");
                            }

                        }
                    }
                    if (filesOnMenu[i]->hasRealName)
                    {
                        char* partNamePointerBeforeConcatenate = partName;

                        if (strstr(filesOnMenu[i]->fileName,"_01.pac")
                         || strstr(filesOnMenu[i]->fileName,"_02.pac")
                         || strstr(filesOnMenu[i]->fileName,"_03.pac")
                         || strstr(filesOnMenu[i]->fileName,"_04.pac")
                         || strstr(filesOnMenu[i]->fileName,"_06.pac")
                         || strstr(filesOnMenu[i]->fileName,"_07.pac")
                         || strstr(filesOnMenu[i]->fileName,"_08.pac")
                         || strstr(filesOnMenu[i]->fileName,"_09.pac")
                         || strstr(filesOnMenu[i]->fileName,"_10.pac")
                         )
                        {
                            if (strstr(filesOnMenu[i]->fileName,"_ub_0034_01.pac"))
                            {
                                partName = concatenate(partName," (Non-Winged)");
                            }
                            else
                            {
                                char variationName[16] = "";
                                sprintf(variationName," Variation %c",filesOnMenu[i]->fileName[strlen(filesOnMenu[i]->fileName) - 5]);
                                partName = concatenate(partName,variationName);
                            }
                        }
                        if (strstr(filesOnMenu[i]->fileName,"_01_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_02_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_03_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_04_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_06_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_07_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_08_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_09_dm.pac")
                         || strstr(filesOnMenu[i]->fileName,"_10_dm.pac"))
                         {
                            if (strstr(filesOnMenu[i]->fileName,"_ub_0034_01_dm.pac"))
                            {
                                partName = concatenate(partName," (Non-Winged)");
                            }
                            else
                            {
                                char variationName[16] = "";
                                sprintf(variationName," Variation %c",filesOnMenu[i]->fileName[strlen(filesOnMenu[i]->fileName) - 8]);
                                partName = concatenate(partName,variationName);
                            }
                         }

                        if (strstr(filesOnMenu[i]->fileName,"_dm"))
                        {
                            partName = concatenate(partName," Damaged");
                        }

                         if (strstr(filesOnMenu[i]->fileName,"_mul"))
                         {
                            partName = concatenate(partName," (Awakening Weapon Withdrawn)");
                         }
                         if (strstr(filesOnMenu[i]->fileName,"_na.pac"))
                         {
                            partName = concatenate(partName," NA");
                         }
                        if (strstr(filesOnMenu[i]->fileName,"_s.pac")|| strstr(filesOnMenu[i]->fileName,"_s_dm.pac"))
                         {
                            partName = concatenate(partName," S");
                         }

                        if (partNamePointerBeforeConcatenate != partName && partNamePointerBeforeConcatenate != NULL)
                        {
                            free(partNamePointerBeforeConcatenate);
                        }
                    }


                    if (partName != NULL)
                    {
                        if (!filesOnMenu[i]->partNameAlreadyAssined)
                        {
                            filesOnMenu[i]->RealName = concatenate(filesOnMenu[i]->RealName,partName);
                            filesOnMenu[i]->partNameAlreadyAssined = 1;
                        }
                    }
                }
            }

        }
    }
}



char* menuSelectClass(int* cursor)
{
    MenuElements menu;
    menu.nOptions = 15;
    strcpy(menu.header,"Select a class:");
    strcpy(menu.menuoptions[0],"Warrior");
    strcpy(menu.menuoptions[1],"Sorceress");
    strcpy(menu.menuoptions[2],"Ranger");
    strcpy(menu.menuoptions[3],"Berserker");
    strcpy(menu.menuoptions[4],"Tamer");
    strcpy(menu.menuoptions[5],"Blader(Musa)");
    strcpy(menu.menuoptions[6],"Valkyrie");
    strcpy(menu.menuoptions[7],"Wizard");
    strcpy(menu.menuoptions[8],"Witch");
    strcpy(menu.menuoptions[9],"Kunoichi");
    strcpy(menu.menuoptions[10],"Ninja");
    strcpy(menu.menuoptions[11],"Dark Knight");
    strcpy(menu.menuoptions[12],"Plum(Maehwa)");
    strcpy(menu.menuoptions[13],"Striker");
    strcpy(menu.menuoptions[14],"Mystic");
    strcpy(menu.menuoptions[15],"Ran");


    return createArrowMenu(menu, cursor);
}

char* menuSelectPart(int* cursor)
{
    MenuElements menu;
    menu.nOptions = 11;
    strcpy(menu.header,"Select a part:");
    strcpy(menu.menuoptions[0],"Upperbody");
    strcpy(menu.menuoptions[1],"Lowerbody");
    strcpy(menu.menuoptions[2],"Hand");
    strcpy(menu.menuoptions[3],"Foot");
    strcpy(menu.menuoptions[4],"Helmet");
    strcpy(menu.menuoptions[5],"Shoulder");
    strcpy(menu.menuoptions[6],"Cloak");
    strcpy(menu.menuoptions[7],"Underwear");
    strcpy(menu.menuoptions[8],"Event Costume");
    strcpy(menu.menuoptions[9],"Body Mesh");
    strcpy(menu.menuoptions[10],"Head");
    strcpy(menu.menuoptions[11],"Weapons");


    return createArrowMenu(menu,cursor);
}

char* menuSelectExt()
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
            printCursor(cursor,1);printf("Armor Files");
            printCursor(cursor,2);printf("Texture Files");

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
                if (cursor == 1)
                {
                    return "pac";
                }
                if (cursor == 2)
                {
                    return "dds";
                }
                return "Back";
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
                return "Back";
            }
        }
    }
    return "Back";
}

int shouldDisplayFile(FileBlock* fileBlock, char* partFolderPrefix, char* classPrefix, char*  partPrefix, char* fileExt)
{
    if (fileBlock->fileName[0] != 'p' || !fileHasExt(fileBlock->fileName,fileExt))
    {
        return 0;
    }
    if (contains(fileBlock->folderName,"weapon") && contains(fileBlock->folderName,"0_com"))
    {
        return 0;
    }
    if ((contains(fileBlock->folderName,classPrefix)) && (contains(fileBlock->folderName,partFolderPrefix)) && fileHasExt(fileBlock->fileName,"pac"))
    {
        return 1;
    }
    else if (fileHasExt(fileBlock->fileName,"dds"))
    {
        if (contains(fileBlock->fileName,classPrefix) && contains(fileBlock->fileName, partPrefix))
        {
            int skip = 0;

            if (contains(fileBlock->fileName,"contribute") && strcmpi(partPrefix,"event_costume") != 0)
            {
                return 0;
            }

            if (skip == 0)
            {
                return 1;
            }
        }
        else if (((strcmpi(partPrefix,"_uw_") == 0) && fileHasExt(fileBlock->fileName,"dds")) || contains(fileBlock->fileName,"_99_"))
        {
            if (isStockingTexture(fileBlock->fileName))
            {
                return 1;
            }
        }
    }

    return 0;
}





void loadMarkedFiles(FileBlock* fileBlocks, MetaFileInfo *metaFileInfo, int operation)
{
    printf("\nLoading marked files...");
    clearFileBlocks(fileBlocks,metaFileInfo);

    int i = 0;
    int j = 0;

    int paz_browser_patcher_state_n_files = 0;
    PazBrowserPatcherState* pazBrowserPatcherState =  load_paz_browser_patcher_state(&paz_browser_patcher_state_n_files, operation);
    printf("\nDone\n");




    if (pazBrowserPatcherState != NULL)
    {
        printf("\n\nMarking files...");
        int filesFound = 0;
        for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
        {
            if (fileBlocks[i].fileName[0] == 'p' && (contains(fileBlocks[i].folderName,"1_pc") || contains(fileBlocks[i].folderName,"texture")))
            {
                for (j = 0; j < paz_browser_patcher_state_n_files; j++)
                {
                    if (strcmp(fileBlocks[i].fileName,pazBrowserPatcherState->fileName[j]) == 0)
                    {
                        fileBlocks[i].needPatch = 1;
                        fileBlocks[i].changeID = getFileColor(fileBlocks[i].hash,operation);

                        filesFound++;
                        break;
                    }
                    if (filesFound == paz_browser_patcher_state_n_files)
                    {
                        break;
                    }
                }
            }
        }
        printf("\nDone");
    }

    for(i = 0; i < paz_browser_patcher_state_n_files; i++)
    {
        free(pazBrowserPatcherState->fileName[i]);
    }
    free(pazBrowserPatcherState);
}



int runPazBrowser(FileBlock* fileBlocks,MetaFileInfo *metaFileInfo, char* operationName, int operation,PatcherState* patcherState)
{

    int classSelected_idx = 0;
    int partSelected_idx = 0;
    if(!isWindows10)
    {
       // MAX_ROWS = 2;
       MAX_FILE_NAME_LENGTH = 22;
    }

    // CLEAR FILE BLOCKS
    //clearFileBlocks(fileBlocks,metaFileInfo);


    if (operation != GET_TEXTURES)
    {
       loadMarkedFiles(fileBlocks, metaFileInfo, operation);
    }


    while(1)
    {
        char* fileExt = "pac";
        if (operation <= RESTORE_A_FILE)
        {
            fileExt = menuSelectExt();
        }


        if (strcmpi(fileExt,"Back") == 0)
        {
            break;
        }
        while(1)
        {
            char* classSelected = menuSelectClass(&classSelected_idx);

            if (strcmpi(classSelected,"Back") != 0)
            {

                while(1)
                {
                    char* partSelected = menuSelectPart(&partSelected_idx);

                    if (strcmpi(partSelected,"Back") != 0)
                    {

                        while(menuSelectFile(classSelected,partSelected,fileExt,fileBlocks,metaFileInfo, operation,patcherState))
                        {

                        }
                    }
                    else
                    {
                         break;
                    }
                }
                partSelected_idx = 0;
            }
            else
            {
                break;
            }
         }

        if (operation != REMOVE_A_FILE && operation != RESTORE_A_FILE)
        {
            break;
        }
    }


    return 0;
}

void previewFile(FileBlock* fileBlockSelected, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, int openAfterExtracted)
{

 if (fileHasExt(fileBlockSelected->fileName,"pac"))
 {
        int fileIsExtracted = 0;

         char* extractionPath = concatenate("patcher_resources\\extracted_files\\",substr(fileBlockSelected->fileName,0,strlen(fileBlockSelected->fileName)-3));
        extractionPath[strlen(extractionPath)-1] = '\\';


        if (!fileExists(concatenate(extractionPath,fileBlockSelected->fileName)))
        {
            fileIsExtracted = extractFile(fileBlockSelected);
        }
        else
        {
            printf("\nFile already extracted\n");
            fileIsExtracted = 1;
        }

        if (fileIsExtracted)
        {

                extractTexturesFrom(fileBlockSelected, fileBlocks, metaFileInfo, openAfterExtracted);

                if (openAfterExtracted)
                {
                    char* filePath = concatenate(getCurrentPath(),extractionPath);
                    char* command = (char*) malloc((strlen("cd \"patcher_resources\\3d_converter\\\" & 3dconverter.exe \"%s\"") + strlen(filePath) + 64));
                    filePath = concatenate(filePath,fileBlockSelected->fileName);
                    charReplace(filePath,'/','\\');

                    sprintf(command,"cd \"patcher_resources\\3d_converter\\\" & 3dconverter.exe \"%s\"",filePath);
                    printf("Opening \"%s\" with 3D Object Converter...", fileBlockSelected->fileName);
                    printf("\n\ncommand executed:\n%s\n\n", command);



                    system(command);

                    free(command);
                    free(filePath);
                 }
        }
    }
    else
    {
        extractFileCustomPath(fileBlockSelected,"patcher_resources\\extracted_files\\",1);

        char* command = (char*) malloc(strlen(fileBlockSelected->fileName) + strlen("cd %s && %s") + 100);
        sprintf(command,"\"cd %s & %s","patcher_resources\\extracted_files\\",fileBlockSelected->fileName);

        system(command);
        free(command);
    }
}


char* getPartFolderPrefix(char* partSelected)
{
    if (strcmpi(partSelected,"Upperbody") == 0)
    {
        return "9_upperbody";
    }
    else if (strcmpi(partSelected,"Lowerbody") == 0)
    {
        return "10_lowerbody";
    }
    else if (strcmpi(partSelected,"Hand") == 0)
    {
        return "11_hand";
    }
    else if (strcmpi(partSelected,"Foot") == 0)
    {
        return "12_foot";
    }
    else if (strcmpi(partSelected,"Helmet") == 0)
    {
        return "13_hel";
    }
    else if (strcmpi(partSelected,"Shoulder") == 0)
    {
        return "14_sho";
    }
    else if (strcmpi(partSelected,"Cloak") == 0)
    {
        return "19_cloak";
    }
    else if (strcmpi(partSelected,"Underwear") == 0)
    {
        return "38_underwear";
    }
    else if (strcmpi(partSelected,"Event Costume") == 0)
    {
        return "event_costume";
    }
     else if (strcmpi(partSelected,"Weapons") == 0)
    {
        return "weapon";
    }
     else if (strcmpi(partSelected,"Body Mesh") == 0)
    {
        return "nude";
    }
     else if (strcmpi(partSelected,"Head") == 0)
    {
        return "head";
    }
    else
    {
        printf("\nSelected part \"%s\" is has not the expected name\n", partSelected);
        system("PAUSE");
    }
    return "";
}



char* getClassPrefixFromMenuOption(char* classSelected)
{
    if (strcmpi(classSelected,"Warrior") == 0)
    {
        return "phm";
    }
    else if (strcmpi(classSelected,"Sorceress") == 0)
    {
        return "phw";
    }
    else if (strcmpi(classSelected,"Ranger") == 0)
    {
        return "pew";
    }
    else if (strcmpi(classSelected,"Berserker") == 0)
    {
        return "pgm";
    }
    else if (strcmpi(classSelected,"Tamer") == 0)
    {
        return "pbw";
    }
    else if (strcmpi(classSelected,"Blader(Musa)") == 0)
    {
        return "pkm";
    }
    else if (strcmpi(classSelected,"Valkyrie") == 0)
    {
        return "pvw";
    }
    else if (strcmpi(classSelected,"Wizard") == 0)
    {
       return "pwm";
    }
    else if (strcmpi(classSelected,"Witch") == 0)
    {
        return "pww";
    }
    else if (strcmpi(classSelected,"Kunoichi") == 0)
    {
        return "pnw";
    }
    else if (strcmpi(classSelected,"Ninja") == 0)
    {
        return "pnm";
    }
    else if (strcmpi(classSelected,"Dark Knight") == 0)
    {
        return "pdw";
    }
    else if (strcmpi(classSelected,"Plum(Maehwa)") == 0)
    {
        return "pkw";
    }
    else if (strcmpi(classSelected,"Striker") == 0)
    {
        return "pcm";
    }
    else if (strcmpi(classSelected,"Mystic") == 0)
    {
        return "pcw";
    }
    else if (strcmpi(classSelected,"Ran") == 0)
    {
        return "psw";
    }
    else
    {
        printf("\nSelected class \"%s\" is has not the expected name\n", classSelected);
        system("PAUSE");
    }
    return "";
}


char* getPartPrefixFromMenuOption(char* partSelected)
{
    if (strcmpi(partSelected,"Upperbody") == 0)
    {
        return "_ub_";
    }
    else if (strcmpi(partSelected,"Lowerbody") == 0)
    {
        return "_lb_";
    }
    else if (strcmpi(partSelected,"Hand") == 0)
    {
        return "_hand_";
    }
    else if (strcmpi(partSelected,"Foot") == 0)
    {
        return "_foot_";
    }
    else if (strcmpi(partSelected,"Helmet") == 0)
    {
        return "_hel_";
    }
    else if (strcmpi(partSelected,"Shoulder") == 0)
    {
        return "_sho_";
    }
    else if (strcmpi(partSelected,"Cloak") == 0)
    {
        return "_cloak_";
    }
    else if (strcmpi(partSelected,"Underwear") == 0)
    {
        return "_uw_";
    }
    else if (strcmpi(partSelected,"Event Costume") == 0)
    {
        return "event_costume";
    }
     else if (strcmpi(partSelected,"Weapons") == 0)
    {
        return "weapon";
    }
     else if (strcmpi(partSelected,"Body Mesh") == 0)
    {
        return "nude";
    }
     else if (strcmpi(partSelected,"Head") == 0)
    {
        return "head";
    }
    else
    {
        printf("\nSelected part \"%s\" is has not the expected name\n", partSelected);
        system("PAUSE");
    }
    return "";
}


void compensateLength(FileBlock** filesOnMenu,int nOptions, int currentFileIDX)
{
    int row = (int)((float)currentFileIDX/(float)MAX_LINES);


    int rowStartIDX = row * MAX_LINES;
    int rowEndIDX = ((row+1) * MAX_LINES)  - 1;
    if (rowEndIDX > nOptions - 1)
    {
        rowEndIDX = nOptions - 1;
    }

    char* nameToDisplay = filesOnMenu[currentFileIDX]->fileName;

    if (filesOnMenu[currentFileIDX]->hasRealName && displayRealFileNames)
    {
        nameToDisplay = filesOnMenu[currentFileIDX]->RealName;
    }

    int longestLength = 0;


    int i = 0;
    for (i = rowStartIDX; i <= rowEndIDX; i++)
    {
        if (displayRealFileNames && filesOnMenu[i]->hasRealName)
        {
            if (strlen(filesOnMenu[i]->RealName) > longestLength)
            {
                longestLength = strlen(filesOnMenu[i]->RealName);
            }
        }
        else
        {
            if (strlen(filesOnMenu[i]->fileName) > longestLength)
            {
                longestLength = strlen(filesOnMenu[i]->fileName);
            }
        }
    }

    /*if (strlen(nameToDisplay) == longestLength)
    {
        char *strToPrint = (char*)malloc(32);
        sprintf(strToPrint," %d - %d", strlen(nameToDisplay),longestLength);
        printColor(strToPrint,YELLOW);
    }
    else
    {
        printf(" %d - %d", strlen(nameToDisplay),longestLength);
    }*/

    for(i = 0; i < (longestLength - strlen(nameToDisplay)); i++)
    {
        printf(" ");
    }
}

int getCursorRow(int cursorPos)
{
    return ((int)(cursorPos/MAX_LINES));
}

int getCursorLine(int cursorPos)
{
    return ((int)(cursorPos- (getCursorRow(cursorPos)*MAX_LINES)));
}

void nextPage(int* pageStart, int* pageEnd,int* longestLength, FileBlock** filesOnMenu,int nOptions,MetaFileInfo*  metaFileInfo)
{
    if ((*pageEnd) < nOptions)
    {
        (*pageStart) = (*pageEnd);
        (*pageEnd) += MAX_LINES* MAX_ROWS;
    }
    recalculateSettings(filesOnMenu,nOptions,(*pageStart),metaFileInfo);
}

void previousPage(int* pageStart, int* pageEnd,int* longestLength, FileBlock** filesOnMenu,int nOptions,MetaFileInfo*  metaFileInfo)
{
    if ((*pageStart) - (MAX_LINES* MAX_ROWS) >= 0)
    {
        (*pageStart) -= (MAX_LINES* MAX_ROWS);
        (*pageEnd) -= MAX_LINES* MAX_ROWS;
    }
    recalculateSettings(filesOnMenu,nOptions,(*pageStart),metaFileInfo);
}

void goToPage(int pageNum, int* pageStart, int* pageEnd, int nOptions)
{
    int pageStartLocal = pageNum * (MAX_ROWS*MAX_LINES);
    int pageEndLocal = pageStartLocal + (MAX_ROWS*MAX_LINES);

    if(pageStartLocal < 0)
    {
        pageStartLocal = 0;
        pageEndLocal = pageStartLocal + (MAX_ROWS*MAX_LINES);
    }
    if (pageEndLocal > nOptions)
    {
        pageEndLocal = nOptions;
        pageStartLocal = pageEndLocal - (MAX_ROWS*MAX_LINES);
    }
    (*pageStart) = pageStartLocal;
    (*pageEnd) = pageEndLocal;
}

void goToLastPage(int* pageStart, int* pageEnd, int nOptions)
{
    (*pageStart) = nOptions - (MAX_ROWS*MAX_LINES);
    (*pageEnd) = nOptions;
}

int getCurrentPage(int cursor,int nOptions)
{
    return ((int)(cursor/(MAX_LINES*MAX_ROWS)) + 1);
}

int getTotalPages(int nOptions)
{
    return ((int)(nOptions/(MAX_LINES*MAX_ROWS)) + 1);
}




FileBlock* menuSelectFile(char* classSelected,char* partSelected,char* fileExt, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, int operation, PatcherState* patcherState)
{

    char* partFolderPrefix = getPartFolderPrefix(partSelected);

    char* classPrefix = getClassPrefixFromMenuOption(classSelected);
    char* partPrefix = getPartPrefixFromMenuOption(partSelected);



    int i = 0;
    long nOptions = 0;
    int cursor = 0;
    int pageStart = 0;
    int pageEnd = MAX_LINES* MAX_ROWS;
    int longestLength = 0;

    int selectAll = 0;
    char input = '\0';


    FileBlock** filesOnMenu = NULL;

    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (shouldDisplayFile(&fileBlocks[i], partFolderPrefix, classPrefix, partPrefix, fileExt))
        {
            nOptions++;
        }
    }

;
    if (nOptions == 0)
    {
        printf("\n No files to display.");
        PAUSE();
    }

    filesOnMenu = (FileBlock**) malloc((nOptions + 1) * sizeof(FileBlock*));

    int currentMenuOption = 0;
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (shouldDisplayFile(&fileBlocks[i], partFolderPrefix, classPrefix, partPrefix, fileExt))
        {
            filesOnMenu[currentMenuOption] = &fileBlocks[i];
            currentMenuOption++;
        }
    }


    readRealNamesFile(filesOnMenu,nOptions);
    recalculateSettings(filesOnMenu,nOptions,pageStart,metaFileInfo);


    while(1)
    {
        if (isWindows10)
        {
            uiMenuSelectFileColor(filesOnMenu, nOptions, cursor, pageStart, pageEnd, longestLength, operation, selectAll, metaFileInfo);
        }
        else
        {
            uiMenuSelectFile( filesOnMenu, nOptions, cursor, pageStart, pageEnd, longestLength, operation, selectAll, metaFileInfo);
        }

        input = getch();

        if (input == ENTER ||  input == 10)
        {
            if (operation == GET_TEXTURES)
            {
                extractTexturesFrom(filesOnMenu[cursor],fileBlocks, metaFileInfo,1);
                PAUSE();
            }
            else
            {
                int skip = 0;
                int filesToPatchCount = 0;
                 for(i = 0; i < metaFileInfo->fileBlocksCount; i++)
                 {
                     if (fileBlocks[i].needPatch == 1)
                     {
                         filesToPatchCount++;
                     }
                 }


                if (filesToPatchCount == 0)
                {
                    printf("\nWarning: No files were selected to patch.");
                    printf("\nMake sure you use SPACE or R to mark the files");
                    printf("\n\nDo you want to proceed or you want to go back and change your settings first?\n");
                    printf("ENTER = Proceed    ESC = Go Back\n");
                    char c = '\0';

                    while(c != ENTER &&  c != 10 && c != ESC)
                    {
                        c = getch();
                    }
                    if (c == ESC)
                    {
                        skip = 1;
                    }
                }

                if (skip == 0)
                {
                    save_paz_browser_patcher_state(fileBlocks,metaFileInfo,operation);

                    if (operation == FIX_HOLES)
                    {
                        generatePartcutdesc(fileBlocks,metaFileInfo,patcherState);
                          printf("\n----------------------------------------------------------------------------\n\n");
                            printf("\nThe system is now going to run MetaInjector.exe to patch your game files.");
                            PAUSE();
                        runMetaInjector();
                        PAUSE();
                    }
                    else if (operation == REMOVE_A_FILE)
                    {
                        printf("\n\nCopy files...\n");
                        copyBlankFiles(fileBlocks,metaFileInfo,"files_to_patch\\");
                        //patchMetaFile(fileBlocks,metaFileInfo->fileBlocksCount,REMOVE_A_FILE,metaFileInfo,1);
                        //filesPatched = countPatchedFiles(fileBlocks,metaFileInfo);

                    }
                    else if (operation == RESTORE_A_FILE)
                    {
                        patchMetaFile(fileBlocks,metaFileInfo->fileBlocksCount,RESTORE_A_FILE,metaFileInfo,1);
                        //filesPatched = countPatchedFiles(fileBlocks,metaFileInfo);
                        PAUSE();
                    }
                    else if (operation == GET_TEXTURES)
                    {

                    }
                 }
            }
        }
        if (operation == GET_TEXTURES)
        {
            if (input == 'L' || input == 'l')
            {
                previewFile(filesOnMenu[cursor],fileBlocks,metaFileInfo,1);
            }
        }
        if (operation != GET_TEXTURES)
        {
            if (input == SPACE)
            {
                filesOnMenu[cursor]->needPatch = !filesOnMenu[cursor]->needPatch;
                if (filesOnMenu[cursor]->needPatch == 0)
                {
                    filesOnMenu[cursor]->changeID = 0;
                }
                else
                {
                    filesOnMenu[cursor]->changeID = 1;
                }
            }

            if (input == 'L' || input == 'l')
            {
                previewFile(filesOnMenu[cursor],fileBlocks,metaFileInfo,1);
            }
             else if (input == 's' || input == 'S')
            {
                int i = 0;
                int atLeastOneToExtract = 0;
                for(i = 0; i < nOptions; i++)
                {
                    if (filesOnMenu[i]->needPatch != 0)
                    {
                        previewFile(filesOnMenu[i],fileBlocks,metaFileInfo,0);
                        atLeastOneToExtract = 1;
                    }
                }
                if (atLeastOneToExtract)
                {
                    system("start patcher_resources\\extracted_files\\");
                }
            }
            else if (input == 'r' || input == 'R')
            {
                char confirmation = '\0';
                printf("\nAre you sure you want to UNMARK all files?\n");
                printf("(Warning: Your file \"%s\" will be deleted)\n\n", PAZ_BROWSER_PATCHER_STATE_FILE_NAME[operation]);
                printf("   ENTER = Proceed    ESC = Go Back\n");
                while(confirmation != ENTER && confirmation != 10 && confirmation != ESC)
                {
                    confirmation = getch();
                }
                if (confirmation == ENTER || confirmation == 10)
                {
                    clearFileBlocks(fileBlocks,metaFileInfo);
                    char* pathToFileToDelete = concatenate("patcher_resources\\", PAZ_BROWSER_PATCHER_STATE_FILE_NAME[operation]);
                    openFile(pathToFileToDelete,"wb");
                    openFile("patcher_resources\\red_markers_partcutdesc.bin","wb");
                    free(pathToFileToDelete);
                    printf("\nAll Files have been Unmarked.");
                    Sleep(1500);
                }
            }

            else if (input == 'c' || input == 'C')
            {
                if (filesOnMenu[cursor]->needPatch == 0)
                {
                    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
                    {
                        if (contains(fileBlocks[i].fileName,removeClassPrefix(filesOnMenu[cursor]->fileName)))
                        {
                            fileBlocks[i].needPatch = 1;
                            fileBlocks[i].changeID = 2;
                        }
                    }
                }
                else if (filesOnMenu[cursor]->needPatch == 1 )
                {
                    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
                    {
                        if (contains(fileBlocks[i].fileName,removeClassPrefix(filesOnMenu[cursor]->fileName)))
                        {
                            fileBlocks[i].needPatch = 0;
                            fileBlocks[i].changeID = 0;
                        }
                    }
                }
            }

            else if (input == 'g' || input == 'G')
            {
                if (filesOnMenu[cursor]->needPatch == 0)
                {
                    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
                    {
                        if (haveTheSameGender(fileBlocks[i].fileName, filesOnMenu[cursor]->fileName) && contains(fileBlocks[i].fileName,removeClassPrefix(filesOnMenu[cursor]->fileName)))
                        {
                            fileBlocks[i].needPatch = 1;
                            if (isAFemaleFile(filesOnMenu[cursor]->fileName))
                            {
                                fileBlocks[i].changeID = 3;
                            }
                            else
                            {
                                fileBlocks[i].changeID = 4;
                            }

                        }
                    }
                }

                else if (filesOnMenu[cursor]->needPatch == 1 )
                {
                    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
                    {
                        if (haveTheSameGender(fileBlocks[i].fileName, filesOnMenu[cursor]->fileName) && contains(fileBlocks[i].fileName,removeClassPrefix(filesOnMenu[cursor]->fileName)))
                        {
                            fileBlocks[i].needPatch = 0;
                            fileBlocks[i].changeID = 0;
                        }
                    }
                }
            }
            else if (input == 'n' || input == 'N')
            {
                displayRealFileNames = !displayRealFileNames;
                recalculateSettings(filesOnMenu,nOptions,pageStart,metaFileInfo);
            }
            else if (input == 'a' || input == 'A')
            {


                for (i = 0; i < nOptions; i++)
                {
                    if (selectAll == 0)
                    {
                        filesOnMenu[i]->needPatch = 1;
                        filesOnMenu[i]->changeID = 1;
                    }
                    else
                    {
                        filesOnMenu[i]->needPatch = 0;
                        filesOnMenu[i]->changeID = 0;
                    }

                }
                 selectAll = !selectAll;
            }
        }
        if (input == ESC)
        {
             resizeWindow(120,30);
            return NULL;
        }
        if (input == PAGE_UP)
        {
            nextPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo);
            cursor = pageStart;
        }
        if (input == PAGE_DOWN)
        {
            previousPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo);
            cursor = pageStart;
        }
        if (input == ARROW_DOWN)
        {
            if( cursor + 1 < nOptions)
            {
                cursor++;
            }
            if (cursor >= pageEnd)
            {
                nextPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo);
            }
        }
        if (input == ARROW_UP)
        {
            if (cursor - 1 >= 0)
            {
                cursor--;
            }
            if (cursor < pageStart)
            {
                previousPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo);
            }
        }
        else if (input == ARROW_RIGHT)
        {
            if (cursor + MAX_LINES < nOptions)
            {
                cursor += MAX_LINES;
            }
            if (cursor >= pageEnd)
            {
                nextPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo);
            }
        }
        if (input == ARROW_LEFT)
        {
            if (cursor - MAX_LINES >= 0)
            {
                cursor -= MAX_LINES;
            }
              if (cursor < pageStart)
            {
                previousPage(&pageStart,&pageEnd,&longestLength,filesOnMenu,nOptions,metaFileInfo);
            }
        }
    }

    resizeWindow(120,30);
    return 0;
}


int recalculateSettings(FileBlock** filesOnMenu, int nOptions, int pageStart,MetaFileInfo* metaFileInfo)
{
        int necessaryWindowWidth = getRowsTotalLength(filesOnMenu,nOptions,pageStart);


        if (!isWindows10)
        {
            necessaryWindowWidth += ((MAX_ROWS-1) * strlen("[ ][ ]"));
        }

        if (necessaryWindowWidth > (120 - 4))
        {
            resizeWindow(necessaryWindowWidth + 4,36);
        }
        else
        {
            resizeWindow(120,36);
        }




        FILE* metaFile = openFile("pad00000.meta","rb");

        /*int i = 0;
        for(i = pageStart; i < nOptions; i++)
        {
            filesOnMenu[i]->isPatched = 0;
            if (isPatched(filesOnMenu[i],metaFileInfo,metaFile))
            {
                filesOnMenu[i]->isPatched = 1;
            }
        }*/

        fclose(metaFile);

        return necessaryWindowWidth;
}

int getRowsTotalLength(FileBlock** filesOnMenu,int nOptions,int pageStart)
{
    int totalLength = 0;
    int i = 0;
    int row = 0;
    for (row = 0; row < MAX_ROWS; row++)
    {
        int rowStartIDX = row * MAX_LINES + pageStart;
        int rowEndIDX = ((row+1) * MAX_LINES)  - 1 + pageStart;
        if (rowEndIDX > nOptions - 1)
        {
            rowEndIDX = nOptions - 1;
        }
        int longestLength = 0;

        for (i = rowStartIDX; i <= rowEndIDX; i++)
        {
            if (filesOnMenu[i]->hasRealName && displayRealFileNames)
            {
                if (longestLength < strlen(filesOnMenu[i]->RealName))
                {
                    longestLength = strlen(filesOnMenu[i]->RealName);
                }
            }
            else
            {
                if (longestLength < strlen(filesOnMenu[i]->fileName))
                {
                    longestLength = strlen(filesOnMenu[i]->fileName);
                }
            }
        }
        totalLength += (1 * strlen("  ->000 - ")) + longestLength;
       // printf("\ntotalLength += %d  = %d", longestLength,totalLength);
    }
    if (totalLength < 97)
    {
        totalLength = 97;
    }
   // printf("\ntotalLength: %d", totalLength);
   // PAUSE();
    return totalLength;
}


void uiMenuSelectFileColor(FileBlock** filesOnMenu,int nOptions, int cursor, int pageStart, int pageEnd,int longestLength, int operation, int selectAll, MetaFileInfo* metaFileInfo)
{
        int i = 0;
        int j = 0;
        int optionToDisplay = 0;

        system("cls");
        printMainHeader();
        if (operation != GET_TEXTURES && operation != BROWSE_GAME_FILES)
        {
            char* operationName = getOperationName(operation);


           // printf("\x1b[31m    Red Font Files \x1b[0m: ");printf("File currently patched in meta file\n");
            printf("\x1b[43m Yellow background \x1b[0m: ");printf("File marked to %s\n",operationName);
            printf("\x1b[41m Red background    \x1b[0m: ") ;printf("File marked to %s across all classes\n",operationName );

            if (isAFemaleFile(filesOnMenu[0]->fileName))
            {
                printf("\x1b[45m Pink background   \x1b[0m: ");printf("File marked to %s across all female classes\n\n",operationName);
            }
            else
            {
                printf("\x1b[46m Cyan background   \x1b[0m: ");printf("File marked to %s across all male classes\n\n",operationName);
            }

            printf("Remember: All the files you mark are saved in: \"\\Paz\\patcher_resources\\");printColor(PAZ_BROWSER_PATCHER_STATE_FILE_NAME[operation], YELLOW);
        }


        printf("\nPage %d - %d", getCurrentPage(cursor,nOptions),getTotalPages(nOptions));
        printf("\n");
        for(i = pageStart; i < nOptions; i++)
        {
            char* fileNameToDisplay = filesOnMenu[i]->fileName;
            if (filesOnMenu[i]->hasRealName && displayRealFileNames)
            {
                fileNameToDisplay = filesOnMenu[i]->RealName;
            }
            if (filesOnMenu[i]->needPatch == 1)
            {
                if (cursor == i)
                {
                    // YELLOW BACKGROUND WITH CURSOR
                    if (filesOnMenu[i]->changeID == 1)
                    {
                        if (filesOnMenu[i]->isPatched)
                        {
                            printf("\x1b[43m\x1b[31m  ->%.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                        else
                        {
                           printf("\x1b[43m  ->%.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }

                    }
                    else if (filesOnMenu[i]->changeID == 2)
                    {
                        if (filesOnMenu[i]->isPatched)
                        {
                            // RED BACKGROUND WITH YELLOW LETTERS with CURSOR
                            printf("\x1b[41m\x1b[91m  ->%.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                        else
                        {
                            printf("\x1b[41m  ->%.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                    }
                    else if (filesOnMenu[i]->changeID == 3)
                    {
                        // PINK BACKGROUND WITH CURSOR
                        if (filesOnMenu[i]->isPatched)
                        {
                            printf("\x1b[45m\x1b[91m  ->%.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                        else
                        {
                            printf("\x1b[45m  ->%.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                    }
                    else if (filesOnMenu[i]->changeID == 4)
                    {
                        // CYAN BACKGROUND WITH CURSOR
                        if (filesOnMenu[i]->isPatched)
                        {
                            printf("\x1b[46m\x1b[31m  ->%.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                        else
                        {
                            printf("\x1b[46m  ->%.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                    }
                }
                else
                {
                    // YELLOW BACKGROUND
                    if (filesOnMenu[i]->changeID == 1)
                    {
                        if (filesOnMenu[i]->isPatched)
                        {
                            printf("\x1b[43m\x1b[31m    %.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                        else
                        {
                           printf("\x1b[43m    %.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }

                    }
                    else if (filesOnMenu[i]->changeID == 2)
                    {
                        if (filesOnMenu[i]->isPatched)
                        {
                            // RED BACKGROUND WITH YELLOW LETTERS
                            printf("\x1b[41m\x1b[91m    %.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                        else
                        {
                            printf("\x1b[41m    %.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                    }
                    else if (filesOnMenu[i]->changeID == 3)
                    {
                        // PINK BACKGROUND
                        if (filesOnMenu[i]->isPatched)
                        {
                            printf("\x1b[45m\x1b[91m    %.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                        else
                        {
                            printf("\x1b[45m    %.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                    }
                    else if (filesOnMenu[i]->changeID == 4)
                    {
                        // CYAN BACKGROUND
                        if (filesOnMenu[i]->isPatched)
                        {
                            printf("\x1b[46m\x1b[31m    %.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                        else
                        {
                            printf("\x1b[46m    %.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                        }
                    }
                }
            }
            else
            {
                if (cursor == i)
                {
                    if (filesOnMenu[i]->isPatched)
                    {
                        // RED FOREGROUND WITH CURSOR
                        printf("  ->\x1b[31m%.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                    }
                    else
                    {
                        printf("  ->%.3d - %s",i+1,fileNameToDisplay);
                    }

                }
                else
                {
                    if (filesOnMenu[i]->isPatched)
                    {
                        // RED FOREGROUND
                        printf("\x1b[31m    %.3d - %s\x1b[0m",i+1,fileNameToDisplay);
                    }
                    else
                    {

                        printf("    %.3d - %s",i+1,fileNameToDisplay);
                    }

                }
            }

            compensateLength(filesOnMenu,nOptions,i);

            for (j = 1; j < MAX_ROWS; j++)
            {
                if (nOptions > (i + (j * MAX_LINES)))
                {
                    optionToDisplay = i + (j * MAX_LINES);

                    fileNameToDisplay = filesOnMenu[optionToDisplay]->fileName;
                    if (filesOnMenu[optionToDisplay]->hasRealName && displayRealFileNames)
                    {
                        fileNameToDisplay = filesOnMenu[optionToDisplay]->RealName;
                    }


                    if (cursor == optionToDisplay)
                    {
                        if (filesOnMenu[optionToDisplay]->needPatch == 1)
                        {
                            if (filesOnMenu[optionToDisplay]->changeID == 1)
                            {
                                if (filesOnMenu[optionToDisplay]->isPatched)
                                {
                                    printf("\x1b[43m\x1b[31m  ->%.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }
                                else
                                {
                                    printf("\x1b[43m  ->%.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }

                            }
                            else if (filesOnMenu[optionToDisplay]->changeID == 2)
                            {
                                if (filesOnMenu[optionToDisplay]->isPatched)
                                {
                                    printf("\x1b[41m\x1b[91m  ->%.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }
                                else
                                {
                                   printf("\x1b[41m  ->%.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }

                            }
                            else if (filesOnMenu[optionToDisplay]->changeID == 3)
                            {
                                if (filesOnMenu[optionToDisplay]->isPatched)
                                {
                                    printf("\x1b[45m\x1b[91m  ->%.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }
                                else
                                {
                                   printf("\x1b[45m  ->%.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }

                            }
                            else if (filesOnMenu[optionToDisplay]->changeID == 4)
                            {
                                if (filesOnMenu[optionToDisplay]->isPatched)
                                {
                                     printf("\x1b[46m\x1b[31m  ->%.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }
                                else
                                {
                                     printf("\x1b[46m  ->%.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }

                            }
                        }
                        else
                        {
                            if (filesOnMenu[optionToDisplay]->isPatched)
                            {
                                  printf("  ->\x1b[31m%.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                            }
                            else
                            {
                                  printf("  ->%.3d - %s",optionToDisplay+1,fileNameToDisplay);
                            }

                        }
                    }
                    else
                    {
                        if (filesOnMenu[optionToDisplay]->needPatch == 1)
                        {
                            if (filesOnMenu[optionToDisplay]->changeID == 1)
                            {
                                if (filesOnMenu[optionToDisplay]->isPatched)
                                {
                                      printf("\x1b[43m\x1b[31m    %.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }
                                else
                                {
                                      printf("\x1b[43m    %.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }

                            }
                            else if (filesOnMenu[optionToDisplay]->changeID == 2)
                            {
                                if (filesOnMenu[optionToDisplay]->isPatched)
                                {
                                      printf("\x1b[41m\x1b[91m    %.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }
                                else
                                {
                                      printf("\x1b[41m    %.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }

                            }
                            else if (filesOnMenu[optionToDisplay]->changeID == 3)
                            {
                                if (filesOnMenu[optionToDisplay]->isPatched)
                                {
                                     printf("\x1b[45m\x1b[91m    %.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }
                                else
                                {
                                     printf("\x1b[45m    %.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }

                            }
                            else if (filesOnMenu[optionToDisplay]->changeID == 4)
                            {
                                if (filesOnMenu[optionToDisplay]->isPatched)
                                {
                                     printf("\x1b[46m\x1b[31m    %.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }
                                else
                                {
                                     printf("\x1b[46m    %.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                                }

                            }
                        }
                        else
                        {
                            if (filesOnMenu[optionToDisplay]->isPatched)
                            {
                                 printf("\x1b[31m    %.3d - %s\x1b[0m",optionToDisplay+1,fileNameToDisplay);
                            }
                            else
                            {
                                 printf("    %.3d - %s",optionToDisplay+1,fileNameToDisplay);
                            }

                        }
                    }

                    if (j < MAX_ROWS - 1)
                    {
                        compensateLength(filesOnMenu,nOptions,optionToDisplay);
                    }
                }
            }
            printf("\n");

            if ((i + 1) >= (pageEnd - ((MAX_ROWS-1)*MAX_LINES)))
            {
                break;
            }
        }

        if (operation == GET_TEXTURES)
        {
            printf("\n     ENTER - Get Textures from File    L - Preview Current File\n\n");
            printf("ARROWS - Change Selected Option");
            printf("\nESC  - Go back\n");
        }
        else
        {
            if (selectAll == 0)
            {
                printf("\n                          SPACE - Mark file             A - Select All from this list\n");
            }
            else
            {
                printf("\n                          SPACE - Mark file             A - Unselect All from this list\n");
            }

            printf("\n       C - Mark this file across all classes      L - Preview Current File S - Preview Selected Files");
            printf("\n       G - Mark this file across the same gender  R - Reset Everything     N - Toggle Real Names/File Names\n\n");
            printf("ARROWS - Change Selected Option       ENTER - Confirm and apply changes          ESC - Go back\n");
        }

}

void uiMenuSelectFile(FileBlock** filesOnMenu,int nOptions, int cursor, int pageStart, int pageEnd, int longestLength, int operation, int selectAll, MetaFileInfo* metaFileInfo)
{
        int i = 0;
        int j = 0;
        int optionToDisplay = 0;


        if (operation != GET_TEXTURES && operation != BROWSE_GAME_FILES)
        {
            char* operationName = getOperationName(operation);


            //printf("[P]: ");printf("File currently patched in meta file\n");
            printf("[M]: ");printf("File marked to %s\n",operationName);
            printf("[C]: ");printf("File marked to %s across all classes\n",operationName );

            if (isAFemaleFile(filesOnMenu[0]->fileName))
            {
                printf("[G]: ");printf("File marked to %s across all female classes\n\n",operationName);
            }
            else
            {
                printf("[G]: ");printf("File marked to %s across all male classes\n\n",operationName);
            }

            printf("Remember: All the files you mark are saved to: \"\\Paz\\patcher_resources\\");printColor(PAZ_BROWSER_PATCHER_STATE_FILE_NAME[operation], YELLOW);
        }
        else
        {
            printMainHeader();
        }


        printf("\nPage %d - %d", getCurrentPage(cursor,nOptions),getTotalPages(nOptions));
        printf("\n");
        for(i = pageStart; i < nOptions; i++)
        {

            char* fileNameToDisplay = filesOnMenu[i]->fileName;
            if (filesOnMenu[i]->hasRealName && displayRealFileNames)
            {
                fileNameToDisplay = filesOnMenu[i]->RealName;
            }


            if (cursor == i)
            {
                printf("  ->%.3d - ",i+1);
            }
            else
            {
                printf("    %.3d - ",i+1);
            }

            if (filesOnMenu[i]->changeID == 1)
            {
                printf("[M]");
            }
            else if (filesOnMenu[i]->changeID == 2)
            {
                printf("[C]");
            }
            else if (filesOnMenu[i]->changeID == 3 || filesOnMenu[i]->changeID == 4)
            {
                printf("[G]");
            }
            else
            {
               printf("   ");
            }

            if (filesOnMenu[i]->isPatched)
            {
                printf("[P]");
            }
            else
            {
                printf("   ");
            }

            if (cursor == i)
            {
                printf(" %s ",fileNameToDisplay);
            }
            else
            {
                printf(" %s ",fileNameToDisplay);
            }


            compensateLength(filesOnMenu,nOptions,i);

            for (j = 1; j < MAX_ROWS; j++)
            {
                if (nOptions > (i + (j * MAX_LINES)))
                {
                    optionToDisplay = i + (j * MAX_LINES);

                    fileNameToDisplay = filesOnMenu[optionToDisplay]->fileName;
                    if (filesOnMenu[optionToDisplay]->hasRealName && displayRealFileNames)
                    {
                        fileNameToDisplay = filesOnMenu[optionToDisplay]->RealName;
                    }

                    if (cursor == optionToDisplay)
                    {
                        printf("  ->%.3d - ",optionToDisplay+1);
                    }
                    else
                    {
                        printf("    %.3d - ",optionToDisplay+1);
                    }

                    if (filesOnMenu[optionToDisplay]->changeID == 1)
                    {
                        printf("[M]");
                    }
                    else if (filesOnMenu[optionToDisplay]->changeID == 2)
                    {
                        printf("[C]");
                    }
                    else if (filesOnMenu[optionToDisplay]->changeID == 3 || filesOnMenu[optionToDisplay]->changeID == 4)
                    {
                        printf("[G]");
                    }
                    else
                    {
                       printf("   ");
                    }

                      if (filesOnMenu[optionToDisplay]->isPatched)
                    {
                        printf("[P]");
                    }
                    else
                    {
                        printf("   ");
                    }

                    if (cursor == optionToDisplay)
                    {
                        printf(" %s",fileNameToDisplay);
                    }
                    else
                    {
                        printf(" %s",fileNameToDisplay);
                    }



                    if (j < MAX_ROWS - 1)
                    {
                        compensateLength(filesOnMenu,nOptions,optionToDisplay);
                    }
                }
            }
            printf("\n");


            if ((i + 1) >= (pageEnd - ((MAX_ROWS-1)*MAX_LINES)))
            {
                break;
            }
        }

        if (operation == GET_TEXTURES)
        {
            printf("\n     ENTER - Get Textures from File    L - Preview Current File\n\n");
            printf("ARROWS - Change Selected Option");
            printf("\nESC  - Go back\n");
        }
        else
        {
            if (selectAll == 0)
            {
                printf("\n                          SPACE - Mark file             A - Select All from this list\n");
            }
            else
            {
                printf("\n                          SPACE - Mark file             A - Unselect All from this list\n");
            }

            printf("\n       C - Mark this file across all classes      L - Preview Current File    S - Preview Selected Files");
            printf("\n       G - Mark this file across the same gender  R - Unselect all files (including other classes)\n\n");
            printf("ARROWS - Change Selected Option       ENTER - Confirm and apply changes          ESC - Go back\n");
        }
}



