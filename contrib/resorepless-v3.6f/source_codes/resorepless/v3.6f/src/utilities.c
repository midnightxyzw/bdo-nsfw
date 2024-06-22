#include "../include/utilities.h"


void printMainHeader()
{

    printf("******************************************************\n");
    printf("*                                                    *\n");
    printf("*      Black Desert Online Resorepless v3.6f         *\n");
    printf("*                                                    *\n");
    printf("******************************************************\n");
}


PatcherState* loadPatcherState(char* patcherStateFileName)
{
    printf("Load patcher state:\n");
    PatcherState* patcherState = NULL;

    patcherState = (PatcherState*) calloc(1, sizeof(PatcherState));

     /** Patcher State Default values */
    int i = 0;
    for (i = 0; i < BODY_SIZE_N_OPTIONS; i++)
    {
        patcherState->bodyPartSize[i].min_size      = MIN_DEFAULT_SIZE;
        patcherState->bodyPartSize[i].default_size  = DEFAULT_DEFAULT_SIZE;
        patcherState->bodyPartSize[i].max_size      = MAX_DEFAULT_SIZE;
    }

    for (i = 0; i < TOTAL_CLASSES; i++)
    {
        patcherState->weaponToRemove[i] = KEEP_ALL;
        patcherState->modAppliesToClass[i] = 1;
    }
    patcherState->modAppliesToClass[WIZARD] = 0;
    patcherState->weaponToRemove[SORCERESS] = BOTH_WEAPONS;
    patcherState->option[USE_PENISES] = ON;
    patcherState->option[USE_3D_VAGINAS] = OFF;

    patcherState->option[GENDER] = BOTH_GENDERS;
    patcherState->option[NPC] = 1;
    patcherState->penisType[WARRIOR_PENIS]   = HARD;
    patcherState->penisType[BERSERKER_PENIS] = NORMAL;
    patcherState->penisType[BLADER_PENIS]    = HARD;
    patcherState->penisType[WIZARD_PENIS]    = NORMAL;
    patcherState->penisType[NINJA_PENIS]     = HARD;
    patcherState->penisType[STRIKER_PENIS]   = HARD;

     patcherState->kibeliusWings = KEEP_WINGS;

    FILE* patcher_state_file = fopen(concatenate("patcher_resources\\",patcherStateFileName), "rb");




    if (patcher_state_file != NULL)
    {
        if (getFileSize(patcher_state_file) == sizeof(PatcherState))
        {
            fread(patcherState,1,sizeof(PatcherState),patcher_state_file);
            printf("Success.\n");
        }
        else
        {
            printf("\nRestoring config from previous versions...\n");
            Sleep(500);
            if (getFileSize(patcher_state_file) == sizeof(PatcherState_2_8))
            {
                printf("v2.8 config file found.\n");

                PatcherState_2_8* patcherState_2_8 = (PatcherState_2_8*) malloc(1 * sizeof(PatcherState_2_8));
                fread(patcherState_2_8,1,sizeof(PatcherState_2_8),patcher_state_file);

                int i = 0;

                for (i = 0; i < 9; i++)
                {
                    patcherState->option[i] = patcherState_2_8->option[i];
                }

                for (i = 0; i < 5; i++)
                {
                    patcherState->pubicHair[i] = patcherState_2_8->pubicHair[i];
                }

                patcherState->transparentKibelius = patcherState_2_8->transparentKibelius;

                for (i = 0; i < 13; i++)
                {
                    patcherState->weaponToRemove[i] = patcherState_2_8->weaponToRemove[i];
                }

                for (i = 0; i < 13; i++)
                {
                    patcherState->removeWeaponInOnly[i] = patcherState_2_8->removeWeaponInOnly[i];
                }

                for (i = 0; i < 6; i++)
                {
                    if (i < ARMS)
                    {
                        patcherState->bodyPartSize[i] = patcherState_2_8->bodyPartSize[i];
                    }
                    else
                    {
                        patcherState->bodyPartSize[i+1] = patcherState_2_8->bodyPartSize[i];
                    }
                }
                printf("Success.\n");
                Sleep(500);
            }
            if (getFileSize(patcher_state_file) == sizeof(PatcherState_3_0))
            {
                printf("v3.0 config file found.\n");

                PatcherState_3_0* patcherState_3_0 = (PatcherState_3_0*) malloc(1 * sizeof(PatcherState_3_0));
                fread(patcherState_3_0,1,sizeof(PatcherState_3_0),patcher_state_file);

                int i = 0;

                for (i = 0; i < 9; i++)
                {
                    patcherState->option[i] = patcherState_3_0->option[i];
                }

                for (i = 0; i < 5; i++)
                {
                    patcherState->pubicHair[i] = patcherState_3_0->pubicHair[i];
                }

                patcherState->transparentKibelius = patcherState_3_0->transparentKibelius;
                patcherState->kibeliusWings = patcherState_3_0->kibeliusWings;

                for (i = 0; i < 13; i++)
                {
                    patcherState->weaponToRemove[i] = patcherState_3_0->weaponToRemove[i];
                }

                for (i = 0; i < 13; i++)
                {
                    patcherState->removeWeaponInOnly[i] = patcherState_3_0->removeWeaponInOnly[i];
                }

                for (i = 0; i < 7; i++)
                {
                    patcherState->bodyPartSize[i] = patcherState_3_0->bodyPartSize[i];
                }
                printf("Success.\n");
                Sleep(500);
            }
            else
            {
                printf("Unable to restore config.\n");
                Sleep(1500);
                system("cls");
                printf("\nIt seems that you are using a different version of the tool\n\n");
                printf("Your previously saved settings will be reseted\nto ensure the correct operation of this version.\n");
                printf("(Except your marked files, they will be kept)\n\n");
                printf("Do you want to continue?\n");
                printf("[ENTER] - Continue  [ESC] - Exit\n");
                char input = '\0';
                while(input != ENTER && input != 10 && input != ESC)
                {
                    input = getch();
                    if (input == ENTER || input == 10)
                    {
                        fclose(patcher_state_file);
                        printf("\nremove: patcher_resources\\resorepless_patcher_state.bin");
                        remove("patcher_resources\\resorepless_patcher_state.bin");
                        if (!fileExists("patcher_resources\\resorepless_patcher_state.bin"))
                        {
                            printf("\nRemoved.");
                        }
                        else
                        {
                            printf("\nFailed.");
                             Sleep(1500);
                            patcher_state_file = fopen(concatenate("patcher_resources\\",patcherStateFileName), "wb");
                            fclose(patcher_state_file);
                        }

                    }
                }
                if (input == ESC)
                {
                    exit(1);
                }
            }
        }
    }
    else
    {
        printf("Patch state file doesn't exist\n");
    }

    fclose(patcher_state_file);

    return patcherState;
}

void runMetaInjector()
{
    if (fileExists("Meta Injector.exe"))
    {
        system("\"Meta Injector.exe\"");
    }
    else if (fileExists("MetaInjector.exe"))
    {
        system("MetaInjector.exe");
    }
    else
    {
        printColor("Meta Injector.exe not found. Make sure you run it after installing this otherwise it won't work.", YELLOW);
        PAUSE();
    }
}


void resizeWindow(int width, int height)
{
    char* command = (char*) malloc (strlen("mode 34400,14400 & powershell -command \"&{$H=get-host;$W=$H.ui.rawui;$B=$W.buffersize;$B.height=3000;$W.buffersize=$B;}\\") + 10);
    sprintf(command,"mode %d,%d",width,height);
    system(command);   //Set mode to ensure window does not exceed buffer size*/
    SMALL_RECT WinRect = {0, 0, width, height};   //New dimensions for window in 8x12 pixel chars
    SMALL_RECT* WinSize = &WinRect;
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), 1, WinSize);   //Set new size for window

    COORD  BufferSize;

    BufferSize.X = width;
    BufferSize.Y = 10000;

    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), BufferSize);   //Set new size for window
    free(command);
}
int compare(void const *a, void const *b) {
    FileBlock *aa = (FileBlock *)a;
    FileBlock *bb = (FileBlock *)b;

    char* aaa = (char*)aa->fileName;
    char* bbb = (char*)bb->fileName;

    return strcmp(aaa, bbb);
}

int sort_by_folder_name_and_file_name(void const *a, void const *b) {
    FileBlock *aa = (FileBlock *)a;
    FileBlock *bb = (FileBlock *)b;

    char* folderNameA = (char*)aa->folderName;
    char* folderNameB = (char*)bb->folderName;

    char* fileNameA =  (char*)aa->fileName;
    char* fileNameB =  (char*)bb->fileName;

    char* completePathA = concatenate(folderNameA,fileNameA);
    char* completePathB = concatenate(folderNameB,fileNameB);

    int result = strcmp(completePathA, completePathB);

    free(completePathA);
    free(completePathB);

    return result;
}

int compare_middle_digits(void const *a, void const *b) {
    FileBlock** aa = (FileBlock **)a;
    FileBlock** bb = (FileBlock **)b;

    char* ExtA = getFileExt((char*)(*aa)->fileName);
    char* ExtB = getFileExt((char*)(*bb)->fileName);

    char* middleA = getMiddleDigits((char*)(*aa)->fileName);
    char* middleB = getMiddleDigits((char*)(*bb)->fileName);

    char* middleExtNameA = concatenate3(ExtA,middleA,(char*)(*aa)->fileName);
    char* middleExtNameB = concatenate3(ExtB,middleB,(char*)(*bb)->fileName);

    int result = strcmp(middleExtNameA, middleExtNameB);

    free(ExtA);
    free(ExtB);

    free(middleA);
    free(middleB);

    free(middleExtNameA);
    free(middleExtNameB);

    return result;
}

int compare_meta_offset(void const *a, void const *b) {
    FileBlock *aa = (FileBlock *)a;
    FileBlock *bb = (FileBlock *)b;

    long aaa = (long)aa->metaOffset;
    long bbb = (long)bb->metaOffset;

    return (aaa - bbb);
}

char* endian_convert(int num)
{
    char big_end[12];
    sprintf(big_end,"%.8x", num);

    char* little_end = (char*) malloc(9);
    int i = 0;

    for(i = 0; i < strlen(big_end); i +=2)
    {
        char firstNum = big_end[strlen(big_end) - i - 2];
        if (firstNum >= 'a' && firstNum <= 'f')
        {
            firstNum -= 32;
        }
        char secondNum = big_end[strlen(big_end) - i - 1];
        if (secondNum >= 'a' && secondNum <= 'f')
        {
            secondNum -= 32;
        }
        little_end[i] = firstNum;
        little_end[i+1] = secondNum;
    }
    little_end[8]  = '\0';
    return little_end;
}




char* getTwoDigits(char* fileName)
{
    int start = indexOf('_',fileName,0) + 1;
    int end = indexOf('_',fileName,1);

    char* twoDigits = substr(fileName,start,end);

    return twoDigits;
}

char* getMiddleDigits(char* fileName)
{
    int  start = indexOf('_',fileName,1) + 1;
    int end = indexOf('_',fileName,2);

    char* middle = substr(fileName,start,end);

    return middle;
}

char* getClassPrefix(char* fileName)
{
    return substr(fileName,0,3);
}

char* removeClassPrefix(char* fileName)

{
    int firstUnderlineIdx = indexOf('_', fileName,0);
    return substr(fileName,firstUnderlineIdx, strlen(fileName));
}

char* getLastFolder(char* folderName)
{
    int i = strlen(folderName);
    int start = 0;
    int skipped = 0;
    for ( i = strlen(folderName); i >=0; i--)
    {
        if (folderName[i] == '/')
        {
            if (skipped == 0)
            {
                skipped = 1;
            }
            else
            {
                start = i + 1;
                break;
            }
        }
    }
    return substr(folderName,start,strlen(folderName) - 1);
}

