#include "../include/patcher.h"

/* **********************************************************************
 *
 *                        RUN PATCHER
 *
 ************************************************************************/
void runPatcher(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo,PatcherState* patcherState)
{
    int filesToPatchCount = 0;

    FileBlock** filesToPatch = markFilesToPatch(fileBlocks, metaFileInfo, patcherState,&filesToPatchCount);

    // QUICK SORT
    //qsort(filesToPatch,filesToPatchCount,sizeof(FileBlock*),compare_middle_digits);


    printf("\n------------- All set -------------\n");
    printf("\n%d file(s) will be patched.\n\n", filesToPatchCount);
    printColor("Resorepless will delete all files in \"files_to_patch\\.resorepless\\\"\n",YELLOW);
    printf("Make sure you don't have nothing important there.\n\n");

    printColor("Do not post screenshots of nude characters in the beauty album or anywhere in the game.\n", RED);
    printColor("This can most likely result in a permanent ban of your account.\n\n", RED);


    printf("ENTER = Proceed    ESC = Go Back\n");
    char c = '\0';

    while(c != ENTER &&  c != 10 && c != ESC)
    {
        c = getch();
    }
    if (c == ESC)
    {
        return;
    }

    printf("\n\nDeleting: files_to_patch\\.resorepless\n");
    system("mkdir files_to_patch\\\\empty & robocopy /mir files_to_patch\\\\empty files_to_patch\\.resorepless >>patcher_resources\\NULL");

    system("rmdir /S /Q files_to_patch\\\\empty");
    remove("patcher_resources\\NULL");
    printf("\n");


    restoreBackup(getLatestBackup());


    copyPatcherResourcesFiles(filesToPatch, filesToPatchCount, patcherState);

    if (bodyPartsSizeChanged(patcherState))
    {
        extractBreastSizePatcherFiles(fileBlocks,metaFileInfo->fileBlocksCount,1);

        printf("\n\nModifying XML files...\n");
        patchBreastSizePatcherFiles(fileBlocks, metaFileInfo->fileBlocksCount, patcherState);

    }


    printf("\n\nCopying nude textures...\n\n");
    copyNudeTextures(patcherState);
    printf("Done.\n");



    //undoLastChanges(0);
    //patchMetaFile(fileBlocks, metaFileInfo->fileBlocksCount, INSTALL, metaFileInfo,0);

    free(filesToPatch);

    clearFileBlocks(fileBlocks,metaFileInfo);
    loadMarkedFiles(fileBlocks,metaFileInfo,FIX_HOLES);
    generatePartcutdesc(fileBlocks,metaFileInfo,patcherState);

    //filesPatched = countPatchedFiles(fileBlocks,metaFileInfo);


    printf("\n\nFinished!\n");
    printf("\n------------------------------------------------------------------------------------------------------------\n");

    printf("\nThe tool is going to run Meta Injector.exe now, as it is required to actually patch the game.\n");
    printf("If you have any extra files you want to add to the game, place them in the \"files_to_patch\" folder now.\n\n");
    printf("On the screen that is going to come up next, select \"Run Injector\"\n");
    printf("\nWhen asked, change the option from:\n\n");
    printColor("     Keep files and folders structures from the \"files_to_patch\" folder. (Recommended)",RED);
    printf("\n\n     to\n\n");
    printColor("  -> Let the program decide where the files should go.", GREEN);
    printf("\n\n");

    PAUSE();

    runMetaInjector();

}