int contains(char* fullString, char* substring)
{
    if (substring == NULL || fullString == NULL)
    {
        return 0;
    }
    if (strlen(substring) == 0 || strlen(fullString) == 0)
    {
        return 0;
    }
    if (strlen(substring) > strlen(fullString))
    {
        //printf("\nWarning: Wrong use of function 'contains(%s,%s)'. The first argument has to be longer than the second", fullString, substring);
        return 0;
    }
     int i = 0;
    int j = 0;

    int combo = 0;
    int starCount = 0;
    for (i = 0; i < strlen(fullString); i++)
    {
        if (substring[j] == '*')
        {
            //Ignores the next character in the "fullstring"
            i++; // This prevents things like this being true: _003 contains _*003

            //printf("\n\nsubstring[%d] == \'*\'",j);
            //printf("\nwhile substring[%d] == \'*\'",j);
            while(substring[j] == '*')
            {
                starCount++;
                //printf("\nstarCount = %d", starCount);
                j++;
               //printf("\nj = %d", j);
                if (j >= strlen(substring))
                {
                   // printf("\nj >= strlen(substring)");
                    //printf("\nbreak;");
                    if (combo == (strlen(substring) - starCount))
                    {
                        //printf("\n\n RETURN 1");
                       // PAUSE();
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
           //printf("\nout while1.\n\nfullString[i] = %c     substring[j] = %c", fullString[i],substring[j]);

            if (j < strlen(substring))
            {
              // printf("\nwhile fullString[i] = %c  != substring[j] = %c", fullString[i],substring[j]);
                while(fullString[i] != substring[j])
                {
                    //printf("\nfullString[i] = %c  substring[j] = %c", fullString[i],substring[j]);
                    i++;
                   //printf("\ni = %d", i);
                    if (i >= strlen(fullString))
                    {
                        //printf("\nj >= strlen(substring)");
                       //printf("\nbreak;");
                        break;
                    }
                }

               //printf("\noutwhile2\n\n");
            }
        }
        if (substring[j] == fullString[i]
        || ((substring[j] + 32  <= 'z') && (substring[j] + 32 == fullString[i]))
        || ((substring[j] - 32 >= 'A') && (substring[j] - 32 == fullString[i]))
        || (substring[j] == '/' && fullString[i] == '\\')
        || (substring[j] == '\\' && fullString[i] == '/'))
        {
           //printf("\n\nNormal: fullstring[i] = %c  == %c substring[j]   i: %d  j : %d", fullString[i],substring[j],i,j);
            combo++;
            if (j + 1 <= strlen(substring))
            {
                j++;
            }
            else
            {
                combo = 0;
                j = 0;
                starCount = 0;
               //printf("\n\nj(%d) + 1 <= strlen(substring)(%d)", j,strlen(substring));
               //printf("\ncombo break 2   combo = 0 j = 0\n\n");
            }
        }

        else
        {
            //printf("\n\nNormal: fullstring[i] = %c  != %c substring[j]   i: %d  j : %d", fullString[i],substring[j],i,j);
            combo = 0;
            j = 0;
            starCount = 0;
           //printf("\ncombo break 1  combo = 0 j = 0\n\n");
        }
        //printf("\ncheck combo == strlen(substring) - starcount = %d  == %d - %d  (%d)  ", combo,strlen(substring),starCount,strlen(substring) - starCount);
        if (combo == (strlen(substring) - starCount))
        {
           //printf("\n\n RETURN 1");
            //PAUSE();
            return 1;
        }
    }
    //printf("\n\n RETURN 0");
    //PAUSE();
    return 0;
}


int beginsWith(char* fullString, char* substring)
{
    if (strlen(substring) > strlen(fullString))
    {
        //printf("\nWarning: Wrong use of function 'contains(%s,%s)'. The first argument has to be longer than the second", fullString, substring);
        return 0;
    }
    int i = 0;
    int j = 0;
    int combo = 0;
    for (i = 0; i < strlen(fullString); i++)
    {
        if (substring[j] == fullString[i]
        || ((substring[j] + 32  <= 'z') && (substring[j] + 32 == fullString[i]))
        || ((substring[j] - 32 >= 'A') && (substring[j] - 32 == fullString[i]))
        || (substring[j] == '/' && fullString[i] == '\\')
        || (substring[j] == '\\' && fullString[i] == '/'))
        {
            combo++;
            if (j + 1 <= strlen(substring))
            {
                j++;
            }
            else
            {
                return 0;
                j = 0;
            }

        }
        else
        {
            return 0;
            j = 0;
        }
        if (combo == strlen(substring))
        {
            return 1;
        }
    }
    return 0;
}


int containsStrict(char* fullString, char* substring)
{
    if (strlen(substring) > strlen(fullString))
    {
        return 0;
    }
    int i = 0;
    int j = 0;

    int combo = 0;
    int starCount = 0;
    for (i = 0; i < strlen(fullString); i++)
    {
        if (substring[j] == '*')
        {
            while(substring[j] == '*')
            {
                starCount++;
                j++;
                if (j >= strlen(substring))
                {
                    if (combo == (strlen(substring) - starCount))
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }

            if (j < strlen(substring))
            {
                while(fullString[i] != substring[j])
                {
                    i++;
                    if (i >= strlen(fullString))
                    {
                        break;
                    }
                }
            }
        }
        if (substring[j] == fullString[i]
        || ((substring[j] + 32  <= 'z') && (substring[j] + 32 == fullString[i]))
        || ((substring[j] - 32 >= 'A') && (substring[j] - 32 == fullString[i]))
        || (substring[j] == '/' && fullString[i] == '\\')
        || (substring[j] == '\\' && fullString[i] == '/'))
        {
            combo++;
            if (j + 1 <= strlen(substring))
            {
                j++;
            }
            else
            {
                return 0;
            }
        }

        else
        {
            return 0;
        }
        if (combo == (strlen(substring) - starCount))
        {
            return 1;
        }
    }
    return 0;
}


int containsStartEnd(char* fullString, char* substring, int* start, int* end)
{
    if (strlen(substring) > strlen(fullString))
    {
        //printf("\nWarning: Wrong use of function 'contains(%s,%s)'. The first argument has to be longer than the second", fullString, substring);
        return 0;
    }
    int i = 0;
    int j = 0;
    int combo = 0;
    for (i = 0; i < strlen(fullString); i++)
    {
        if (substring[j] == fullString[i]
        || substring[j] + 32 == fullString[i]
        || substring[j] - 32 == fullString[i]
        || (substring[j] == '/' && fullString[i] == '\\')
        || (substring[j] == '\\' && fullString[i] == '/'))
        {
            combo++;
            if (j + 1 <= strlen(substring))
            {
                j++;
            }
            else
            {
                combo = 0;

                j = 0;
            }

        }
        else
        {
             (*start) = i + 1;
            combo = 0;
            j = 0;
        }
        if (combo == strlen(substring))
        {
            (*end) = (*start) + strlen(substring);
            return 1;
        }
    }
    return 0;
}

char* getFourDigits(char* fileName)
{
    int start = indexOf('_',fileName,2) + 1;
    int end = indexOf('_',fileName,3);
    int dot = indexOf('.',fileName,0);

    if (dot < end)
    {
        end = dot;
    }

    char* fourDigits = substr(fileName,start,end);

    return fourDigits;
}


void askConfirmation()
{
    printf("Do you want to proceed anyway? (y/n) ");
    char input[32] = "";
     while (strcmpi(input,"y") != 0 && strcmpi(input,"n") != 0)
     {
        fflush(stdin);
        scanf("%s", input);
        fflush(stdin);
     }

    if (strcmpi(input,"n") == 0)
    {
        printf("\nExiting.\n\n");
        system("PAUSE");
        exit(1);
    }
}

int folderExists(char* pathToFolder)
{
    return (opendir(pathToFolder) != NULL);
}

void createPath(char* pathToCreate)
{
    charReplace(pathToCreate,'/','\\');

    DIR* dir = opendir(pathToCreate);

    if (dir)
    {
        closedir(dir);
    }
    else
    {
        char* mdCommand = (char*) malloc(strlen("md \"%s\"") + strlen(pathToCreate) + 1);
        sprintf(mdCommand,"md \"%s\"",pathToCreate);
        //printf("\n%s\n",mdCommand);
        system(mdCommand);
        free (mdCommand);
    }
}

void backupMenu()
{
    int backupSelected = -1;

        long backupCount = 0;
        char** backupNames = getBackupList(&backupCount);
        if (backupCount > 0)
        {
            while (1)
            {
                 backupSelected  = selectBackup(backupNames, backupCount);

                 if (backupSelected == -1)
                 {
                     return;
                 }

                 if (backupSelected < 0 || backupSelected > backupCount - 1)
                 {
                    printf("\nInvalid option!\n\n");
                 }
                 else
                 {
                     break;
                 }
            }

            restoreBackup(backupNames[backupSelected]);
        }
        else
        {
             printf("No backup was found");
        }
        free(backupNames);
        printf("\n");
        system("PAUSE");
}



char* getLatestBackup()
{
    long backupCount = 0;
    char** backups = getBackupList(&backupCount);
    if (backupCount == 0)
    {
        return "pad00000.meta";
    }
    return backups[backupCount - 1];
}

// Merges 2 strings into 1
char* concatenate (char* str1, char*str2)
{
    if (str1 == NULL && str2 != NULL)
    {
        return str2;
    }
    if (str2 == NULL && str1 != NULL)
    {
        return str1;
    }
    if (str1 == NULL && str2 == NULL)
    {
        char* emptyStr = (char*) malloc(1);
        emptyStr[0] = '\0';
        return emptyStr;
    }
    char* result = (char*)malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(result,str1);
    strcat(result,str2);
    return result;
}

char* concatenate3(char* str1, char*str2, char* str3)
{
    if (str1 == NULL)
    {
        return concatenate(str2,str3);
    }
    if (str2 == NULL)
    {
        return concatenate(str1,str3);
    }
    if (str3 == NULL)
    {
        return concatenate(str1,str2);
    }
    if (str1 == NULL && str2 == NULL && str3 == NULL)
    {
        char* emptyStr = (char*) malloc(1);
        emptyStr[0] = '\0';
        return emptyStr;
    }

    char* result = (char*)malloc(strlen(str1) + strlen(str2) + strlen(str3)+ 1);
    strcpy(result,str1);
    strcat(result,str2);
    strcat(result,str3);
    return result;
}



int isNumOrLetter(char c)
{
    if (c >= 'a' && c <='z')
    {
        return 1;
    }
    if (c >= 'A' && c <='Z')
    {
        return 1;
    }
    if (c >= '0' && c <='9')
    {
        return 1;
    }
    if (c == '_')
    {
        return 1;
    }
    return 0;
}

int hexToInt(char* hex)
{
   /*FILE* tmpFile = fopen("tmp.bin","wb");
   fwrite(hex,sizeof(int),1,tmpFile);
   int num = 0;
   rewind(tmpFile);
   fread(tmpFile)*/
   return 0;
}

char* intToHex(int x)
{
    char* hex = (char*) malloc(sizeof(int) + 1);
    sprintf(hex,"%d",x);
    return hex;
}

void printColor(char* stringToPrint, Color COLOR)
{
    if (isWindows10)
    {
        switch (COLOR)
        {
            case (RED):
            {
                printf("\033[31;1m%s\033[0m", stringToPrint);
            }break;
            case (GREEN):
            {
                printf("\033[32;1m%s\033[0m", stringToPrint);
            }break;
            case (YELLOW):
            {
                printf("\033[33;1m%s\033[0m", stringToPrint);
            }break;
        }
    }
    else
    {
        printf("%s", stringToPrint);
    }
}

void printBackgroundColor(char* stringToPrint, Color COLOR)
{
    switch (COLOR)
    {
        case (RED):
        {
            printf("\x1b[41;37m%s\x1b[0m",stringToPrint);
        }break;
        case (GREEN):
        {
            printf("\x1b[41;37m%s\x1b[0m",stringToPrint);
        }break;
        case (YELLOW):
        {
            printf("\x1b[41;37m%s\x1b[0m",stringToPrint);
        }break;
    }
}

void printFileBlock(FileBlock fileBlock)
{
    printf("\n%ld|%ld|%ld|offset:%ld|%s|%s",fileBlock.hash,fileBlock.folderNum,fileBlock.fileNum,fileBlock.metaOffset,fileBlock.folderName,fileBlock.fileName);
}

void addToStringArray(char* fileNameToAdd, char*** ref_stringArray, int* ref_arrraySize)
{
    // If the variable that holds the fileNames of the files that failed to patch hasn't been initialized yet
    if ((*ref_stringArray) == NULL)
    {
        (*ref_stringArray) = (char**) malloc(sizeof(char*));
    }
    else // If it has been initialized
    {
         // Increases the size
        (*ref_stringArray) = (char**) realloc((*ref_stringArray), (((*ref_arrraySize) + 1) * sizeof(char*)));
    }
    // Allocates memory to copy the failed file name
    (*ref_stringArray)[(*ref_arrraySize)] = (char*) malloc(strlen(fileNameToAdd) + 1);
    strcpy((*ref_stringArray)[(*ref_arrraySize)],fileNameToAdd); // Copies the name of the failed file

    (*ref_arrraySize)++;
}

int alreadyInStringArray(char* fileName,char** stringArray, int arrraySize)
{
    int i = 0;
    for (i = 0; i < arrraySize; i++)
    {
        if (strcmpi(stringArray[i],fileName) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void printHex(char* str)
{
    int i = 0;
    printf("\n");
    for (i = 0; i < strlen(str); i++)
    {
        printf("%x ", str[i]);
    }
    printf("\n");
}


void charReplace(char* str,char token,char replace)
{
    int i = 0;
    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == token)
        {
            str[i] = replace;

        }
    }
}

void printNextAsHex(char* str,int start, int length)
{
    int i = 0;

    for (i = start; i < (start + length); i++)
    {
        printf("%x ", str[i]);
    }
    printf("\n");
}

void printNextAsDec(char* str,int start, int length)
{
    int i = 0;
    printf("\n");
    for (i = start; i < (start + length); i++)
    {
        printf("%d ", str[i]);
    }
    printf("\n");
}

void printIntAsHex(int num)
{
    char* converted = intToHex(num);
    printHex(converted);
}

int nextFourBytesAsInt(char* str, int startLocation)
{
    int i = 0;
    char fourBytes[5] = "";
    fourBytes[4] = '\0';
    for (i = 0; i < 4; i++)
    {
        fourBytes[i] = str[startLocation+i];
    }
    return hexToInt(fourBytes);
}

char* substr(char* str,int start, int end)
{
    if (end < start)
    {
        //printf("\ntrying to create a substring where end < start |  %s start: %d   end: %d\n", str, start, end);
        return "";
    }
    if (end == start)
    {
        return "";
    }
    char* sub = (char*) malloc((end - start) + 1);
    int j = 0;
    int i = 0;
    for (i = start; i <= end; i++)
    {
        sub[j] = str[i];
        j++;
    }
    sub[j - 1] = '\0';
    return sub;
}

int indexOf(char token, char* str, int skips)
{
    int i = 0;
    int skipped = 0;
    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == token)
        {
            if (skipped == skips)
            {
                return i;
            }
            else
            {
                skipped++;
            }
        }
    }
    return i -1;
}

long locateHash(long hash,MemFileInt* metaFileInMemory,MetaFileInfo* metaFileInfo)
{
    long currentBlock = 0;
    int readNum = 0;

    int startBlock = metaFileInfo->fileBlocksStart/sizeof(int);
    int endBlock = metaFileInfo->fileBlocksEnd/sizeof(int);

    for (currentBlock = startBlock; currentBlock < endBlock; currentBlock++)
    {
        readNum = metaFileInMemory->block[currentBlock];

        if(readNum == hash)
        {
            return currentBlock;
        }
    }
    return NOT_FOUND;
}


long locateIntInMemory(int intToLocate,MemFileInt* arrayInMemory, MetaFileInfo* metaFileInfo)
{
    long currentBlock = 0;
    int readNum = 0;

    for (currentBlock = 0; currentBlock < arrayInMemory->size; currentBlock++)
    {
        readNum = arrayInMemory->block[currentBlock];


        int startBlock = metaFileInfo->fileBlocksStart/sizeof(int);
        int endBlock = metaFileInfo->fileBlocksEnd/sizeof(int);


        if(readNum == intToLocate)
        {
            if ((currentBlock < startBlock || currentBlock > endBlock))
            {
                printf("                       OUTSIDE!");
                 return NOT_FOUND;
            }
            return currentBlock;

        }

    }
    return NOT_FOUND;
}

long locate2IntsInMemory(int int1, int int2 ,MemFileInt* arrayInMemory)
{
    //printf("\nlooking for: %.8x | %.8x\n", int1, int2);
    long currentBlock = 0;
    for (currentBlock = 0; currentBlock < arrayInMemory->size; currentBlock++)
    {

        if(arrayInMemory->block[currentBlock] == int1 && arrayInMemory->block[currentBlock + 1] == int2)
        {
            return currentBlock;
        }

    }
    return NOT_FOUND;
}



/*void undoLastChanges(int echo)
{

    FILE* metaFile = openFile("pad00000.meta","rb+");
    FILE* modFile = fopen(concatenate("patcher_resources\\",LATEST_MODIFICATIONS_FILE_NAME),"rb");
    if (modFile == NULL)
    {
        printf("\nNo latest modifications file detected.");
        fclose(metaFile);
        return;
    }

    printf("\n\nRestoring last changes:\n");
    if (getFileSize(modFile) == 0)
    {
        printf("\nNo changes to undo.");
        fclose(metaFile);
        fclose(modFile);
        return;
    }
    else
    {
        FileBlock* fileToRestore = (FileBlock*) malloc(1 * sizeof(FileBlock));
        int restored = 0;
        int failed = 0;
        long bytesWritten = 0;

        long hash_read_from_file = 0;

        while(fread(&fileToRestore->metaOffset,sizeof(long),1,modFile))
        {
            fread(&fileToRestore->hash,sizeof(long),1,modFile);
            fread(&fileToRestore->folderNum,sizeof(long),1,modFile);
            fread(&fileToRestore->fileNum,sizeof(long),1,modFile);

            int fileNameLen = 0;
            fread(&fileNameLen,sizeof(int),1,modFile);

            fileToRestore->fileName = (char*) malloc(fileNameLen + 1);

            fread(fileToRestore->fileName,fileNameLen,sizeof(char),modFile);
            fileToRestore->fileName[fileNameLen] = '\0';


            fseek(metaFile,fileToRestore->metaOffset,SEEK_SET);
            fread(&hash_read_from_file,sizeof(long),1,metaFile);

            fseek(metaFile,fileToRestore->metaOffset + sizeof(long),SEEK_SET);

            if (hash_read_from_file == fileToRestore->hash)
            {
                bytesWritten = fwrite(&fileToRestore->folderNum,sizeof(long),1,metaFile);
                if (bytesWritten != 1)
                {
                     printColor(" Failed  ", RED);
                     printf("%ld bytes written",bytesWritten);
                }
                bytesWritten = 0;
                bytesWritten = fwrite(&fileToRestore->fileNum,sizeof(long),1,metaFile);
                if (bytesWritten != 1)
                {
                     printColor(" Failed  ", RED);
                     printf("%ld bytes written",bytesWritten);
                }
                else
                {
                     restored++;
                    if(echo)
                    {
                       printColor("Restored ", YELLOW);
                       printf("Hash:%s - %s \n", endian_convert(hash_read_from_file), fileToRestore->fileName);
                    }
                }
            }
            else
            {
                printColor(" Failed  ", RED);
                printf("Hash:%s - %s (Expected: %s)\n", endian_convert(hash_read_from_file), fileToRestore->fileName, endian_convert((int)fileToRestore->hash));

                failed++;
            }
            if (fileToRestore->fileName != NULL)
            {
                free(fileToRestore->fileName);
            }
        }
        if(echo)
        {
           printf("\nRestored: \033[32;1m%d\033[0m   Failed: \033[31;1m%d\033[0m", restored, failed);
        }
        else
        {
            if (failed > 0)
            {
                printColor("\n\nWarning:",RED); printf(" Some files failed to be restored\n\n");
            }
            else
            {
                printf("Done.");
            }
        }


        fclose(modFile);
        remove(concatenate("patcher_resources\\",LATEST_MODIFICATIONS_FILE_NAME));
        fclose(metaFile);
    }
}
*/

/*void toHex(int decimalNumber)
{
	int remainder;
	int quotient;
    int i=1,j;
    char hexadecimalNumber[100];

    quotient = decimalNumber;

    while(quotient!=0)
	{
         remainder = quotient % 16;

      //To convert integer into character
      if( remainder < 10)
           remainder = remainder + 48;		// Add 48(become ascii later) value to remainder if less than 10--see ascii table for more info
      else
         remainder = remainder + 55;		// Add 55(become ascii later) value to remainder if greater than 10--see ascii table for more info

      hexadecimalNumber[i++]= remainder;

      quotient = quotient / 16;
    }

    //printf("Equivalent hexadecimal value of %d is: ",decimalNumber);

    for(j = i -1 ;j> 0;j--)
    {
      printf("%c",hexadecimalNumber[j]);
    }

}

void printNextBytesAsHex(FILE* fp, int howManyBytes)
{
    int i = 0;
    char c;
    for (i = 0; i < howManyBytes; i++)
    {
        fread(&c,1,1,fp);
        printf("%x ", c);
    }
}*/



char** getBackupList(long* backupCount)
{
    return getFilesSingleFolder(getCurrentPath(),"backup",backupCount);
}

int selectBackup(char** backupNames, long backupCount)
{
    int i = 0;

    char input = '\0';
    int nOptions = backupCount;
    int cursor = 1;

     // Display the menu to select the region
    while(1) // This condition makes the menu repeat itself until a valid input is entered
    {
        if (input >= 0)
        {
            system("cls"); // clears the screen
            printMainHeader();
            printf("Select a backup to restore:\n");
            for (i = 0; i < backupCount; i++)
            {
                printCursor(cursor,i+1);printf("%s", backupNames[i]);
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
                return (cursor - 1);
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
                return -1;
            }
        }
    }
    return -1;
}




void createBackup()
{
    /*if (!isClean("pad00000.meta"))
    {
        printf("\nUnexpected bytes read in your latest backup file\n");*/
        /*printf("\nYour pad00000.meta file is not clean");
        printf("\nPlease delete your \"version.dat\" file and let the launcher re-check all your game files");*/
       /* printf("The program will not work properly.\n");
        printf("\nTry running it as administrator.");
        PAUSE();
        return;
    }*/
    int backupNameLen = strlen("pad00000[YYYY-MM-DD].meta.backup");
    char* backupName = (char*)malloc(backupNameLen + 1);


   // remove(concatenate("patcher_resources\\",LATEST_MODIFICATIONS_FILE_NAME));

    time_t now;
    time(&now);

    struct tm* now_tm;
    now_tm = localtime(&now);

    strftime (backupName, backupNameLen + 1, "pad00000[%Y-%m-%d].meta.backup", now_tm);

    char* cmd = (char*) malloc(strlen("copy pad00000.meta pad00000[YYYY-MM-DD].meta.backup") + 1);
    sprintf(cmd,"copy pad00000.meta %s", backupName);
    system(cmd);

    if (fileExists(backupName))
    {
        printf("\nA new backup was created: %s\n",backupName);
    }
    else
    {
        printf("\nCould not create a backup\n");
        printf("Try running the program as administrator\n");
        printf("\nThe program can try to proceed without that file, but it can crash\n\n");
        askConfirmation();
    }
}

int backupExists()
{
    /*long backupCount = 0;
    char** backups = getFilesSingleFolder(getCurrentPath(),"backup", &backupCount);

    if (backupCount == 1)
    {
        printf("%ld backup was found\n", backupCount);
    }
    else
    {
        printf("%ld backups were found\n", backupCount);
    }

    FILE* backupFile = NULL;
    FILE* metaFile = openFile("pad00000.meta","rb");

    int i,j;
    for(i = 0; i < backupCount; i++)
    {
        FILE* backup1 = fopen(backups[i],"rb");
        long version_backup1 = -1;
        fread(&version_backup1,sizeof(long),1,backupFile);
        for(j = 0; j < backupCount; j++)
        {
            if (i != j)
            {
                FILE* backup2 = fopen(backups[j],"rb");
                long version_backup2 = -1;
                fread(&version_backup2,sizeof(long),1,backupFile);
                fclose(backup2);

                if (version_backup1 == version_backup2)
                {
                    if (strlen(backups[i]) <= strlen("pad00000[YYYY-MM-DD].meta.backup"))
                    {
                        printf("\nremove %s\n", backups[i]);
                        remove(backups[i]);
                    }
                    else
                    {
                        printf("\nremove %s\n", backups[j]);
                        remove(backups[j]);
                    }
                }
            }
        }
        fclose(backup1);
    }


    for(i = 0; i < backupCount; i++)
    {
        backupFile = fopen(backups[i],"rb");

        long version_backup = -1;
        long version_metaFile = -1;

        fread(&version_backup,sizeof(long),1,backupFile);
        rewind(metaFile);
        fread(&version_metaFile,sizeof(long),1,metaFile);

        fclose(backupFile);

        if (version_backup == version_metaFile)
        {
            fclose(metaFile);
            return 1;
        }

        fclose(backupFile);
    }
    fclose(metaFile);*/
    return 0;
}

void restoreBackup(char* backupName)
{
    char command[2048];
    printf("\nRestoring: %s", backupName);
    sprintf(command,"copy /y \"%s\" pad00000.meta",backupName);
    system(command);
    //preventFileRecheck(0);
    //remove(concatenate("patcher_resources\\",LATEST_MODIFICATIONS_FILE_NAME));
}

//-----------------------------------------------------------------------------------------------------------------
char* resoreplessFolder()
{
    // return (oneLevelDown(getCurrentPath()));
    return "files_to_patch\\.resorepless\\";
}

void generateReferenceFile()
{
    printf("Generating reference file (it takes a while)...");
    char command[2048] = "";
    char* metaFileName = getLatestBackup();
    if (strlen(metaFileName) == 0)
    {
        metaFileName = (char*) malloc(strlen("pad00000.meta") + 1);
        strcpy(metaFileName,"pad00000.meta");
    }

    sprintf(command, "cd patcher_resources & quickbms.exe 2>NULL 1>reference_file.txt blackdesert_file_lister.bms \"%s", concatenate(getCurrentPath(),metaFileName));
    system(command);
    remove("NULL");

    long refFileSize = getFileSizeByName("patcher_resources/reference_file.txt");

    if (refFileSize < 20000000)
    {
        sprintf(command, "cd patcher_resources & quickbms.exe blackdesert_file_lister.bms \"%s", concatenate(getCurrentPath(),metaFileName));
        system(command);
        printf("\n\nFailed to generate reference_file_textures.txt, the reason is shown above\n");
        if (refFileSize != 0)
        {
            printf("  The program can try using the created reference_file.txt instead,\n");
            printf("  But it will not find a lot of files\n");
            askConfirmation();
        }
        else
        {
            printf("\nThe program cannot continue... Exiting");
            system("PAUSE");
            exit(1);
        }
    }
    else
    {
        printf("\nDone.\n\n");
    }
}

int metaFileChangedSize()
{
    long lastMetafileSize = 0;
    FILE* log = fopen("patcher_resources/paz_browser.log","rb");
    if (!log)
    {
        return 1;
    }
    fread(&lastMetafileSize,sizeof(long),1,log);

    if (lastMetafileSize != getFileSizeByName("pad00000.meta"))
    {
        fclose(log);
        return 1;
    }
    fclose(log);
    return 0;
}

int isCapitalLetter(char c)
{
    return (c >= 'A' && c <= 'Z');
}

int isMinLetter(char c)
{
    return (c >= 'A' && c <= 'Z');
}

FileBlock* getTexturesFrom(FileBlock* fileBlock, char* fileLocation, int* return_texture_count, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo)
{


    printf("\nGetting Textures Names from %s...", fileBlock->fileName);
    char** textureNames = (char**)malloc(MAX_TEXTURES * sizeof(char*));

      int i = 0;
    for(i = 0; i < MAX_TEXTURES; i++)
    {
        textureNames[i] = (char*) malloc(MAX_PATH);
    }

    FileBlock* textures = (FileBlock*) malloc(MAX_TEXTURES * sizeof(FileBlock));
    int texturesCount = 0;



    FILE* PACFile = openFile(concatenate(fileLocation,fileBlock->fileName), "rb");

    char c = '\0';
    int legibleCharCount = 0;
    int underlineCount = 0;
    char c_anterior = '\0';
    int lastUnderline = -1;
    /*int invalid = 0;
    int shortestLength = 65535;
    int longestLength  = 0;

    for(i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        int currentFileNameLength = strlen(fileBlocks[i].fileName);
        if (currentFileNameLength < shortestLength)
        {
            shortestLength = currentFileNameLength;
        }
        if (currentFileNameLength > shortestLength)
        {
            longestLength = currentFileNameLength;
        }
    }*/
    /*printf("\n%ld - shortestLength: (%d)   longestLength: %d", metaFileInfo->fileBlocksCount, shortestLength, longestLength);
    PAUSE();*/

    while (fread(&c,1,1,PACFile))
    {
         //printf("%c", c);
         if (texturesCount >= MAX_TEXTURES)
         {
             printf("\nWarning: The number of textures loaded exceeds the limit of %d!", MAX_TEXTURES);
             break;
         }

        if (isNumOrLetter(c) && legibleCharCount < MAX_PATH)
        {
            int doIt = 1;
            if (legibleCharCount == 0)
            {
                /*if (isCapitalLetter(c))
                {
                    doIt = 1;
                }
                else
                {
                    doIt = 0;
                }*/
            }


            if (doIt)
            {
                textureNames[texturesCount][legibleCharCount] = c;
                textureNames[texturesCount][legibleCharCount + 1] = '\0';
                legibleCharCount++;
            }
        }
        else
        {


            if (/*((strlen(textureNames[texturesCount]) + 4) >= shortestLength)
                &&  (strlen(textureNames[texturesCount]) + 4) <= longestLength)(
                && */(underlineCount >= 1)
                && (textureNames[texturesCount][0] != '_')
                && (textureNames[texturesCount][strlen(textureNames[texturesCount])-1] != '_')
                /*&& (invalid == 0)*/)
            {
                strcat(textureNames[texturesCount],".dds");

                char* textureFileNameNoExt = substr(textureNames[texturesCount],0,strlen(textureNames[texturesCount]) - 4);


               //printf("\n%s", textureNames[texturesCount]);



                for(i = 0; i < metaFileInfo->fileBlocksCount; i++)
                {

                    //printf("\n%s - %s : %d", fileBlocks[i].fileName, textureFileNameNoExt,contains(fileBlocks[i].fileName,textureFileNameNoExt));
                    if ( strlen(textureFileNameNoExt) > 3 && beginsWith(fileBlocks[i].fileName,textureFileNameNoExt) && abs(strlen(textureFileNameNoExt) - strlen(fileBlocks[i].fileName)) <= 8)
                    {
                        //printf("\n%s begins with %s  %d",fileBlocks[i].fileName,textureFileNameNoExt,abs(strlen(textureFileNameNoExt) - strlen(fileBlocks[i].fileName)));
                        memcpy(&textures[texturesCount],&fileBlocks[i],sizeof(FileBlock));
                        /*textures[texturesCount].fileName = (char*) malloc(strlen(textureFileNameNoExt + 1));
                        strcpy(textures[texturesCount].fileName,textureFileNameNoExt);*/
                        printf("\n%d - %s",texturesCount + 1, textures[texturesCount].fileName);
                        //PAUSE();
                        texturesCount++;
                        if (texturesCount >= MAX_TEXTURES)
                        {
                            printf("\nToo many textures!\n\n");
                            PAUSE();
                            (*return_texture_count) = texturesCount - 1;
                            fclose(PACFile);
                            for(i = 0; i < MAX_TEXTURES; i++)
                            {
                                free(textureNames[i]);
                            }
                            free(textureNames);
                            return textures;
                        }
                    }

                }
                  free(textureFileNameNoExt);
                  //PAUSE();
            }
            legibleCharCount = 0;
            lastUnderline = ftell(PACFile);
            //invalid = 0;
        }
        if (legibleCharCount > 0)
        {
            if (c == '_' && c_anterior != '_')
            {


                if ((underlineCount == 0 && (ftell(PACFile) - lastUnderline) <= 3)
                || (underlineCount > 0 && (ftell(PACFile) - lastUnderline) <= 2))
                {
               //    printf("\n%s - (ftell(PACFile) - lastUnderline): %d",textureNames[texturesCount],(ftell(PACFile) - lastUnderline));
               // printf("\n%s is invalid\n",textureNames[texturesCount]);
                    //PAUSE();
                 /*   legibleCharCount = 0;
                    lastUnderline = ftell(PACFile);
                    invalid = 1;*/
                }
                else
                {

                }
                       underlineCount++;
                    lastUnderline = ftell(PACFile);
            }
        }
        else
        {
            underlineCount = 0;
        }
        c_anterior = c;
    }
    if (texturesCount > 0)
    {
       /* printf("\n\nTextures Names found:");
        for (i = 0; i < texturesCount; i++)
        {
            printf("\n%s", textureNames[i]);
        }*/
    }
    printf("\n\n");
    (*return_texture_count) = texturesCount;
    fclose(PACFile);
    for(i = 0; i < MAX_TEXTURES; i++)
    {
        free(textureNames[i]);
    }
    free(textureNames);
    return textures;
}

int isPatched(FileBlock* fileBlock, MetaFileInfo* metaFileInfo, FILE* metaFile)
{
    fseek(metaFile,fileBlock->metaOffset,SEEK_SET);
    int hashFound = 0;
    long hash = -1;
    fread(&hash,sizeof(long),1,metaFile);
    if (hash != fileBlock->hash)
    {
        while(ftell(metaFile) >= metaFileInfo->fileBlocksStart)
        {
            fseek(metaFile,((-2) * sizeof(long)), SEEK_CUR);
            fread(&hash,sizeof(long),1,metaFile);
            if (hash == fileBlock->hash)
            {
                hashFound = 1;
                break;
            }
        }
        if (hashFound == 0)
        {
            fseek(metaFile,fileBlock->metaOffset + sizeof(long),SEEK_SET);
            while(ftell(metaFile) < metaFileInfo->fileBlocksEnd)
            {
                fread(&hash,sizeof(long),1,metaFile);
                if (hash == fileBlock->hash)
                {
                    hashFound = 1;
                    break;
                }
            }
        }
    }
    else
    {
        hashFound = 1;
    }
    if (hashFound == 0)
    {
        return 0;
    }
    else
    {
        long folderNum = -1;
        long fileNum = -1;
        fread(&folderNum,sizeof(long),1,metaFile);
        fread(&fileNum,sizeof(long),1,metaFile);

        if (folderNum == RANDOM_FOLDER_NUM && fileNum == RANDOM_FILE_NUM)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}
int createMenu(MenuElements menu)
{
    char input = '\0';
    int optionSelected = -1;
    int i = 0;

     // Display the menu to select the region
    while(1) // This condition makes the menu repeat itself until a valid input is entered
    {
        system("cls"); // clears the screen
        printMainHeader();
        printf("%s\n", menu.header);
        for (i = 1; i <= menu.nOptions; i++)
        {
            printf("%d - %s\n",i, menu.menuoptions[i]);
        }
        printf("0 - %s\n", menu.menuoptions[0]);
        printf("\nEnter your choice: ");
        fflush(stdin);
        input = getch();

        if (input == ENTER ||  input == 10)
        {
            printf("\n");
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
        if(optionSelected < 1 || optionSelected > menu.nOptions)
        {
            printf("\nInvalid option!\n\n");
            Sleep(500);
        }
        else
        {
            printf("\n");
            break;
        }
    }

    return optionSelected;
}

FileBlock* binarySearchFileBlock(char* key, FileBlock* fileBlocks, int fileblocksCount)
{
    int first = 0;
    int last = fileblocksCount - 1;
    int middle = (int)(first+last)/2;

    while (first <= last)
    {
        if (strcmpi(fileBlocks[middle].fileName,key) < 0)
        {
            first = middle + 1;
        }

        else if (strcmpi(fileBlocks[middle].fileName,key)  == 0)
        {
            return &fileBlocks[middle];
        }
        else
        {
            last = middle - 1;
        }

        middle = (first + last)/2;
   }
   if (first > last)
   {
      /*printf("Not found! %s is not present in the list.\n", key);
      system("PAUSE");*/
   }
   return NULL;
}

void PAUSE()
{
    printf("\n\n");
    system("PAUSE");
}


char* createArrowMenu(MenuElements menu, int* cursor)
{
    char input = '\0';
    if ((*cursor) < 0 || (*cursor) > menu.nOptions)
    {
        (*cursor) = 0;
    }
    int i = 0;

    while(1)
    {
        system("cls");
        printMainHeader();
        printf("%s\n\n", menu.header);
        for (i = 0; i <= menu.nOptions; i++)
        {
            if((*cursor) == i)
            {
               printf(" -> %s\n",menu.menuoptions[i]);
            }
            else
            {
                printf("    %s\n",menu.menuoptions[i]);
            }
        }


        printf("\n");
        printf("ENTER  - Confirm\n");
        printf("ARROWS - Change Selected Option\n");
        printf("ESC    - Go Back\n");


        input = getch();

        if (input == ENTER ||  input == 10)
        {
            break;
        }
        else if (input == ARROW_DOWN)
        {
            if ((*cursor) + 1 <= menu.nOptions)
            {
                (*cursor)++;
            }
            else
            {
                (*cursor) = 0;
            }
        }
        else if (input == ARROW_UP)
        {
            if ((*cursor) - 1 >= 0)
            {
                (*cursor)--;
            }
            else
            {
                (*cursor) = menu.nOptions;
            }
        }

        if (input == ESC)
        {
            return "Back";
        }

    }
    char* optionSelected = (char*) malloc(strlen(menu.menuoptions[(*cursor)]) + 1);
    strcpy(optionSelected,menu.menuoptions[(*cursor)]);

    return optionSelected;
}



void savePatcherState(PatcherState* patcherState,char* patcherStateFileName)
{
    FILE* patcher_state_file = fopen(concatenate("patcher_resources\\",patcherStateFileName), "wb");
    fwrite(patcherState,1,sizeof(PatcherState),patcher_state_file);
    fclose(patcher_state_file);
}


char* getClassAsString(FileBlock* fileBlock)
{
    char* classPrefix = getClassPrefix(fileBlock->fileName);

    if(strcmpi(fileBlock->fileName,"phw_01_nude_0001.dds") == 0)
    {
        return"   Sorceress/Valkyrie/Plum(Maehwa)\n           ";
    }

     if(contains(fileBlock->fileName,"0034")  && fileHasExt(fileBlock->fileName,"dds"))
     {
         return "   All Female";
     }

    if (strcmpi(fileBlock->fileName,"common_decals_road_04_dec.dds") == 0)
    {
        return "\t\"Blank\"";
    }

     if(contains(fileBlock->fileName,"customizationboneparamdesc"))
     {
         return " Extracting Breast Size Patcher XML Files...";
     }

    if (strcmpi(classPrefix,"phm") == 0)
    {
        return "\tWarrior";
    }
    else if (strcmpi(classPrefix,"phw") == 0)
    {
        return "\tSorceress";
    }
    else if (strcmpi(classPrefix,"pew") == 0)
    {
        return "\tRanger";
    }
    else if (strcmpi(classPrefix,"pgm") == 0)
    {
        return "\tBerserker";
    }
    else if (strcmpi(classPrefix,"pbw") == 0)
    {
        return "\tTamer";
    }
    else if (strcmpi(classPrefix,"pkm") == 0)
    {
        return "\tBlader (Musa)";
    }
    else if (strcmpi(classPrefix,"pvw") == 0)
    {
       return "\tValkyrie";
    }
    else if (strcmpi(classPrefix,"pwm") == 0)
    {
       return "\tWizard";
    }
    else if (strcmpi(classPrefix,"pww") == 0)
    {
       return "\tWitch";
    }
    else if (strcmpi(classPrefix,"pnw") == 0)
    {
        return "\tKunoichi";
    }
    else if (strcmpi(classPrefix,"pnw") == 0)
    {
        return "\tNinja";
    }
    else if (strcmpi(classPrefix,"pdw") == 0)
    {
        return "\tDark Knight";
    }
    else if (strcmpi(classPrefix,"pkww") == 0)
    {
        return "\tPlum (Maehwa)";
    }
    else if (strcmpi(classPrefix,"pkw") == 0)
    {
        return "\tPlum";
    }
    else if (strcmpi(classPrefix,"pdw") == 0)
    {
        return "\tDark Knight";
    }
    else if (strcmpi(classPrefix,"pcm") == 0)
    {
        return "\tStriker";
    }
    else if (strcmpi(classPrefix,"pcw") == 0)
    {
        return "\tMystic";
    }
    return "";
}

char* getArmorPartAsString(FileBlock* fileBlock)
{
    char* armorPart = getMiddleDigits(fileBlock->fileName);
    if (strcmpi(armorPart,"ub") == 0)
    {
         if (fileHasExt(fileBlock->fileName,"dds"))
         {
             if (contains(fileBlock->fileName,"0034"))
             {
                 return "Kibelius Armor";
             }
             return "Bra";
         }
        return "Upperbody";
    }

    else if (strcmpi(armorPart,"lb") == 0)
    {
         if (fileHasExt(fileBlock->fileName,"dds"))
         {
             return "Stocking";
         }
        return "Lowerbody";
    }
    else if (strcmpi(armorPart,"uw") == 0)
    {
        return "Underwear";
    }
    else if (strcmpi(armorPart,"hand") == 0)
    {
        return "Gloves";
    }
    else if (strcmpi(armorPart,"foot") == 0)
    {
        return "Boots";
    }
    else if (strcmpi(armorPart,"sho") == 0)
    {
        return "Shoulders";
    }
    else if (strcmpi(armorPart,"hel") == 0)
    {
       return "Helmets";
    }
    else if (strcmpi(armorPart,"cloak") == 0)
    {
       return "Cloak";
    }
    else if (contains(fileBlock->fileName,"treedummer"))
    {
       return "Treant Camouflage";
    }
    else if (contains(fileBlock->folderName,"event_costume"))
    {
       return "Event Costumes";
    }
    else if (contains(fileBlock->folderName,"weapon"))
    {
       return "Weapon";
    }
    if (contains(fileBlock->fileName,"nude"))
    {
       return "Nude";
    }

    return "";
}


FileBlock* searchFileBlock(char* fileName, FileBlock* fileBlocks, int fileBlockCount)
{
    int i = 0;
    for (i = 0; i < fileBlockCount; i++)
    {
        if (strcmpi(fileBlocks[i].fileName,fileName) == 0)
        {
            return &fileBlocks[i];
        }
    }
    printf("\n\nFile \"%s\" could not be found in the pad00000.meta file.\n\n\tThis could mean this file is part of the 8000 files that cannot be patched\n\tbecause their hashes are not present in the meta file.", fileName);
    PAUSE();
    return NULL;
}

FileBlock* searchFileBlockcontains(char* fileName, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo)
{
    int i = 0;
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (contains(fileBlocks[i].fileName,fileName))
        {
            return &fileBlocks[i];
        }
    }
     printf("\n\nFile \"%s\" could not be found in the pad00000.meta file.\n\n\tThis could mean this file is part of the 8000 files that cannot be patched\n\tbecause their hashes are not present in the meta file.\n\n", fileName);
      PAUSE();
    return NULL;
}

int extractFileCustomPath(FileBlock* fileBlockSelected, char* extractionLocation, int echo)
{
    if(echo)
    {
        printf("\nExtracting %s to: %s\n", fileBlockSelected->fileName,extractionLocation);
    }

     if (!fileExists("patcher_resources\\quickbms.exe"))
    {
        printf("\n\nquickbms.exe is not located in the \"patcher_resources\\\" folder\n\n");
        system("PAUSE");
        free(extractionLocation);
        return 0;
    }

    /*if (strcmpi(extractionLocation,"patcher_resources\\") == 0)
    {
        strcpy(extractionLocation,"./");
    }*/

    char pazName[32];
    sprintf(pazName,"PAD%.5ld.PAZ",fileBlockSelected->pazNum);
    char* command = (char*) malloc(strlen("cd patcher_resources\\ & quickbms.exe  1>NULL 2>NULL2 -Y -f \"*%s*\" simple_extraction.bms \"../%s\" \"../%s") + strlen(fileBlockSelected->fileName) + strlen(pazName) + strlen(extractionLocation) + 256);
    if (echo)
    {
        sprintf(command,"cd patcher_resources\\ & quickbms.exe -Y -f \"*%s*\" simple_extraction.bms \"../%s\" \"../%s", fileBlockSelected->fileName,pazName,extractionLocation);
        printf("command: %s\n", command);
    }
    else
    {
        sprintf(command,"cd patcher_resources\\ & quickbms.exe  1>NULL 2>NULL2 -Y -f \"*%s*\" simple_extraction.bms \"../%s\" \"../%s", fileBlockSelected->fileName,pazName,extractionLocation);
    }

    system(command);
    free(command);
    remove("patcher_resources\\NULL");
    remove("patcher_resources\\NULL2");

    /*if (strcmpi(extractionLocation,"./") == 0)
    {
        extractionLocation = (char*) malloc(strlen("patcher_resources\\ ") + 1);
        strcpy(extractionLocation,"patcher_resources\\");
    }*/

    char* destPath = concatenate(extractionLocation,fileBlockSelected->fileName);

    //charReplace(destPath,'/','\\');

    if (!fileExists(destPath))
    {
        printf("\nError: Could find extracted file in:\n\"%s\"\n\n",destPath);
        system("PAUSE");
        return 0;
    }
    if (echo)
    {
        printf("\nSuccess: File extracted: \"%s\"\n",destPath);
    }

    free(command);
    free(destPath);
    return 1;
}

int extractFile(FileBlock* fileBlockSelected)
{
    char* extractionLocation = concatenate("patcher_resources\\extracted_files\\",substr(fileBlockSelected->fileName,0,strlen(fileBlockSelected->fileName)-3));
    extractionLocation[strlen(extractionLocation)-1] = '\\';

    int result = extractFileCustomPath(fileBlockSelected, extractionLocation,1);
    free(extractionLocation);
    return result;
}

char* captalizeFirstLetter(char* str)
{
    char* capStr = (char*) malloc(strlen(str) + 1);
    strcpy(capStr,str);
    if (capStr[0] >= 'a' && capStr[0] <= 'z')
    {
        capStr[0] -= 32;
    }
    return capStr;
}

void extractFileFromPaz(char* fileName, long pazNum, char* dest, int echo)
{
    char pazName[13];
    sprintf(pazName,"PAD%.5ld.PAZ", pazNum);
    char* command = (char*) malloc(strlen("cd patcher_resources\\ & quickbms.exe 1>NULL 2>NULL2 -Y -f \"*%s*\" blackdesert.bms ../%s \"%s") + strlen(fileName) + strlen(pazName) + strlen(dest) + 1);
    if (echo)
    {
        sprintf(command,"cd patcher_resources\\ & quickbms.exe -Y -f \"*%s*\" blackdesert.bms ../%s \"%s", fileName, pazName, dest);
        //printf("\ncommand: %s\n", command);
    }
    else
    {
         sprintf(command,"cd patcher_resources\\ & quickbms.exe 1>NULL 2>NULL2 -Y -f \"*%s*\" blackdesert.bms ../%s \"%s", fileName, pazName, dest);
    }


    system(command);
    remove("patcher_resources\\NULL");
    remove("patcher_resources\\NULL2");
    free(command);
}

int getClassMacro(char* fileName)
{
    if (contains(fileName,"phm"))
    {
        return WARRIOR;
    }
    else if (contains(fileName,"phw"))
    {
        return SORCERESS;
    }
    else if (contains(fileName,"pew"))
    {
        return RANGER;
    }
    else if (contains(fileName,"pgm") || contains(fileName,"pgb"))
    {
        return BERSERKER;
    }
    else if (contains(fileName,"pbw"))
    {
        return TAMER;
    }
     else if (contains(fileName,"pkm"))
    {
        return BLADER;
    }
    else if (contains(fileName,"pvw"))
    {
        return VALKYRIE;
    }
    else if (contains(fileName,"pwm"))
    {
        return WIZARD;
    }
    else if (contains(fileName,"pww"))
    {
        return WITCH;
    }
    else if (contains(fileName,"pnw"))
    {
        return KUNOICHI;
    }
    else if (contains(fileName,"pnm"))
    {
        return NINJA;
    }
    else if (contains(fileName,"pdw"))
    {
        return DARK_KNIGHT;
    }
    else if (contains(fileName,"pkww") || contains(fileName,"pkw"))
    {
        return MAEHWA;
    }
    else if (contains(fileName,"pcm"))
    {
        return STRIKER;
    }
    else if (contains(fileName,"pcw"))
    {
        return MYSTIC;
    }
    else if (contains(fileName,"psw"))
    {
        return RAN;
    } else {
        return RAN;
    }
    return NOT_FOUND;
}

void save_paz_browser_patcher_state(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, int operation)
{
    int i = 0;
    char comma = ',';

    FILE* paz_patcher_state_file = fopen(concatenate("patcher_resources\\", PAZ_BROWSER_PATCHER_STATE_FILE_NAME[operation]),"wb");
    FILE* red_markers_file       = fopen(concatenate("patcher_resources\\", PAZ_BROWSER_RED_MARKERS_FILE_NAME[operation]),"wb");

    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (fileBlocks[i].needPatch == 1)
        {
            fwrite(fileBlocks[i].fileName,strlen(fileBlocks[i].fileName),sizeof(char),paz_patcher_state_file);
            fwrite(&comma,1,sizeof(char),paz_patcher_state_file);

            fwrite(&fileBlocks[i].changeID,sizeof(int),1,red_markers_file);
            fwrite(&fileBlocks[i].hash,sizeof(long),1,red_markers_file);
        }
    }
    fclose(paz_patcher_state_file);
    fclose(red_markers_file);
}

int getFileColor(long fileHash, int operation)
{
   FILE* red_markers_file = fopen(concatenate("patcher_resources\\", PAZ_BROWSER_RED_MARKERS_FILE_NAME[operation]),"rb");

    long blocksRead = 0;
    int color = -1;
    long hashRead = 0;
    if (red_markers_file != NULL)
    {
        while(1)
        {
            blocksRead = fread(&color,sizeof(long),1,red_markers_file);
            if (blocksRead == 0)
            {
                fclose(red_markers_file);
                return 1;
            }
            if (color >= 0 && color <= 4)
            {
                blocksRead = fread(&hashRead,sizeof(long),1,red_markers_file);
                if (blocksRead == 0)
                {
                    fclose(red_markers_file);
                    return 1;
                }
            }
            else
            {
                hashRead = color;
                color = 2;
            }

            if (fileHash ==  hashRead)
            {
                fclose(red_markers_file);
                return color;
            }
        }
    }
    fclose(red_markers_file);
    return 1;
}

PazBrowserPatcherState* load_paz_browser_patcher_state(int* paz_browser_patcher_state_n_files, int operation)
{
    (*paz_browser_patcher_state_n_files) = 0;
    PazBrowserPatcherState* pazBrowserPatcherState = (PazBrowserPatcherState*) calloc(1,sizeof(PazBrowserPatcherState));

    if (strlen(PAZ_BROWSER_PATCHER_STATE_FILE_NAME[operation]) > 0)
    {
        char* pathToPatcherStateFile = concatenate("patcher_resources\\", PAZ_BROWSER_PATCHER_STATE_FILE_NAME[operation]);
        int totalEntries = 0;
        char** entries = readCommaSeparatedFile(pathToPatcherStateFile,&totalEntries);
        (*paz_browser_patcher_state_n_files) = totalEntries;

        pazBrowserPatcherState->fileName = (char**) malloc(totalEntries * sizeof(char*));

        int i = 0;
        for (i = 0; i < totalEntries; i++)
        {
            pazBrowserPatcherState->fileName[i] = entries[i];
        }
    }

    return pazBrowserPatcherState;
}

char** readCommaSeparatedFile(char* pathToFile, int* totalEntriesReturn)
{
    FILE* commaSeparatedFile = fopen(pathToFile,"rb");

    char readByte = '\0';
    int totalEntries = 0;

    if (commaSeparatedFile)
    {
        // Reads the file and counts how many '.' (from .pac) it has, giving the total number of file names present
        while(fread(&readByte,sizeof(char),1,commaSeparatedFile))
        {
            if (readByte == '.')
            {
                totalEntries++;
            }
        }
    }
    if (totalEntries == 0)
    {
        printf("\nNo entries found in \"%s\".\n", pathToFile);
        return NULL;
    }

    char** entries = (char**) malloc(totalEntries * sizeof(char*));

    int i = 0;
    long start = 0;
    long end = 0;
    rewind(commaSeparatedFile);
    while(fread(&readByte,sizeof(char),1,commaSeparatedFile))
    {
        if (readByte == ',')
        {
            if (start < end)
            {
                start = ftell(commaSeparatedFile);
            }
            else
            {
                end = ftell(commaSeparatedFile) - 1;
                long length = end - start;

                entries[i] = (char*) calloc((length + 1) , sizeof(char));

                fseek(commaSeparatedFile,start,SEEK_SET);

                fread(entries[i],1,length,commaSeparatedFile);
                entries[i][length] = '\0';
                //printf("\n%d/%d - %s",i+1,totalEntries, entries[i]);
                i++;

                //Skips the ',' again
                //fread(&readByte,1,1,commaSeparatedFile);

            }
        }
    }
    (*totalEntriesReturn) = totalEntries;
    fclose(commaSeparatedFile);
    return entries;
}

void clearFileBlocks(FileBlock* fileBlocks, MetaFileInfo *metaFileInfo)
{
    int i = 0;
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        fileBlocks[i].needPatch = 0;
        fileBlocks[i].changeID = 0;
    }
}

void extractTexturesFrom(FileBlock* fileBlockSelected, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, int openAfterExtracted)
{
    char* extractionPath = concatenate("patcher_resources\\extracted_files\\",substr(fileBlockSelected->fileName,0,strlen(fileBlockSelected->fileName)-3));
    extractionPath[strlen(extractionPath)-1] = '\\';

    if (!fileExists(concatenate(extractionPath,fileBlockSelected->fileName)))
    {
        extractFile(fileBlockSelected);
    }

    int texturesCount = 0;

    FileBlock* textures = getTexturesFrom(fileBlockSelected,extractionPath,&texturesCount,fileBlocks,metaFileInfo);

    if (texturesCount == 0 || textures == NULL)
    {
        printf("\nFailed to get textures from \"%s\"\n\n", fileBlockSelected->fileName);
        printf("Textures count = %d\n", texturesCount);
        system("PAUSE");
    }
    else
    {
        int i = 0;
        for(i = 0; i < texturesCount; i++)
        {
            if (textures[i].fileName != NULL)
            {
                char* textureFullPath = concatenate(extractionPath,textures[i].fileName);
                if (!fileExists(textureFullPath))
                {
                    extractFileCustomPath(&textures[i],extractionPath,1);
                }
                free(textureFullPath);
            }
        }


        if (openAfterExtracted)
        {
            // Open the extracted folder
            char* command2 = (char*) malloc(strlen("start %s") + strlen(extractionPath) + 1);
            sprintf(command2,"start %s", extractionPath);
            system(command2);
            Sleep(1000);
            free(command2);
        }
    }
    free(extractionPath);
    free(textures);
}



void copyBlankFiles(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, char* destFolder)
{
    int i = 0;
    char* root = destFolder;
    int filesToPatch = 0;
    //int pacFilePatched = 0;
    //FileBlock* groundTextureBlock = NULL;
    printf("\nCopying \"Blank\" Files...\n\n");
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (fileBlocks[i].needPatch == 1)
        {
            filesToPatch++;

            // PAC FILE COPY
            if (fileHasExt(fileBlocks[i].fileName,"pac"))
            {
                // Replace every pac file marked to patch, with a ground model, which we removed the texture, so it's transparent
                copyFileTo("t0072_pumpkin_ground_0001.pac",fileBlocks[i].fileName,"patcher_resources/models/",concatenate(root,fileBlocks[i].folderName),1);
                //pacFilePatched = 1;
            }
            // DDS FILE COPY
            else if (fileHasExt(fileBlocks[i].fileName,"dds"))
            {
               if (!contains(fileBlocks[i].fileName,"_ao.dds"))
                {
                    if (fileBlocks[i].size < 1000000)
                    {
                        copyFileTo("blank_dxt1.dds",fileBlocks[i].fileName,"patcher_resources/texture/",concatenate(root,fileBlocks[i].folderName),1);
                    }
                    else
                    {
                        copyFileTo("blank_dxt5.dds",fileBlocks[i].fileName,"patcher_resources/texture/",concatenate(root,fileBlocks[i].folderName),1);
                    }
                }
                else
                {
                    if (fileBlocks[i].size >= 600000)
                    {
                        copyFileTo("blank_ao_683.dds",fileBlocks[i].fileName,"patcher_resources/texture/",concatenate(root,fileBlocks[i].folderName),1);
                    }
                    else if (fileBlocks[i].size >= 170000 && fileBlocks[i].size < 600000)
                    {
                        copyFileTo("blank_ao_171.dds",fileBlocks[i].fileName,"patcher_resources/texture/",concatenate(root,fileBlocks[i].folderName),1);
                    }
                    else if (fileBlocks[i].size >= 43000 && fileBlocks[i].size < 170000)
                    {
                        copyFileTo("blank_ao_43.dds",fileBlocks[i].fileName,"patcher_resources/texture/",concatenate(root,fileBlocks[i].folderName),1);
                    }
                    else
                    {
                        printColor("        _ao file size mismatch\n",YELLOW);
                        copyFileTo("blank_ao_171.dds",fileBlocks[i].fileName,"patcher_resources/texture/",concatenate(root,fileBlocks[i].folderName),1);
                    }
                }
            }
        }

        /*if (strcmpi(fileBlocks[i].fileName,"common_decals_road_04_dec.dds") == 0)
        {
            groundTextureBlock = &fileBlocks[i];
        }*/
    }





    if (filesToPatch == 0)
    {
        printf("\nNo files to copy");
        PAUSE();
    }
    else
    {
        printf("\nDone.\n\n");
        printf("\n----------------------------------------------------------------------------\n\n");
        printf("\nThe system is now going to run MetaInjector.exe to patch your game files.");
        PAUSE();
        runMetaInjector();


        /*if (pacFilePatched)
        {
            // printf("\nPatch common_decals_road_04_dec.dds");
            groundTextureBlock->needPatch = 1;
            //patchMetaFile(groundTextureBlock,1,REMOVE_A_FILE,metaFileInfo,1);
        }*/
    }
}

void getFilesFromTexture(char* textureName, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo)
{
    if (textureName[strlen(textureName)-1] == '\n')
    {
        textureName[strlen(textureName)-1] = '\0';
    }
    printf("\nGetting files that contains: \"%s\"\n", textureName);
    char* extractionLocation = concatenate("patcher_resources\\extracted_files\\",textureName);
    charReplace(extractionLocation,'.','_');
    charReplace(extractionLocation,'\n','\\');

    if (extractionLocation[strlen(extractionLocation)-1] != '\\')
    {
        extractionLocation = concatenate(extractionLocation,"\\");
    }
    int i = 0;
    int j = 0;
    int filesCount = 0;
    int processedCount = 0;

    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (fileHasExt(fileBlocks[i].fileName,"pac")
            && contains(fileBlocks[i].folderName,"1_pc")
            && !contains(fileBlocks[i].folderName,"weapon")
            && !contains(fileBlocks[i].folderName,"head")
            && !contains(fileBlocks[i].folderName,"etc")
            && !contains(fileBlocks[i].folderName,"40_glasses")
            && !contains(fileBlocks[i].folderName,"41_piercing")
            && !contains(fileBlocks[i].fileName,"earring")
            && !contains(fileBlocks[i].fileName,"dummy")
            )
        {

            filesCount++;
        }
    }



    int run = 0;
    for(run = 0; run < 2; run++)
    {
        for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
        {
             if (((run == 0) && (strcmpi(getClassPrefix(textureName),getClassPrefix(fileBlocks[i].fileName)) == 0) && !contains(fileBlocks[i].folderName,"nude")) // First run, same class
              || ((run == 1) && (strcmpi(getClassPrefix(textureName),getClassPrefix(fileBlocks[i].fileName)) != 0))
              || ((run == 1) && (contains(fileBlocks[i].folderName,"nude")))) // Second run, different class
             {
                if (fileHasExt(fileBlocks[i].fileName,"pac")
                    && contains(fileBlocks[i].folderName,"1_pc")
                    && !contains(fileBlocks[i].folderName,"weapon")
                    && !contains(fileBlocks[i].folderName,"head")
                    && !contains(fileBlocks[i].folderName,"etc")
                    && !contains(fileBlocks[i].folderName,"40_glasses")
                    && !contains(fileBlocks[i].folderName,"41_piercing")
                    && !contains(fileBlocks[i].fileName,"earring")
                    && !contains(fileBlocks[i].fileName,"dummy")
                    )
                {



                    //printf("\nrun: %d  %s %s ", run, getClassPrefix(textureName),getClassPrefix(fileBlocks[i].fileName));
                    printf("(%d/%d)- %.2f percent done", processedCount + 1, filesCount,(float)((double)processedCount/(double)filesCount) * 100.0);
                    extractFileCustomPath(&fileBlocks[i],extractionLocation,0);
                    int textureCount = 0;
                    FileBlock* textures = getTexturesFrom(&fileBlocks[i],extractionLocation,&textureCount,fileBlocks,metaFileInfo);

                    int found = 0;
                    for (j = 0; j < textureCount; j++)
                    {
                        if (strcmpi(textures[j].fileName,textureName) == 0)
                        {
                            printf("\n%s has your texture!", fileBlocks[i].fileName);
                            found = 1;
                            break;
                        }
                    }
                    if (found)
                    {
                         char* extractionLocation_PAC_FILE = concatenate(extractionLocation,fileBlocks[i].fileName);

                         // Replaces the folder name from filename.pac to filename/
                         for(j = 0; j < strlen(extractionLocation_PAC_FILE); j++)
                         {
                            if (extractionLocation_PAC_FILE[j] == '.')
                            {
                                extractionLocation_PAC_FILE[j] = '\\';
                                if (j + 1 < strlen(extractionLocation_PAC_FILE))
                                {
                                    extractionLocation_PAC_FILE[j+1] = '\0';
                                }
                            }
                         }

                         createPath(extractionLocation_PAC_FILE);
                         moveFile(fileBlocks[i].fileName,extractionLocation,extractionLocation_PAC_FILE);

                         for (j = 0; j < textureCount; j++)
                         {
                             if (textures[i].fileName != NULL)
                             {
                                 extractFileCustomPath(&textures[i],extractionLocation_PAC_FILE,0);
                             }
                         }
                        char* command = (char*) malloc(strlen("start %s") + strlen(extractionLocation_PAC_FILE) + 1);
                        sprintf(command,"start %s", extractionLocation_PAC_FILE);
                        system(command);
                        free(command);
                        free(extractionLocation_PAC_FILE);

                        PAUSE();
                    }
                    remove(concatenate(extractionLocation,fileBlocks[i].fileName));

                      free(textures);
                    processedCount++;
                    if (processedCount == filesCount)
                    {
                        break;
                    }
                }

            }

        }

    }
       char* command = (char*) malloc(strlen("start %s") + strlen(extractionLocation) + 1);
        sprintf(command,"start %s", extractionLocation);
        system(command);
        free(command);

}

int isAFemaleFile(char* fileName)
{
    return (fileName[2] == 'w' || fileName[2] == 'W');
}

int isAMaleFile(char* fileName)
{
    return (fileName[2] == 'm' || fileName[2] == 'M');
}

int haveTheSameGender(char* fileName1, char* fileName2)
{
    return (fileName1[2] == fileName2[2]);
}

char* getOperationName(int operation)
{
    if (operation == REMOVE_A_FILE)
    {
       return "remove";
    }
    else if (operation == RESTORE_A_FILE)
    {
       return "restore";
    }
     else if (operation == FIX_HOLES)
    {
       return "fix";
    }
     else if (operation == GET_TEXTURES)
    {
       return "get textures from";
    }
    return "";
}



 /*long countPatchedFiles(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo)
{
   printf("\nCounting patched files...");
    long filesPatched = 0;
    int i = 0;
    FileBlock* moddedFileBlocks = (FileBlock*) malloc(metaFileInfo->fileBlocksCount * sizeof(FileBlock));
    FILE* metaFile = openFile("pad00000.meta", "rb");*/
    /*FILE* patchesFilesList = openFile("patcher_resources\\patched_files.txt", "wb");
    char newLine = '\n';
    char comma = ',';*/

    /*fseek(metaFile,metaFileInfo->fileBlocksStart,SEEK_SET);

    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        fread(&moddedFileBlocks[i].hash,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks[i].folderNum,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks[i].fileNum,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks[i].pazNum,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks[i].fileOffset,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks[i].zsize,sizeof(long),1,metaFile);
        fread(&moddedFileBlocks[i].size,sizeof(long),1,metaFile);

        if ((moddedFileBlocks[i].folderNum == RANDOM_FOLDER_NUM) && (moddedFileBlocks[i].fileNum == RANDOM_FILE_NUM))
        {
            filesPatched++;
        }
    }

    if (filesPatched == 0)
    {
        rewind(metaFile);
        long readNum1 = -1;
        long readNum2 = -1;
        fread(&readNum1,sizeof(long),1,metaFile);
        while(1)
        {
            if (!fread(&readNum2,sizeof(long),1,metaFile))
            {
                break;
            }
            if (readNum1 == RANDOM_FOLDER_NUM && readNum2 == RANDOM_FILE_NUM)
            {
                filesPatched++;
            }
            readNum1 = readNum2;
        }
    }
    fclose(metaFile);

    if (moddedFileBlocks != NULL)
    {
       free(moddedFileBlocks);
    }

    printf("\nDone.");

    return filesPatched;
    return 0;
}*/