FileBlock** markFilesToPatch(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, PatcherState* patcherState,int* filesToPatchCount)
{
    printf("\nMarking files to patch...\n");
    int i = 0;
    int j = 0;

    FileBlock** filesToPatch = (FileBlock**) malloc(metaFileInfo->fileBlocksCount * sizeof(FileBlock*));

    // Used if the Helmet option is set to "Remove All"
    FileBlock** helmets = NULL;
    int totalHelmets = 0;
    if (patcherState->option[HELMETS] == REMOVE_ALL_HELMETS)
    {
        helmets = getFilteredFileBlocks(fileBlocks,metaFileInfo,"13_hel",&totalHelmets);
    }

    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (strstr(fileBlocks[i].folderName,"under18"))
        {
            continue;
        }
        // Clears the need patch variable in case the user runs the program multiple times
        fileBlocks[i].needPatch = 0;

        // This is the texture of the pac file that every removed armor will load, it's now a blank texture so you can't see it
        if (strcmpi(fileBlocks[i].fileName,"common_decals_road_04_dec.dds") == 0)
        {
            fileBlocks[i].needPatch = 1;
        }

        // Breast Size Patcher
        if (contains(fileBlocks[i].fileName,"customizationboneparamdesc"))
        {
            //fileBlocks[i].needPatch = 1;
        }



        // WEAPON REMOVAL
        if (contains(fileBlocks[i].folderName,"weapon") && contains(fileBlocks[i].folderName,"1_pc") && fileHasExt(fileBlocks[i].fileName,"pac") && !contains(fileBlocks[i].folderName,"0_com") )
        {
            weaponOptionRequirePatch(&fileBlocks[i], patcherState);
        }

        //  Valkyrie Venslar Feather removal
        if (patcherState->option[ARMOR] == REMOVE_ALL_ARMORS && strcmpi(fileBlocks[i].fileName,"pvw_00_feather_0001_hair.dds") == 0)
        {
            fileBlocks[i].needPatch = 1;
        }

        // NPC Clothing Removal
        /*if (patcherState->option[NPC] == REMOVE_NPC_CLOTHING)
        {
            if ((strcmpi(fileBlocks[i].fileName,"nhw_adult_poor_0057_ub_0001.dds") == 0)
            || (strcmpi(fileBlocks[i].fileName,"nhw_fat_poor_0002_ub_0001.dds") == 0))
            {
                fileBlocks[i].needPatch = 1;
            }
        }*/

        if (needToPatchPartcutdesc(patcherState))
        {
            if (strcmp(fileBlocks[i].fileName,"partcutdesc.xml") == 0)
            {
                //fileBlocks[i].needPatch = 1;
            }
        }

        int isAPACFile = fileHasExt(fileBlocks[i].fileName,"pac");
        int isADDSFile = fileHasExt(fileBlocks[i].fileName,"dds");

        // ALL OTHER FILES
        if (fileBlocks[i].fileName[0] == 'p' && (fileBlocks[i].fileName[2] == 'w' || fileBlocks[i].fileName[2] == 'm') && (isAPACFile || isADDSFile))
        {
            if (((patcherState->option[GENDER] == FEMALE_ONLY) && (isAFemaleFile(fileBlocks[i].fileName)))
            ||  ((patcherState->option[GENDER] == MALE_ONLY  ) && (isAMaleFile(fileBlocks[i].fileName)))
            ||   (patcherState->option[GENDER] == BOTH_GENDERS))
            {
                if (canPatchClass(fileBlocks[i].fileName,patcherState))
                {
                    // If it's a .pac file
                    if (isAPACFile)
                    {
                        // SUZU's New 3D Nude Models
                        if (isAFemaleNudeModel(fileBlocks[i].fileName) && patcherState->option[USE_3D_VAGINAS] == ON && patcherState->modAppliesToClass[getClassMacro(fileBlocks[i].fileName)])
                        {
                            fileBlocks[i].needPatch = 1;
                        }



                        // Nude Male Models
                        if (isAMaleNudeModel(fileBlocks[i].fileName) && patcherState->option[USE_PENISES])
                        {
                            if ((patcherState->option[UNDERWEAR] == REMOVE_STARTER_UNDERWEAR) && (isStarter(fileBlocks[i].fileName)))
                            {
                                fileBlocks[i].needPatch = 1;
                            }
                            else if (patcherState->option[UNDERWEAR] == REMOVE_ALL_UNDERWEAR)
                            {
                                fileBlocks[i].needPatch = 1;
                            }
                        }


                        if (contains(fileBlocks[i].folderName,"9_upperbody")
                        || contains(fileBlocks[i].folderName,"14_sho")
                        || contains(fileBlocks[i].folderName,"19_cloak")
                        || contains(fileBlocks[i].folderName,"10_lowerbody")
                        || contains(fileBlocks[i].folderName,"15_underup"))
                        {
                            if (patcherState->option[ARMOR] == REMOVE_ALL_ARMORS)
                            {
                                fileBlocks[i].needPatch = 1;
                            }
                            else if  (patcherState->option[ARMOR] == REMOVE_STARTER_ARMORS)
                            {
                                if (isStarter(fileBlocks[i].fileName))
                                {
                                    if (contains(fileBlocks[i].fileName,"_ub_")
                                      ||contains(fileBlocks[i].fileName,"_sho_")
                                      ||contains(fileBlocks[i].fileName,"_lb_")
                                      ||contains(fileBlocks[i].fileName,"_underup_")
                                    )
                                    {
                                         fileBlocks[i].needPatch = 1;
                                    }
                                }
                            }
                            else if  (patcherState->option[ARMOR] == REMOVE_REGULAR_ARMORS)
                            {
                               if (isRegular(fileBlocks[i].fileName))
                               {
                                   fileBlocks[i].needPatch = 1;
                               }
                            }

                            // Prevents Kibelius armor  from being patch if it's set to transparent
                            if (patcherState->transparentKibelius != 0)
                            {
                                if (contains(fileBlocks[i].fileName,"_00_ub_0034"))
                                {
                                   fileBlocks[i].needPatch = 0;
                                }
                            }


                            if (contains(fileBlocks[i].fileName,"_00_sho_0034"))
                            {
                                if (patcherState->kibeliusWings == KEEP_WINGS)
                                {
                                    fileBlocks[i].needPatch = 0;
                                }
                                else
                                {
                                    fileBlocks[i].needPatch = 1;
                                }
                            }
                        }

                        else if (contains(fileBlocks[i].folderName,"event_costume"))
                        {
                            if  (patcherState->option[LIFE_SKILL_ARMORS] == REMOVE_ALL_LIFE_SKILL)
                            {
                                fileBlocks[i].needPatch = 1;
                            }
                        }


                        else if (contains(fileBlocks[i].folderName,"38_underwear"))
                        {

                            if (patcherState->option[UNDERWEAR] == REMOVE_ALL_UNDERWEAR)
                            {
                                fileBlocks[i].needPatch = 1;
                            }
                            else if  (patcherState->option[UNDERWEAR] == REMOVE_STARTER_UNDERWEAR)
                            {
                                if (contains(fileBlocks[i].fileName,"_00_uw_0001"))
                                {
                                    fileBlocks[i].needPatch = 1;
                                }
                            }

                            if (patcherState->option[STOCKINGS] == KEEP_ALL_STOCKINGS || patcherState->option[STOCKINGS] == REMOVE_STARTER_UNDERWEAR)
                            {
                                if (isAFemaleFile(fileBlocks[i].fileName))
                                {
                                    fileBlocks[i].needPatch = 0;
                                }
                            }
                        }


                        // CENSORSHIP REMOVAL
                        if (patcherState->option[CENSORSHIP] >= MINIMAL_CENSORSHIP_REMOVAL)
                        {
                            if (isPantieFile(fileBlocks[i].fileName))
                            {
                                fileBlocks[i].needPatch = 1;
                            }
                        }

                        if (patcherState->option[CENSORSHIP] == HIGH_CENSORSHIP_REMOVAL)
                        {
                            if (isPantsFile(fileBlocks[i].fileName))
                            {
                                fileBlocks[i].needPatch = 1;
                            }
                        }


                        if (contains(fileBlocks[i].folderName,"11_hand"))
                        {
                            if (patcherState->option[GLOVES] == REMOVE_ALL_GLOVES)
                            {
                                fileBlocks[i].needPatch = 1;
                            }
                            else if  (patcherState->option[GLOVES] == REMOVE_STARTER_GLOVES)
                            {
                                if (isStarter(fileBlocks[i].fileName))
                                {
                                    if (contains(fileBlocks[i].fileName,"_hand_"))
                                    {
                                        fileBlocks[i].needPatch = 1;
                                    }
                                }
                            }
                            else if  (patcherState->option[GLOVES] == REMOVE_REGULAR_GLOVES)
                            {
                               if (isRegular(fileBlocks[i].fileName))
                               {
                                   fileBlocks[i].needPatch = 1;
                               }
                            }

                            if (contains(fileBlocks[i].fileName,"_10_hand_0003.pac"))
                            {
                                fileBlocks[i].needPatch = 0;
                            }
                        }


                        if (contains(fileBlocks[i].folderName,"12_foot"))
                        {
                            if (patcherState->option[BOOTS] == REMOVE_ALL_BOOTS)
                            {
                                 fileBlocks[i].needPatch = 1;
                            }
                            else if  (patcherState->option[BOOTS] == REMOVE_STARTER_BOOTS)
                            {
                                if (isStarter(fileBlocks[i].fileName))
                                {
                                    if (contains(fileBlocks[i].fileName,"_foot_"))
                                    {
                                        fileBlocks[i].needPatch = 1;
                                    }
                                }
                            }
                            else if  (patcherState->option[BOOTS] == REMOVE_REGULAR_BOOTS)
                            {
                               if (isRegular(fileBlocks[i].fileName))
                               {
                                   fileBlocks[i].needPatch = 1;
                               }
                            }

                            if (contains(fileBlocks[i].fileName,"_10_foot_0003.pac"))
                            {
                                fileBlocks[i].needPatch = 0;
                            }
                        }

                        if (contains(fileBlocks[i].folderName,"13_hel"))
                        {
                            if (patcherState->option[HELMETS] == REMOVE_ALL_HELMETS)
                            {
                                if (!contains(fileBlocks[i].fileName,"haircut"))
                                {
                                    if(isAFemaleFile(fileBlocks[i].fileName))
                                    {
                                        if (!contains(fileBlocks[i].fileName,"_00_hel_0052")
                                        && !contains(fileBlocks[i].fileName,"_10_hel_0008")
                                        && !contains(fileBlocks[i].fileName,"_00_hel_0072")
                                        && !contains(fileBlocks[i].fileName,"_00_hel_0083")
                                        && !contains(fileBlocks[i].fileName,"_00_hel_0053")
                                        && !contains(fileBlocks[i].fileName,"_00_hel_0037")
                                        && !contains(fileBlocks[i].fileName,"_ew_hel_0033")
                                        && !contains(fileBlocks[i].fileName,"_99_")
                                        && !contains(fileBlocks[i].fileName,"_10_hel_0004")
                                        && !contains(fileBlocks[i].folderName,"event_costume_hel")
                                        && !strstr(fileBlocks[i].fileName,"op"))
                                        {
                                            fileBlocks[i].needPatch = 1;

                                            // Prevents some missing heads when removing helmet.
                                            // If the helmet has the sufix _op, it means that you can open and close the helmet
                                            // Most of those helmets covers the full head, which will cause the head to be missing if we remove it
                                            // So we look though all the files marked to patch, and if any of them have the _op, we unmark them
                                            // as well all the other files with the same numbers. Ex: phw_00_hel_0033_op.pac, we unmark also phw_00_hel_0033.pac
                                            char* currentHelTwoDigits = getTwoDigits(fileBlocks[i].fileName);
                                            char* currentHelFourDigits = getFourDigits(fileBlocks[i].fileName);

                                            for (j = 0; j < totalHelmets; j++)
                                            {
                                                char* comparedHelTwoDigits = getTwoDigits(helmets[j]->fileName);
                                                char* comparedHelFourDigits = getFourDigits(helmets[j]->fileName);

                                                if ((strcmp(currentHelTwoDigits,comparedHelTwoDigits) == 0 ) && (strcmp(currentHelFourDigits,comparedHelFourDigits) == 0 ))
                                                {
                                                    if (contains(helmets[j]->fileName,"op"))
                                                    {
                                                        fileBlocks[i].needPatch = 0;
                                                        break;
                                                    }
                                                }
                                                free(comparedHelTwoDigits);
                                                free(comparedHelFourDigits);
                                            }
                                            free(currentHelTwoDigits);
                                            free(currentHelFourDigits);
                                        }
                                    }
                                }
                            }
                        }
                        if (contains(fileBlocks[i].folderName,"Hair"))
                        {
                            if (patcherState->option[HELMETS] == REMOVE_ALL_HELMETS)
                            {
                                if (contains(fileBlocks[i].fileName,"Hair_Hel"))
                                {
                                    fileBlocks[i].needPatch = 0;
                                }
                            }
                        }

                        if (isATreeDummerFile(fileBlocks[i].fileName))
                        {
                            fileBlocks[i].needPatch = 0;
                        }
                    }

                    /****************************************************************************
                     *                            TEXTURE PATCHING                              *
                     ****************************************************************************/
                    if (isADDSFile)
                    {
                        if (patcherState->option[ARMOR] == REMOVE_ALL_ARMORS && patcherState->modAppliesToClass[RAN])
                        {
                                if (contains(fileBlocks[i].fileName,"psw_00_ub_0001") || contains(fileBlocks[i].fileName,"psw_00_lb_0001") || contains(fileBlocks[i].fileName,"psw_00_sho_0001") || contains(fileBlocks[i].fileName,"psw_00_cloak_0001"))
                                {
                                    fileBlocks[i].needPatch = 1;
                                }
                        }
                        if (patcherState->option[GLOVES] == REMOVE_ALL_GLOVES && patcherState->modAppliesToClass[RAN])
                        {
                                if (contains(fileBlocks[i].fileName,"psw_00_hand_0001"))
                                {
                                    fileBlocks[i].needPatch = 1;
                                }
                        }
                         if (patcherState->option[BOOTS] == REMOVE_ALL_BOOTS && patcherState->modAppliesToClass[RAN])
                        {
                                if (contains(fileBlocks[i].fileName,"psw_00_foot_0001"))
                                {
                                    fileBlocks[i].needPatch = 1;
                                }
                        }
                        if ((patcherState->option[CENSORSHIP] >= MEDIUM_CENSORSHIP_REMOVAL))
                        {
                            if (isCensorshipTextureFile(fileBlocks[i].fileName))
                            {
                                fileBlocks[i].needPatch = 1;
                            }
                        }
                        if ((patcherState->option[CENSORSHIP] == MINIMAL_CENSORSHIP_REMOVAL))
                        {
                            if ((strcmpi(fileBlocks[i].fileName,"pbw_00_lb_0018.dds") == 0)
                            || (strcmpi(fileBlocks[i].fileName,"pbw_00_ub_0054.dds") == 0)
                            || (strcmpi(fileBlocks[i].fileName,"pbw_00_ub_0054_dec.dds") == 0))
                            {
                                fileBlocks[i].needPatch = 1;
                            }
                        }


                        if (/**!*/!(patcherState->option[UNDERWEAR] == REMOVE_ALL_UNDERWEAR && patcherState->option[STOCKINGS] == REMOVE_ALL_STOCKINGS) || contains(fileBlocks[i].fileName,"_ao.dds"))
                        {
                            if (contains(fileBlocks[i].folderName,"texture"))
                            {
                                if (contains(fileBlocks[i].fileName,"_uw_") || isStockingTexture(fileBlocks[i].fileName) || contains(fileBlocks[i].fileName,"_99_ub_"))
                                {
                                    if(!contains(fileBlocks[i].fileName,"_n.dds")
                                       && !contains(fileBlocks[i].fileName,"_sp.dds")
                                       && !contains(fileBlocks[i].fileName,"_m.dds")
                                       && !contains(fileBlocks[i].fileName,"_st.dds")
                                       && !contains(fileBlocks[i].fileName,"_fur"))
                                    {

                                        // UPPER BODY UNDERWEAR PATCH
                                        if (patcherState->option[UNDERWEAR] == REMOVE_ALL_UNDERWEAR)
                                        {
                                            if (isStockingTexture(fileBlocks[i].fileName) != 1)
                                            {
                                                fileBlocks[i].needPatch = 1;
                                            }

                                        }
                                         if (patcherState->option[UNDERWEAR] == REMOVE_STARTER_UNDERWEAR)
                                        {
                                            j = 0;
                                            while(starterUnderwearTextures[j] != NULL)
                                            {
                                                if (strcmpi(fileBlocks[i].fileName,starterUnderwearTextures[j]) == 0)
                                                {
                                                    fileBlocks[i].needPatch = 1;
                                                }
                                                j++;
                                            }
                                        }

                                        //STOCKINGS PATCH
                                        if (patcherState->option[STOCKINGS] == REMOVE_ALL_STOCKINGS)
                                        {
                                            if (isStockingTexture(fileBlocks[i].fileName))
                                            {
                                                // Patch the upper body part
                                                fileBlocks[i].needPatch = 1;
                                            }
                                        }
                                         if (patcherState->option[STOCKINGS] == KEEP_ALL_STOCKINGS)
                                        {
                                            if (isStockingTexture(fileBlocks[i].fileName))
                                            {
                                                fileBlocks[i].needPatch = 0;
                                            }
                                        }
                                         if (patcherState->option[STOCKINGS] == REMOVE_STARTER_STOCKINGS)
                                        {
                                            j = 0;
                                            while(starterStockings[j] != NULL)
                                            {
                                                if (contains(fileBlocks[i].fileName,starterStockings[j]))
                                                {
                                                    fileBlocks[i].needPatch = 1;
                                                }
                                                j++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


              // Always patch AO files
        if (contains(fileBlocks[i].fileName,"_ao.dds"))
        {
            if (contains(fileBlocks[i].fileName,"_uw_") || contains(fileBlocks[i].fileName,"_99_ub_"))
            {
                if (fileBlocks[i].size > 1000)
                {
                    fileBlocks[i].needPatch = 1;
                }
                else
                {
                    fileBlocks[i].needPatch = 0;
                }
            }
        }
        // Forces the nude textures to patch no matter what
        if (isAFemaleNudeTexture(fileBlocks[i].fileName,patcherState->option[USE_3D_VAGINAS]))
        {
            //fileBlocks[i].needPatch = 1;
        }
        if (patcherState->option[USE_PENISES] && patcherState->option[GENDER] != FEMALE_ONLY)
        {
            if (isAMaleNudeTexture(fileBlocks[i].fileName))
            {
                fileBlocks[i].needPatch = 1;
            }
        }

         // Transparent Kibelius
        if (patcherState->transparentKibelius != 0)
        {
            if (patcherState->transparentKibelius == NAKED_WITH_WINGS)
            {
                if (contains(fileBlocks[i].fileName,"*_00_ub_0034*") && fileHasExt(fileBlocks[i].fileName,"pac"))
                {
                     fileBlocks[i].needPatch = 1;
                }
            }
            else  if ((strcmpi(fileBlocks[i].fileName,"pew_00_ub_0034_hair.dds") == 0) || (strcmpi(fileBlocks[i].fileName,"pew_00_ub_0034_01_hair.dds") == 0) || (strcmpi(fileBlocks[i].fileName,"pew_00_ub_0034_dm_hair.dds") == 0) || (strcmpi(fileBlocks[i].fileName,"pew_00_ub_0034_01_dm_hair.dds") == 0))
            {
                 fileBlocks[i].needPatch = 1;
            }
        }

        // Adds the file block to the files to patch array, if it reaches this point with needPatch == 1
        if (fileBlocks[i].needPatch == 1)
        {
             filesToPatch[(*filesToPatchCount)] = &fileBlocks[i];
             (*filesToPatchCount)++;
        }
    } // EOF

    if (patcherState->option[HELMETS] == REMOVE_ALL_HELMETS)
    {
        free(helmets);
    }

    printf("Done.\n");
    return filesToPatch;
}

void patchMetaFile(FileBlock* filesToPatch, int filesToPatchCount, int operation, MetaFileInfo* metaFileInfo,int echo)
{

}

void copyPatcherResourcesFiles(FileBlock** filesToPatch, int filesToPatchCount, PatcherState* patcherState)
{

    printf("\nCopying Files...\n");
    int i = 0;
    char* root = resoreplessFolder();

    char* old_class_prefix = (char*) malloc(1);
    old_class_prefix[0] = '\0';
    char* newclass_prefix = NULL;

    int ECHO = 0;


    for (i = 0; i < filesToPatchCount; i++)
    {
        //printFileBeingCopied(filesToPatch[i], filesToPatchCount, &newclass_prefix, &old_class_prefix, i);
        printf("(%d/%d) - %s",i+1,filesToPatchCount,filesToPatch[i]->fileName);

        char* fileDestination = concatenate(root,filesToPatch[i]->folderName);

        // PAC FILE COPY
        if (fileHasExt(filesToPatch[i]->fileName,"pac"))
        {
            if (isAFemaleNudeModel(filesToPatch[i]->fileName))
            {
                 if (patcherState->option[USE_3D_VAGINAS])
                {
                    copyFileTo(filesToPatch[i]->fileName,filesToPatch[i]->fileName,"patcher_resources\\models\\female\\",fileDestination,ECHO);
                }
                if (patcherState->option[USE_3D_VAGINAS] == OFF)
                {
                    printf(" bla ");
                    //copyFileTo(filesToPatch[i]->fileName,filesToPatch[i]->fileName,"patcher_resources\\models\\female\\original\\",fileDestination,ECHO);
                }
            }
            // If it's a male underwear model
            else if (isAMaleNudeModel(filesToPatch[i]->fileName))
            {
                int j = 0;
                // Goes through all the nude male model .pac files listed in the nudeMaleModels array
                for (j = 0; j < TOTAL_MALE_CLASSES; j++)
                {
                    // If they have the same class, like phm_00_uw_0001.pac (from filesToPatch) and phm_00_nude_0001.pac (from nudeMaleModels)
                    if (haveSameClass(filesToPatch[i]->fileName,nudeMaleModels[j]))
                    {
                        if (patcherState->penisType[getPenisMacro(filesToPatch[i]->fileName)] == NORMAL)
                        {
                            /* Copies the file with the same class from the patcher_resources\ folder to the right destination, renaming it to an underwear pac file
                             Ex: COPY phm_00_nude_0001.pac from patcher_resources\models\male\normal\ to Black Desert Online\character\model\1_pc\1_phm\nude\
                             rename phm_00_nude_0001.pac to phm_00_uw_0001.pac
                             */
                            copyFileTo(nudeMaleModels[j],filesToPatch[i]->fileName,"patcher_resources\\models\\male\\normal\\",fileDestination,ECHO);
                        }
                        else if (patcherState->penisType[getPenisMacro(filesToPatch[i]->fileName)] == HARD)
                        {
                            copyFileTo(nudeMaleModels[j],filesToPatch[i]->fileName,"patcher_resources\\models\\male\\hard\\",fileDestination,ECHO);
                        }
                    }
                }
            }
            else
            {
                // Replace every pac file marked to patch, with a ground model, which we removed the texture, so it's transparent
                copyFileTo("t0072_pumpkin_ground_0001.pac",filesToPatch[i]->fileName,"patcher_resources/models/",fileDestination,ECHO);
            }

        }
        // DDS FILE COPY
        else if (fileHasExt(filesToPatch[i]->fileName,"dds"))
        {
            // If it's the Transparent Kibelius textures
            if ((strcmpi(filesToPatch[i]->fileName,"pew_00_ub_0034_hair.dds") == 0)
                || (strcmpi(filesToPatch[i]->fileName,"pew_00_ub_0034_01_hair.dds") == 0)
                || (strcmpi(filesToPatch[i]->fileName,"pew_00_ub_0034_dm_hair.dds") == 0)
                || (strcmpi(filesToPatch[i]->fileName,"pew_00_ub_0034_01_dm_hair.dds") == 0))
            {

                    if (patcherState->transparentKibelius == TRANSPARENT)
                    {
                         copyFileTo(filesToPatch[i]->fileName,filesToPatch[i]->fileName,"patcher_resources\\texture\\kibelius\\transparent\\",fileDestination,1);
                    }
                    else if (patcherState->transparentKibelius == MORE_TRANSPARENT)
                    {
                         copyFileTo(filesToPatch[i]->fileName,filesToPatch[i]->fileName,"patcher_resources\\texture\\kibelius\\more_transparent\\",fileDestination,1);
                    }
                    else
                    {
                        printf(" - Shouldn't get here");
                    }

            }
            // NPC Clothing Removal Files
            else if ((strcmpi(filesToPatch[i]->fileName,"nhw_adult_poor_0057_ub_0001.dds") == 0)
            || (strcmpi(filesToPatch[i]->fileName,"nhw_fat_poor_0002_ub_0001.dds") == 0))
            {
                /*if (patcherState->option[NPC] == REMOVE_NPC_CLOTHING)
                {
                    copyFileTo(filesToPatch[i]->fileName,filesToPatch[i]->fileName,"patcher_resources\\texture\\",fileDestination,ECHO);
                }*/
            }
            else // Regular DDS copy
            {
                if (!contains(filesToPatch[i]->fileName,"_ao.dds"))
                {
                    if (filesToPatch[i]->size < 1000000)
                    {
                        copyFileTo("blank_dxt1.dds",filesToPatch[i]->fileName,"patcher_resources/texture/",fileDestination,ECHO);
                    }
                    else
                    {
                        copyFileTo("blank_dxt5.dds",filesToPatch[i]->fileName,"patcher_resources/texture/",fileDestination,ECHO);
                    }
                }
                else
                {
                    if (strcmpi(filesToPatch[i]->fileName,"pdw_00_uw_0001_ao.dds") == 0)
                    {
                        copyFileTo("pdw_00_uw_0001_ao.dds",filesToPatch[i]->fileName,"patcher_resources/texture/",fileDestination,ECHO);
                    }
                    else if (filesToPatch[i]->size >= 600000)
                    {
                        copyFileTo("blank_ao_683.dds",filesToPatch[i]->fileName,"patcher_resources/texture/",fileDestination,ECHO);
                    }
                    else if (filesToPatch[i]->size >= 170000 && filesToPatch[i]->size < 600000)
                    {
                        copyFileTo("blank_ao_171.dds",filesToPatch[i]->fileName,"patcher_resources/texture/",fileDestination,ECHO);
                    }
                     else if (filesToPatch[i]->size >= 43000 && filesToPatch[i]->size < 170000)
                    {
                        copyFileTo("blank_ao_43.dds",filesToPatch[i]->fileName,"patcher_resources/texture/",fileDestination,ECHO);
                    }
                    else
                    {
                        printColor("        _ao file size mismatch\n",YELLOW);
                        copyFileTo("blank_ao_171.dds",filesToPatch[i]->fileName,"patcher_resources/texture/",fileDestination,ECHO);
                    }
                }
            }
        }

        free(fileDestination);




    }
    if (displayedErrorOnce)
    {
        printf("\nSome xml files have failed to be extracted.");
        printf("\nThis only means that the MIN/MAX size of the body sliders will remain unaltered.");
        PAUSE();
    }

    printf("\nCopying censorship removal files\n");
    char* censorshipRemovalTextureFolder = "patcher_resources\\texture\\censorship_removal_files\\";
    char* characterTextureFolder =   concatenate(root,"character\\texture\\");
    if (patcherState->option[CENSORSHIP] == MINIMAL_CENSORSHIP_REMOVAL)
    {
        systemCopy("pbw_00_lb_0018.dds",censorshipRemovalTextureFolder,characterTextureFolder);
        systemCopy("pbw_00_ub_0054.dds",censorshipRemovalTextureFolder,characterTextureFolder);
        systemCopy("pbw_00_ub_0054_dec.dds",censorshipRemovalTextureFolder,characterTextureFolder);
    }
    else if (patcherState->option[CENSORSHIP] >= MEDIUM_CENSORSHIP_REMOVAL)
    {
        for (i = 0; i < TOTAL_CENSORSHIP_FILES; i++)
        {
            systemCopy(censorshipTextureFiles[i],censorshipRemovalTextureFolder,characterTextureFolder);
        }
    }

    free(newclass_prefix);
    free(old_class_prefix);
    free(root);
    free(characterTextureFolder);
}


void printFileBeingCopied(FileBlock* fileToPatch,long filesToPatchCount, char** newclass_prefix, char** old_class_prefix, int i)
{
    (*newclass_prefix) = getClassPrefix(fileToPatch->fileName);

    if (strcmp((*newclass_prefix),(*old_class_prefix)) != 0)
    {
        // OUTPUT
        printf("\n********************************************");

        if (contains(fileToPatch->fileName,"_ao.dds"))
        {
            printf("\n%s Area Occlusion Textures",getClassAsString(fileToPatch));
        }
        else if (contains(fileToPatch->fileName,"nhw_"))
        {
            printf("\nNPC Textures");
        }
        else if (fileHasExt(fileToPatch->fileName,"dds"))
        {
             printf("\n%s %s Textures", getClassAsString(fileToPatch) , getArmorPartAsString(fileToPatch));
        }
        else
        {
            printf("\n%s %s", getClassAsString(fileToPatch), getArmorPartAsString(fileToPatch));
        }

        free((*old_class_prefix));
        printf("\n********************************************\n");
        (*old_class_prefix) = (char*) malloc(strlen((*newclass_prefix)) + 1);
        strcpy((*old_class_prefix),(*newclass_prefix));
    }

    printf("Creating: (%d/%ld) - %s",i+1,filesToPatchCount,fileToPatch->fileName);
    //printf(" (%d)",  fileToPatch->changeID);
}


void printLatestFolder(char* pathToFile,char* fileName)
{
    char* latestFolder = getLatestFolder(pathToFile);
    if (strcmpi(latestFolder,"texture\\") == 0)
    {
        strcpy(latestFolder,"shaved\\");
    }

    char* scrFullPath = concatenate(latestFolder,fileName);

    int l = 0;
    int diff = 0;
    if (strlen(scrFullPath) < 39)
    {
        diff = 39 - strlen(scrFullPath);
    }
    for (l = 0; l < diff; l++)
    {
        printf(" ");
    }
    printf("%s", latestFolder);
    free(scrFullPath);
    free(latestFolder);
}

void patchPubicHair(char* srcTextureName,char* targetTextureName,PatcherState* patcherState, char* characterTextureFolder)
{
    int PUBIC_HAIR_IDX = getPubicHairMacro(targetTextureName);
    char* currentClassPubicHairFolder = pubicHairFolder[patcherState->pubicHair[PUBIC_HAIR_IDX]];

    char* pathToPubicHairFile = concatenate(currentClassPubicHairFolder,srcTextureName);
    if (patcherState->option[USE_3D_VAGINAS] == OFF && patcherState->pubicHair[PUBIC_HAIR_IDX] == SHAVED)
    {
        reconstructDDS("patcher_resources\\texture\\pubic_hair\\shaved\\",srcTextureName,characterTextureFolder,targetTextureName);
    }
    else if (fileExists(pathToPubicHairFile))
    {
        systemCopyAndRename(srcTextureName,targetTextureName,currentClassPubicHairFolder,characterTextureFolder);
    }
    else
    {
        if (patcherState->pubicHair[PUBIC_HAIR_IDX] != SHAVED)
        {
            reconstructDDS(currentClassPubicHairFolder,srcTextureName,characterTextureFolder,targetTextureName);
        }
    }
}

void copyNudeTextures(PatcherState* patcherState)
{
    int i = 0;
    char* root = resoreplessFolder();
    char* filesToPatchResoreplessTextureFolder = "files_to_patch\\.resorepless\\character\\texture\\";
    char* characterTextureFolder = concatenate(bdoRootFolder(),"character\\texture\\");
    createPath(filesToPatchResoreplessTextureFolder);
    createPath(characterTextureFolder);

    free(root);

    printf("\nMale Nude Textures:\n");
    for (i = 0; i < TOTAL_NUDE_MALE_TEXTURES; i++)
    {

        systemCopy(nudeMaleTextures[i],"patcher_resources\\texture\\",filesToPatchResoreplessTextureFolder);

        char* destPath = concatenate(filesToPatchResoreplessTextureFolder,nudeMaleTextures[i]);

        if (!fileExists(destPath))
        {
            printColor("\nFailed to copy texture", RED);printf(" %s\n", nudeFemaleTextures[i]);
            printf("from: \"patcher_resources\\texture\\\"\n");
            printf("to  : \"%s\"\n\n", filesToPatchResoreplessTextureFolder);
            printf("Make sure you have placed the texture in the correct location, or manually copy the texture to the destination.\n");
            printf("Warning: If you don't do anything, you will have floating heads with no body.");
            PAUSE();
        }
        free(destPath);
    }

    printf("\nFemale Nude Textures:\n");
    for (i = 0; i < VALKYRIE_PUBIC_HAIR; i++)
    {
        createPath(filesToPatchResoreplessTextureFolder);

        systemCopy(nudeFemaleTextures[i],"patcher_resources\\texture\\",filesToPatchResoreplessTextureFolder);

        char* destPath = concatenate(filesToPatchResoreplessTextureFolder,nudeFemaleTextures[i]);
        if (!fileExists(destPath))
        {
            printColor("\nFailed to copy texture", RED);printf(" %s\n", nudeFemaleTextures[i]);
            printf("from: \"patcher_resources\\texture\\\"\n");
            printf("to  : \"%s\"\n\n", filesToPatchResoreplessTextureFolder);
            printf("Make sure you have placed the texture in the correct location, or manually copy the texture to the destination.\n");
            printf("Warning: If you don't do anything, you will have floating heads with no body.");
            PAUSE();

        }
        free(destPath);
    }

   /* printf("\n");
    printf("\nValkyrie,Maehwa, Kunoichi and Mystic Nude Textures:\n");
    int j = 0;
    for (j = VALKYRIE_PUBIC_HAIR; j <= MYSTIC_PUBIC_HAIR; j++)
    {
        // Copies
        systemCopyAndRename("phw_01_nude_0001.dds",nudeFemaleTextures[j],filesToPatchResoreplessTextureFolder,characterTextureFolder);
    }
    printf("\n");*/
    printf("\nApplying Pubic Hair:\n");
    for (i = 0; i < TOTAL_FEMALE_CLASSES; i++)
    {
        char* srcTextureName    = NULL;
        char* targetTextureName = nudeFemaleTextures[i];

        if (i > SORCERESS_PUBIC_HAIR)
        {
            /*srcTextureName = nudeFemaleTextures[SORCERESS_PUBIC_HAIR];
            patchPubicHair(srcTextureName,targetTextureName,patcherState, characterTextureFolder);*/
        }
        else
        {
            srcTextureName = targetTextureName;
            patchPubicHair(srcTextureName,targetTextureName,patcherState, filesToPatchResoreplessTextureFolder);
        }
    }

    if (patcherState->option[USE_3D_VAGINAS] == ON) {
        printf("\n");
        printf("\nFemale Nude Normal Maps:\n");
        for (i = 0; i < TOTAL_FEMALE_NORMAL_MAPS_TEXTURES; i++)
        {
                systemCopy(nudeFemaleNormalMaps[i],"patcher_resources\\texture\\",filesToPatchResoreplessTextureFolder);

                char* destPath = concatenate(filesToPatchResoreplessTextureFolder,nudeFemaleNormalMaps[i]);
                if (!fileExists(destPath))
                {
                    printColor("\nFailed to copy texture", RED);printf(" %s\n", nudeFemaleNormalMaps[i]);
                    printf("from: \"patcher_resources\\texture\\\"\n");
                    printf("to  : \"%s\"\n\n", filesToPatchResoreplessTextureFolder);
                    printf("Make sure you have placed the texture in the correct location, or manually copy the texture to the destination.\n");
                    printf("Warning: If you don't do anything, you will have floating heads with no body.");
                    PAUSE();
                }
                free(destPath);
        }
        printf("\n");
    }


   /* printf("\n");
    printf("\nValkyrie,Maehwa, Kunoichi and Mystic Normal maps, Specular maps, etc:\n");
    int k = 0;
    for(k = 0; k < TOTAL_TEXTURE_VARIATIONS; k++)
    {
        for (j = VALKYRIE_PUBIC_HAIR; j <= MYSTIC_PUBIC_HAIR; j++)
        {
            char* thisClassSecundaryTextureName = (char*) malloc(strlen(sorceressSecundaryNudeTextures[k]));
            strcpy(thisClassSecundaryTextureName,sorceressSecundaryNudeTextures[k]);

            // Replaces the 'h' in 'phw' with stuff like 'v' from pvw or 'k' from 'pkw'
            thisClassSecundaryTextureName[1] = nudeFemaleTextures[j][1];

            systemCopyAndRename(sorceressSecundaryNudeTextures[k],thisClassSecundaryTextureName,"patcher_resources\\texture\\",characterTextureFolder);

            free(thisClassSecundaryTextureName);
        }
        printf("\n");
    }*/
    free(characterTextureFolder);
}



// Checks if the files in the patcher_resources folder can be found in the File Blocks
/*void validatePatcherResourcesFiles(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo)
{
    long modelFilesCount = 0;
    FileBlock* modelFiles = getAllFiles("patcher_resources\\models\\","*", &modelFilesCount);
    long textureFilesCount = 0;
    FileBlock* textureFiles = getAllFiles("patcher_resources\\texture\\","*", &textureFilesCount);

    int i = 0;
    int j = 0;
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        for (j = 0; j < modelFilesCount; j++)
        {
            if (strcmpi(fileBlocks[i].fileName,modelFiles[j].fileName) == 0)
            {
                modelFiles[j].needPatch = 1;
            }
        }
        for (j = 0; j < textureFilesCount; j++)
        {
            if (strcmpi(fileBlocks[i].fileName,textureFiles[j].fileName) == 0)
            {
                textureFiles[j].needPatch = 1;
            }
        }
    }

    for (j = 0; j < modelFilesCount; j++)
    {
        if (modelFiles[j].needPatch != 1)
        {
            printf("\nNot found: %s", modelFiles[j].fileName);
        }
    }
    for (j = 0; j < textureFilesCount; j++)
    {
        if (textureFiles[j].needPatch != 1)
        {
            if (strcmpi(textureFiles[j].fileName,"blank_dxt1.dds") != 0
            && strcmpi(textureFiles[j].fileName,"blank_dxt5.dds") != 0
            && strcmpi(textureFiles[j].fileName,"blank_ao_683.dds") != 0
            && strcmpi(textureFiles[j].fileName,"blank_ao_171.dds") != 0
            && strcmpi(textureFiles[j].fileName,"blank_ao_43.dds") != 0
                && strcmpi(textureFiles[j].fileName,"preview.jpg") != 0)
            {
               printf("\nNot found: %s", textureFiles[j].fileName);
            }
        }
    }
    printf("\n");

    for (j = 0; j < modelFilesCount; j++)
    {
        free(modelFiles[j].fileName);
        free(modelFiles[j].originalPath);
    }
    for (j = 0; j < textureFilesCount; j++)
    {
        free(textureFiles[j].fileName);
        free(textureFiles[j].originalPath);
    }
    free(modelFiles);
    free(textureFiles);
}*/



void weaponOptionRequirePatch(FileBlock* fileBlock, PatcherState* patcherState)
{
    fileBlock->needPatch = 0;

    int CLASS_MACRO = getClassMacro(fileBlock->fileName);


    if (CLASS_MACRO == NOT_FOUND)
    {
        printf("\nWarning: Could not remove weapon with file name \"%s\" because it's from an unknown class to this program (New class maybe?).",fileBlock->fileName);
        return;
    }

    if (patcherState->weaponToRemove[CLASS_MACRO] == PRIMARY_WEAPON
        || patcherState->weaponToRemove[CLASS_MACRO] == BOTH_WEAPONS
        || patcherState->weaponToRemove[CLASS_MACRO] == REMOVE_ALL)
    {
        if (contains(fileBlock->folderName,weaponsFolder[CLASS_MACRO][PRIMARY_WEAPON_FOLDER]))
        {
            if (patcherState->removeWeaponInOnly[CLASS_MACRO])
            {
                if (contains(fileBlock->fileName,"_in"))
                {
                    fileBlock->needPatch = 1;
                }
                else
                {
                     fileBlock->needPatch = 0;
                }
            }
            else
            {
                 fileBlock->needPatch = 1;
            }
        }
    }
    if (patcherState->weaponToRemove[CLASS_MACRO] == SECUNDARY_WEAPON
    || patcherState->weaponToRemove[CLASS_MACRO] == BOTH_WEAPONS
    || patcherState->weaponToRemove[CLASS_MACRO] == REMOVE_ALL)
    {
        if (contains(fileBlock->folderName,weaponsFolder[CLASS_MACRO][SECUNDARY_WEAPON_FOLDER]))
        {
            if (patcherState->removeWeaponInOnly[CLASS_MACRO])
            {
                if (contains(fileBlock->fileName,"_in"))
                {
                    fileBlock->needPatch = 1;
                }
                else
                {
                     fileBlock->needPatch = 0;
                }
            }
            else
            {
                 fileBlock->needPatch = 1;
            }
        }
    }
     if (patcherState->weaponToRemove[CLASS_MACRO] == AWAKENING_WEAPON
      || patcherState->weaponToRemove[CLASS_MACRO] == REMOVE_ALL)
    {
        if (contains(fileBlock->folderName,weaponsFolder[CLASS_MACRO][AWAKENING_WEAPON_FOLDER]))
        {
            if (patcherState->removeWeaponInOnly[CLASS_MACRO])
            {
                if (contains(fileBlock->fileName,"_in"))
                {
                    fileBlock->needPatch = 1;
                }
                 else
                {
                     fileBlock->needPatch = 0;
                }

            }
            else
            {
                 fileBlock->needPatch = 1;
            }
        }
    }

    // Removes Valkyrie Awakening Shield if it's set to remove
    if (CLASS_MACRO == VALKYRIE && ((patcherState->weaponToRemove[VALKYRIE] == SECUNDARY_WEAPON) || (patcherState->weaponToRemove[VALKYRIE] == REMOVE_ALL)))
    {
        if (strcmpi(fileBlock->folderName,"character/model/1_pc/7_pvw/weapon/46_spearshield/") == 0)
        {
            if (patcherState->removeWeaponInOnly[VALKYRIE])
            {
                if (contains(fileBlock->fileName,"_in"))
                {
                    fileBlock->needPatch = 1;
                }
                else
                {
                    fileBlock->needPatch = 0;
                }
            }
            else
            {
                 fileBlock->needPatch = 1;
            }
        }
    }
}


void patchFromFile(int operation, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo,PatcherState* patcherState)
{

    int skip = 0;
    int i = 0;
    int j = 0;
    int filesToPatchCount = 0;
    int paz_browser_patcher_state_n_files = 0;
    PazBrowserPatcherState* pazBrowserPatcherState =  load_paz_browser_patcher_state(&paz_browser_patcher_state_n_files, operation);

    clearFileBlocks(fileBlocks,metaFileInfo);

    if (pazBrowserPatcherState != NULL)
    {

        for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
        {
            for (j = 0; j < paz_browser_patcher_state_n_files; j++)
            {
                if (strcmpi(fileBlocks[i].fileName,pazBrowserPatcherState->fileName[j]) == 0)
                {
                    fileBlocks[i].needPatch = 1;
                    filesToPatchCount++;
                    break;
                }
                if (filesToPatchCount == paz_browser_patcher_state_n_files)
                {
                    break;
                }
            }
        }
    }


    for(i = 0; i < paz_browser_patcher_state_n_files; i++)
    {
        free(pazBrowserPatcherState->fileName[i]);
    }
    free(pazBrowserPatcherState);

    if (filesToPatchCount == 0)
        {
            printf("\nWarning: No files were selected to patch in:\n");
            printf("%s\\patcher_resources\\%s", getCurrentPath(),PAZ_BROWSER_PATCHER_STATE_FILE_NAME[operation]);
            printf("\n\nCheck if the file above exists and if it has something inside of it.");
            printf("\nUse the option \"1 - Browse Files\" to mark files to patch.");
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
            if (operation == FIX_HOLES)
            {
                generatePartcutdesc(fileBlocks,metaFileInfo,patcherState);
                PAUSE();
            }
            else if (operation == REMOVE_A_FILE)
            {
                printf("\n\nCopy files...\n");
                copyBlankFiles(fileBlocks,metaFileInfo,"files_to_patch\\");
            }
            else if (operation == RESTORE_A_FILE)
            {
                patchMetaFile(fileBlocks,metaFileInfo->fileBlocksCount,RESTORE_A_FILE,metaFileInfo,1);
                PAUSE();
            }
        }
}

int bodyPartsSizeChanged(PatcherState* patcherState)
{
    int i = 0;

    for(i = 0; i < BODY_SIZE_N_OPTIONS; i++)
    {
        if ((patcherState->bodyPartSize[i].max_size      != MAX_DEFAULT_SIZE)
        || (patcherState->bodyPartSize[i].min_size       != MIN_DEFAULT_SIZE)
        || (patcherState->bodyPartSize[i].default_size   != DEFAULT_DEFAULT_SIZE))
        {
            return 1;
        }
    }
    return 0;
}




int isATreeDummerFile(char* fileName)
{
    if (contains(fileName,"treedummer"))
    {
        return 1;
    }
    if (contains(fileName,"10_hel_0004"))
    {
        return 1;
    }
     if (contains(fileName,"10_cloak_0004"))
    {
        return 1;
    }
    int i = 0;
    for (i = 0; i < TOTAL_TREEE_DUMMER_FILES; i++)
    {
        if (strcmpi(fileName,treedummer_files[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}


int isRestricted(int CLASS)
{
    if (classRestrictions[CLASS] == 1)
    {
        return 1;
    }
    return 0;
}


int isRegular(char* fileName)
{
    char *cashShopItems[] =
    {
        "PHW_02_*_0006",
        "PEW_02_*_0006",
        "PHM_02_*_0006",
        "PHW_02_*_0005",
        "PGM_02_*_0005",
        "PEW_02_*_0006",
        "PBW_03_*_0001"
    };

    char *regularItems[] =
    {
        "PVW_00_*_0000",
        "PVW_00_*_0001",
        "PKWW_00_*_0012",
        "PKM_00_*_0012",
        "PHW_01_*_0001",
    };

    int i = 0;


    for (i = 0; i < 5; i++)
    {
        if (contains(fileName,regularItems[i]))
        {
            return 1;
        }
    }

    for (i = 0; i < 7; i++)
    {
        if (contains(fileName,cashShopItems[i]))
        {
            return 0;
        }
    }

    int start = indexOf('_',fileName,0) + 1;
    int end = indexOf('_',fileName,1);

    char* twoDigits = substr(fileName,start,end);


    if (((strcmpi(twoDigits,"01") == 0) || (strcmpi(twoDigits,"02") == 0 || strcmpi(twoDigits,"03") == 0)) && strlen(fileName) >= 18)
    {
        free(twoDigits);
        return 1;
    }
    free(twoDigits);
    return 0;
}

int isStarter(char* fileName)
{
    if (contains(fileName,"_uw_"))
    {
        if (contains(fileName,"p*_00_uw_0001"))
        {
            return 1;
        }
    }
    if (contains(fileName,"pvw_02_*_0007"))
    {
        return 1;
    }
    if (contains(fileName,"pw*_03_*_0003"))
    {
        return 1;
    }
    if (contains(fileName,"*_02_*_0001"))
    {
        return 1;
    }
    return 0;
}

int isAFemaleNudeTexture(char* fileName, int use3DVaginas)
{
    int i = 0;
    for(i = 0; i < TOTAL_NUDE_FEMALE_TEXTURES; i++)
    {
        if (strcmpi(fileName,nudeFemaleTextures[i]) == 0)
        {
            return 1;
        }
    }
    if (use3DVaginas)
    {
        for(i = 0; i < TOTAL_FEMALE_NORMAL_MAPS_TEXTURES; i++)
        {
            if ((strlen(nudeFemaleNormalMaps[i]) > 1) && (strcmpi(fileName,nudeFemaleNormalMaps[i]) == 0))
            {
                return 1;
            }
        }
    }
    return 0;
}

int isAFemaleNudeModel(char* fileName)
{
     // Improves performance
    if (fileName[0] != 'p' || fileName[2] != 'w')
    {
        return 0;
    }
    if (!strstr(fileName,"nude"))
    {
        return 0;
    }
    if (!fileHasExt(fileName,"pac"))
    {
        return 0;
    }
    int i = 0;

    for (i = 0; i < TOTAL_FEMALE_CLASSES; i++)
    {
        if (strcmp(fileName,nudeFemaleModels[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isAMaleNudeTexture(char* fileName)
{
    int i = 0;
    for(i = 0; i < TOTAL_NUDE_MALE_TEXTURES; i++)
    {
        if (strcmpi(fileName,nudeMaleTextures[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isAMaleNudeModel(char* fileName)
{
    return (contains(fileName,"p*m_*_uw_*.pac"));
}

int isPantieFile(char* fileName)
{
    int i = 0;
    for(i = 0; i < TOTAL_PANTIES; i++)
    {
        if (contains(fileName,panties[i]))
        {
            return 1;
        }
    }
    return 0;
}

int isPantsFile(char* fileName)
{
    int i = 0;
    for(i = 0; i < TOTAL_PANTS; i++)
    {
        if (contains(fileName,pants[i]))
        {
            return 1;
        }
    }
    return 0;
}

int isCensorshipTextureFile(char* fileName)
{
    int i = 0;
    for(i = 0; i < TOTAL_CENSORSHIP_FILES; i++)
    {
        if (contains(fileName,censorshipTextureFiles[i]))
        {
            return 1;
        }
    }
    return 0;
}


int isStockingTexture(char* fileName)
{
    int i = 0;

    for(i = 0; i < TOTAL_STOCKINGS; i++)
    {
        if (contains(fileName,stockings[i]))
        {
            return 1;
        }
    }

    for(i = 0; i < TOTAL_STARTER_STOCKINGS; i++)
    {
        if (contains(fileName,starterStockings[i]))
        {
            return 1;
        }
    }

    return 0;
}

int canPatchClass(char* fileName, PatcherState* patcherState)
{
    int CLASS_MACRO = getClassMacro(fileName);

    if (CLASS_MACRO == NOT_FOUND)
    {
        return 0;
    }
    return patcherState->modAppliesToClass[CLASS_MACRO];
}

int needToPatchPartcutdesc(PatcherState* patcherState)
{
    return (patcherState->option[CENSORSHIP] >= MINIMAL_CENSORSHIP_REMOVAL || patcherState->option[USE_PENISES]);
}