/*int isClean(char* metaFileName)
{
    FILE* metaFile = openFile(metaFileName,"rb");
    if (!metaFile)
    {
        return 1;
    }
    long readNum1 = -1;
    long readNum2 = -1;
    long filesPatched = 0;
    if (!fread(&readNum1,sizeof(long),1,metaFile))
    {
        printf("\nFailed to read \"pad00000.meta\".\nTry running this program as administrator.");
        PAUSE();
        return 1;
    }
    while(1)
    {
        if (!fread(&readNum2,sizeof(long),1,metaFile))
        {
            break;
        }
        if (readNum1 == RANDOM_FOLDER_NUM && readNum2 == RANDOM_FILE_NUM)
        {
            filesPatched++;
        }
        readNum1 = readNum2;
    }
    fclose(metaFile);
    return (filesPatched == 0);
}*

void listPatchedFiles(FileBlock* fileBlocks,MetaFileInfo* metaFileInfo)
{
    if (filesPatched == 0)
    {
        printf("\n  No files patched.");
        PAUSE();
        return;
    }

    int i = 0;

    FileBlock* patchedFiles = (FileBlock*) malloc(filesPatched * sizeof(FileBlock));

    int currentPatchedFile = 0;

    FILE* metaFile = openFile("pad00000.meta", "rb");
    printf("\nSearching patched files...");
    for (i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (isPatched(&fileBlocks[i],metaFileInfo,metaFile))
        {
            memcpy (&patchedFiles[currentPatchedFile], &fileBlocks[i], sizeof(FileBlock));
            currentPatchedFile++;
        }
    }

    fclose(metaFile);
    qsort(patchedFiles,filesPatched,sizeof(FileBlock),sort_by_folder_name_and_file_name);
    printf("\nDone.");

    printf("\n\nList of currently patched files:\n");
    for(i = 0; i < filesPatched; i++)
    {
        if (filesPatched < 10)
        {
            printf("%d", i+1);
        }
        else if (filesPatched < 100)
        {
            printf("%.2d", i+1);
        }
        else if (filesPatched < 1000)
        {
            printf("%.3d", i+1);
        }
        else
        {
            printf("%d", i+1);
        }
        printf(" - %s%s\n", patchedFiles[i].folderName, patchedFiles[i].fileName);
    }
    PAUSE();
    free(patchedFiles);
}*/

char* bdoRootFolder()
{
    return (oneLevelDown(getCurrentPath()));
}

void preventFileRecheck(int forced)
{
    char* root = bdoRootFolder();

    char* configFileCheckPath = concatenate(root,"config.filecheck");
    char* versionDatPath = concatenate(root,"version.dat");

    FILE* metaFile = openFile(getLatestBackup(),"rb");
    FILE* versionDatFile = fopen(versionDatPath,"rb");

    int clientVersion_versionDat = -1;
    int clientVersion_metaFile = -2;

    fread(&clientVersion_metaFile,sizeof(int),1,metaFile);
    if (versionDatFile != NULL)
    {
         long versionDatFileSize = getFileSize(versionDatFile);
         char* versionDatAsString = (char*) malloc(versionDatFileSize);

        fread(versionDatAsString,sizeof(char),versionDatFileSize,versionDatFile);


        versionDatAsString[versionDatFileSize-1] = '\0';

        clientVersion_versionDat = atoi(versionDatAsString);
        free(versionDatAsString);

        if (forced)
        {
            if (clientVersion_versionDat != clientVersion_metaFile)
            {
                char proceed = 0;
                printf("\nWarning: Client version from version.dat (%d) differs from the pad00000.meta file (%d).", clientVersion_versionDat,clientVersion_metaFile);
                printf("\nAre you sure you want to proceed? (y/n)\n");
                printf(":");
                scanf("%c", &proceed);

                if (proceed != 'y' && proceed != 'Y')
                {
                    free(configFileCheckPath);
                    free(versionDatPath);
                    //free(root);
                    fclose(metaFile);
                    printf("\nNo.");
                    return;
                }
                printf("\nYes.");
            }
        }

    }



    if (!remove(configFileCheckPath))
    {
       printf("\nDeleting config.filecheck file - "); printf("successfully deleted.\n");
    }
    else if (fileExists(configFileCheckPath))
    {
        printf("\nDeleting config.filecheck file - "); printf("Failed to delete.\n");
    }

    if (versionDatFile == NULL || (clientVersion_versionDat != clientVersion_metaFile) || forced)
    {
        printf("\nCreating version.dat file...\n");
        fclose(versionDatFile);
        versionDatFile = openFile(versionDatPath,"wb");
        if (versionDatFile != NULL)
        {
            char endByte = '\x0A';
            char clientVersionAsString[16];
            sprintf(clientVersionAsString,"%d",clientVersion_metaFile);
            fwrite(&clientVersionAsString,sizeof(char),strlen(clientVersionAsString),versionDatFile);
            fwrite(&endByte,1,1,versionDatFile);
        }
        printf("Done.");
    }
    free(configFileCheckPath);
    free(versionDatPath);
    //free(root);
    fclose(metaFile);
    fclose(versionDatFile);
}

int getWindowsVersion()
{

    system("cd patcher_resources\\ & ver >windows_version.txt");

    FILE* fp = fopen("patcher_resources\\windows_version.txt", "rb");

    if (fp == NULL)
    {
        return 0;
    }
    int c = '\0';
    while(fread(&c,1,1,fp))
    {
        if (c == '.')
        {
            fseek(fp,-3,SEEK_CUR);
            char versionStr[3] = "";
            fread(versionStr,2,sizeof(char),fp);
            fclose(fp);
            remove("patcher_resources\\windows_version.txt");
            return  atoi(versionStr);

        }
    }
    fclose(fp);
    remove("patcher_resources\\windows_version.txt");
    return 0;
}

int isALetter(char c)
{
    return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')));
}

int getPenisMacro(char* fileName)
{
    char* classPrefix = getClassPrefix(fileName);
    int MACRO = NOT_FOUND;
    if (strcmpi(classPrefix,"phm") == 0)
    {
        MACRO = WARRIOR_PENIS;
    }
    else if (strcmpi(classPrefix,"pgm") == 0)
    {
        MACRO = BERSERKER_PENIS;
    }
    else if (strcmpi(classPrefix,"pkm") == 0)
    {
        MACRO = BLADER_PENIS;
    }
    else if (strcmpi(classPrefix,"pwm") == 0)
    {
        MACRO = WIZARD_PENIS;
    }
    else if (strcmpi(classPrefix,"pnm") == 0)
    {
        MACRO = NINJA_PENIS;
    }
    else if (strcmpi(classPrefix,"pcm") == 0)
    {
        MACRO = STRIKER_PENIS;
    }
    free(classPrefix);
    return MACRO;
}

int getPubicHairMacro(char* fileName)
{
    // typedef enum {RANGER_PUBIC_HAIR,WITCH_PUBIC_HAIR,TAMER_PUBIC_HAIR,DARK_KNIGHT_PUBIC_HAIR,SORCERESS_PUBIC_HAIR,VALKYRIE_PUBIC_HAIR,MAEHWA_PUBIC_HAIR,KUNOICHI_PUBIC_HAIR,MYSTIC_PUBIC_HAIR} PubicHairClass;
    char* classPrefix = getClassPrefix(fileName);
    int MACRO = NOT_FOUND;
    if (strcmpi(classPrefix,"phw") == 0)
    {
        MACRO = SORCERESS_PUBIC_HAIR;
    }
    else if (strcmpi(classPrefix,"pew") == 0)
    {
        MACRO = RANGER_PUBIC_HAIR;
    }
    else if (strcmpi(classPrefix,"pbw") == 0)
    {
        MACRO = TAMER_PUBIC_HAIR;
    }
    else if (strcmpi(classPrefix,"pvw") == 0)
    {
        MACRO = VALKYRIE_PUBIC_HAIR;
    }
    else if (strcmpi(classPrefix,"pdw") == 0)
    {
        MACRO = DARK_KNIGHT_PUBIC_HAIR;
    }
    else if (contains(classPrefix,"pkw"))
    {
        MACRO = MAEHWA_PUBIC_HAIR;
    }
    else if (strcmpi(classPrefix,"pww") == 0)
    {
        MACRO = WITCH_PUBIC_HAIR;
    }
    else if (strcmpi(classPrefix,"pnw") == 0)
    {
        MACRO = KUNOICHI_PUBIC_HAIR;
    }
    else if (strcmpi(classPrefix,"pcw") == 0)
    {
        MACRO = MYSTIC_PUBIC_HAIR;
    }
    free(classPrefix);
    return MACRO;
}

char* classMacroToPrefix(int classMacro)
{
    if (classMacro == WARRIOR)
    {
        return "phm";
    }
    else if (classMacro == SORCERESS)
    {
        return "phw";
    }
    else if (classMacro == RANGER)
    {
        return "pew";
    }
    else if (classMacro == BERSERKER)
    {
        return "pgm";
    }
    else if (classMacro == TAMER)
    {
        return "pbw";
    }
    else if (classMacro == BLADER)
    {
        return "pkm";
    }
    else if (classMacro == VALKYRIE)
    {
        return "pvw";
    }
    else if (classMacro == WIZARD)
    {
        return "pwm";
    }
    else if (classMacro == WITCH)
    {
        return "pww";
    }
    else if (classMacro == KUNOICHI)
    {
        return "pnw";
    }
    else if (classMacro == NINJA)
    {
        return "pnm";
    }
    else if (classMacro == DARK_KNIGHT)
    {
        return "pdw";
    }
    else if (classMacro == MAEHWA)
    {
        return "pkww";
    }
    else if (classMacro == STRIKER)
    {
        return "pcm";
    }
    else if (classMacro == MYSTIC)
    {
        return "pcw";
    }
    printf("\nWarning: unknown class macro number: %d", classMacro);
    return "";
}

int haveSameClass(char* fileNameA, char* fileNameB)
{
    char* classPrefixA = getClassPrefix(fileNameA);
    char* classPrefixB = getClassPrefix(fileNameB);

    int result = (strcmpi(classPrefixA,classPrefixB) == 0);

    free(classPrefixA);
    free(classPrefixB);

    return result;
}

void printRename(char* fileNameA, char* fileNameB, int longestLength)
{

    int l = 0;
    int diff = 0;
    if (strlen(fileNameA) < longestLength)
    {
        diff = longestLength - strlen(fileNameA);
    }
    for (l = 0; l < diff; l++)
    {
        printf(" ");
    }
    printf("%s",fileNameA);
    printf(" -> ");
    printf("%s", fileNameB);
}


char* getLatestFolder(char* folderPath)
{
    int lastIDX = strlen(folderPath) - 1;
    int i = 0;
    int folderNameStart = 0;
    for (i = (lastIDX - 1) ; i >= 0; i--)
    {
        if (folderPath[i] == '/' || folderPath[i] == '\\')
        {
            folderNameStart = i + 1;
            break;
        }
    }

    int length = lastIDX - folderNameStart;
    char* latestFolder = (char*) malloc(length + 1);

    int j = 0;
    for (i = folderNameStart; i <= lastIDX; i++)
    {
        latestFolder[j] = folderPath[i];
        j++;
    }
    latestFolder[j] = '\0';



    return latestFolder;
}

/*void mergeDDS(char* srcFolder, char* srcFileName,char* destFolder, char* destFileName)
{
    char* pathToSrcDDS = concatenate(srcFolder,srcFileName);
    char* pathToDestDDS = concatenate(destFolder,destFileName);

    FILE* srcDDS = openFile(pathToSrcDDS,"rb+");
    FILE* destDDS = openFile(pathToDestDDS,"rb+");

    free(pathToSrcDDS);
    free(pathToDestDDS);

    long srcFileSize = getFileSize(srcDDS);
    long destFileSize = getFileSize(destDDS);

    if (srcFileSize != destFileSize)
    {
        printf("\nDifferent file sizes: %ld %ld", srcFileSize, destFileSize);
        Sleep(1500);
        fclose(srcDDS);
        fclose(destDDS);
        return;
    }

    char* buffer = (char*) malloc(1);

    long bytesRead = 0;
    long bytesWritten = 0;

    int i = 0;
    for (i = 0; i < 2564224; i++)
    {
        bytesRead += fread(buffer,1,1,srcDDS);
        bytesWritten += fwrite(buffer,1,1,destDDS);
    }

    fseek(srcDDS,8388736,SEEK_SET);
    fseek(destDDS,8388736,SEEK_SET);

    for (i = 8388736; i <= 9027712; i++)
    {
        bytesRead += fread(buffer,1,1,srcDDS);
        bytesWritten += fwrite(buffer,1,1,destDDS);
    }

    fseek(srcDDS,10485888,SEEK_SET);
    fseek(destDDS,10485888,SEEK_SET);

    for (i = 10485888; i <= 10645632; i++)
    {
        bytesRead += fread(buffer,1,1,srcDDS);
        bytesWritten += fwrite(buffer,1,1,destDDS);
    }

    fseek(srcDDS,11010176,SEEK_SET);
    fseek(destDDS,11010176,SEEK_SET);

    for (i = 11010176; i <= 11050112; i++)
    {
        bytesRead += fread(buffer,1,1,srcDDS);
        bytesWritten += fwrite(buffer,1,1,destDDS);
    }

     fseek(srcDDS,11141248,SEEK_SET);
    fseek(destDDS,11141248,SEEK_SET);

    for (i = 11141248; i <= 11150976; i++)
    {
        bytesRead += fread(buffer,1,1,srcDDS);
        bytesWritten += fwrite(buffer,1,1,destDDS);
    }

     fseek(srcDDS,11174016,SEEK_SET);
    fseek(destDDS,11174016,SEEK_SET);

    for (i = 11174016; i <= 11176320; i++)
    {
        bytesRead += fread(buffer,1,1,srcDDS);
        bytesWritten += fwrite(buffer,1,1,destDDS);
    }

      fseek(srcDDS,11182208,SEEK_SET);
    fseek(destDDS,11182208,SEEK_SET);

    for (i = 11182208; i <= 11182848; i++)
    {
        bytesRead += fread(buffer,1,1,srcDDS);
        bytesWritten += fwrite(buffer,1,1,destDDS);
    }

       fseek(srcDDS,11184256,SEEK_SET);
    fseek(destDDS,11184256,SEEK_SET);

    for (i = 11184256; i <= 11184448; i++)
    {
        bytesRead += fread(buffer,1,1,srcDDS);
        bytesWritten += fwrite(buffer,1,1,destDDS);
    }

       fseek(srcDDS,11184768,SEEK_SET);
    fseek(destDDS,11184768,SEEK_SET);

    for (i = 11184768; i <= 11184832; i++)
    {
        bytesRead += fread(buffer,1,1,srcDDS);
        bytesWritten += fwrite(buffer,1,1,destDDS);
    }

    free(buffer);

    fclose(srcDDS);
    fclose(destDDS);
}
*/

void createDDSBin(char* pathToDDS)
{

    FILE* ddsFile = openFile(pathToDDS,"rb");

    char* pathToBin = replaceExt(pathToDDS,"bin");
      printf("\nCreating bin for: %s", pathToBin);
    FILE* binFile = openFile(pathToBin,"wb");
    free(pathToBin);

    FILE* offsetFile = openFile("patcher_resources\\texture\\pubic_hair\\offsets.bin","rb");

    int i = 0;
    char buffer;
    PubicHairOffset offset;

    while(fread(&offset,sizeof(PubicHairOffset),1,offsetFile))
    {
        fseek(ddsFile,offset.value,SEEK_SET);
        printf("\noffset: %ld  length : %ld", offset.value,offset.length);
        for (i = 0; i < offset.length; i++)
        {
            fread(&buffer,1,1,ddsFile);
            fwrite(&buffer,1,1,binFile);
        }
    }
    fclose(ddsFile);
    fclose(offsetFile);
    fclose(binFile);
}

char* replaceExt(char* fileName,char* newExt)
{
    char* newFileName = (char*) malloc(strlen(fileName) + strlen(newExt));
    strcpy(newFileName,fileName);
    int i = 0;
    int j = 0;
    for (i = strlen(fileName) - 1; i >= 0; i--)
    {
        if (fileName[i] == '.')
        {
            for (j = 0; j < strlen(newExt); j++)
            {
                newFileName[i+j+1] = newExt[j];

            }
            newFileName[i+j+1] = '\0';
            break;
        }
    }
    return newFileName;
}

void reconstructDDS(char* pubicHairFolder, char* pubicHairTextureName, char* characterTextureFolder, char* targetTextureName)
{
    char* binFileName = replaceExt(pubicHairTextureName,"bin");

    char* pathToTarget = concatenate(characterTextureFolder,targetTextureName);
    char* pathToSrc = concatenate(pubicHairFolder,binFileName);

    printf("\nMerging: \"%s\"",pathToSrc);
    printf("\n   with: \"%s\"\n\n", pathToTarget);

    free(binFileName);

    FILE* ddsFile = openFile(pathToTarget,"rb+");
    FILE* binFile = openFile(pathToSrc,"rb");

    free(pathToTarget);
    free(pathToSrc);

    FILE* offsetFile = openFile("patcher_resources\\texture\\pubic_hair\\offsets.bin","rb");

    int i = 0;
    char buffer;
    PubicHairOffset offset;

     while(fread(&offset,sizeof(PubicHairOffset),1,offsetFile))
     {
        fseek(ddsFile,offset.value,SEEK_SET);

        for (i = 0; i < offset.length; i++)
        {
            fread(&buffer,1,1,binFile);
            fwrite(&buffer,1,1,ddsFile);
        }
     }
    //printf("\nDone\n");
    fclose(ddsFile);
    fclose(binFile);
}

// Not used, just in case we need to generate it again
void generateTextureBinFiles()
{

    int currentClassTexture = 0;



        for (currentClassTexture = 0; currentClassTexture < TOTAL_NUDE_FEMALE_TEXTURES; currentClassTexture++)
        {
            char* folderName = "patcher_resources\\texture\\pubic_hair\\shaved\\";
            char* ddsFileName = nudeFemaleTextures[currentClassTexture];



            char* pathToDDS = concatenate(folderName,ddsFileName);

             printf("\n%s",pathToDDS);
            createDDSBin(pathToDDS);
        }
        printf("\n");


    PAUSE();
}
// Not used, just in case we need to generate it again
void generate_texture_pac_relations_file(FileBlock* fileBlocks,MetaFileInfo* metaFileInfo)
{
    int i = 0;
    int j = 0;

    FILE* fp = openFile("patcher_resources\\texture_pac_files_relations.txt", "wb");
    for(i = 0; i < metaFileInfo->fileBlocksCount; i++)
    {
        if (fileHasExt(fileBlocks[i].fileName,"pac") && contains(fileBlocks[i].folderName,"character/model/") && fileBlocks[i].fileName[0] != '_')
        {
            printf("\n%d/%ld",i+1, metaFileInfo->fileBlocksCount);
                if (!fileExistsInFolder("patcher_resources/extracted_files/",fileBlocks[i].fileName))
                {
                    extractFileCustomPath(&fileBlocks[i],"patcher_resources/extracted_files/",0);
                }
                int textureCount = 0;
                FileBlock* textures = getTexturesFrom(&fileBlocks[i],"patcher_resources/extracted_files/",&textureCount,fileBlocks,metaFileInfo);

                fwrite(&fileBlocks[i].hash,sizeof(long),1,fp);
                fwrite(&textureCount,sizeof(int),1,fp);

                for(j = 0; j < textureCount; j++)
                {
                    fwrite(&textures[j].hash,sizeof(long),1,fp);
                }
                free(textures);
        }
    }
    fclose(fp);
    PAUSE();

}

void printCursor(int cursor, int valueToCompare)
{
    if (cursor == valueToCompare)
    {
        printf("\n -> ");
    }
    else
    {
        printf("\n    ");
    }
}

void printOnOffSwitch(int variable)
{
    if (variable)
    {
        printColor(" On",GREEN);
    }
    else
    {
        printColor(" Off",RED);
    }
}

int similarity(char* fileNameA, char* fileNameB)
{
    int totalSectionsA = 0;
    int totalSectionsB = 0;
    int similarity = 0;


    int i = 0;
    for (i = 0; i < strlen(fileNameA); i++)
    {
        if (fileNameA[i] == '_' || fileNameA[i] == '.')
        {
            totalSectionsA++;
        }
    }
    for (i = 0; i < strlen(fileNameB); i++)
    {
        if (fileNameB[i] == '_' || fileNameB[i] == '.')
        {
            totalSectionsB++;
        }
    }

    if (totalSectionsA == 0 || totalSectionsB == 0)
    {
        return 0;
    }
    char** fileAsections = (char**) calloc(totalSectionsA, sizeof(char*));
    char** fileBsections = (char**) calloc(totalSectionsB, sizeof(char*));

    char* fileNameACopy = (char*) malloc(strlen(fileNameA));
    char* fileNameBCopy = (char*) malloc(strlen(fileNameB));

    strcpy(fileNameACopy,fileNameA);
    strcpy(fileNameBCopy,fileNameB);

    char* sectionA = strtok(fileNameACopy,"_.");
    i = 0;
    while (sectionA != NULL)
    {
        fileAsections[i] = (char*) malloc(strlen(sectionA));
        strcpy(fileAsections[i],sectionA);
        i++;
        sectionA = strtok(NULL,"_.");
    }

    char* sectionB = strtok(fileNameBCopy,"_.");
    i = 0;
    while (sectionB != NULL)
    {
        fileBsections[i] = (char*) malloc(strlen(sectionB));
        strcpy(fileBsections[i],sectionB);
        i++;
        sectionB = strtok(NULL,"_.");
    }

    int j = 0;
    int anySimilar = 0;
    printf("\nSIMILARITY: %s and %s similar sections: ",fileNameB,fileNameA);
    for (i = 0; i < totalSectionsA + 1; i++)
    {
        anySimilar = 0;
       // printf("\n-----------------");
        for (j = 0; j < totalSectionsB + 1; j++)
        {
           //printf("\nA: %s B: %s", fileAsections[i], fileBsections[j]);
            if (strcmpi(fileAsections[i],fileBsections[j]) == 0)
            {
                //printf(" (1)");
                //printf("%s ", fileAsections[i]);
                similarity++;
                anySimilar = 1;
            }

        }
        if (!anySimilar)
        {
            similarity--;
        }
        //printf("\n-----------------\n");
    }
   // printf("  Total: %d\n", similarity);

    free(fileAsections);
    free(fileBsections);


    return similarity;
}
