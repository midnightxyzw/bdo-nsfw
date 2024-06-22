#include "../include/size_patcher.h"


int validateMinMaxValues(long minStart, long minEnd, long defaultStart, long defaultEnd, long maxStart, long maxEnd, char* fileName, char* boneName)
{
    if (minStart == 0
    || minEnd == 0
    || defaultStart == 0
    || defaultEnd == 0
    || maxStart == 0
    || maxEnd == 0
    || (minStart == minEnd)
    || (defaultStart == defaultEnd)
    || (maxStart == maxEnd))
    {

        printf("\nFile      :%s", fileName);
        printf("\nBoneName  : %s\n", boneName);
        printf("\nmin start : %ld", minStart);
        printf("\nmin end   : %ld\n", minEnd);
        printf("\ndefault start: %ld", defaultStart);
        printf("\ndefault end  : %ld\n", defaultEnd);
        printf("\nmax start: %ld", maxStart);
        printf("\nmax end: %ld\n", maxEnd);
        printf("\nSomething went wrong when trying to fix min default and max values in the file below");
        PAUSE();
        return 0;
    }
    return 1;
}

void patchBreastSizePatcherFiles(FileBlock* filesToPatch, long filesToPatchCount, PatcherState* patcherState)
{

    int i = 0;
    for (i = 0; i < filesToPatchCount; i++)
    {
        if (contains(filesToPatch[i].fileName,"customizationboneparamdesc"))
        {
            printf("\n%s ", filesToPatch[i].fileName);
            char* xmlFileFullPath = concatenate3(resoreplessFolder(),filesToPatch[i].folderName,filesToPatch[i].fileName);

             // Edit XML values
            if (fileExists(xmlFileFullPath))
            {
                FILE* xmlFile = openFile(xmlFileFullPath,"rb+");

                long start = 0;
                long end = 0;


                char* wordBetweenQuotes;
                long minStart = 0;
                long minEnd = 0;

                long defaultStart = 0;
                long defaultEnd = 0;

                long maxStart = 0;
                long maxEnd = 0;

                while(1)
                {
                    // Extracts the string between two quote marks
                    wordBetweenQuotes = fileTokMultipleTags(xmlFile,"\"= ",&start,&end);

                    if (wordBetweenQuotes == NULL)
                    {
                        break;
                    }
                    else
                    {
                         //printf("\n%s", wordBetweenQuotes);
                         if (strcmpi(wordBetweenQuotes,"Min") == 0)
                         {
                            free(wordBetweenQuotes);
                            wordBetweenQuotes = fileTokMultipleTags(xmlFile,"\"",&start,&end);
                            //printf("\nMin: %s", wordBetweenQuotes);
                            minStart = start;
                            minEnd = end;
                         }
                         if (strcmpi(wordBetweenQuotes,"Default") == 0)
                         {
                            free(wordBetweenQuotes);
                            wordBetweenQuotes = fileTokMultipleTags(xmlFile,"\"",&start,&end);
                            //printf("\nDefault: %s", wordBetweenQuotes);
                            defaultStart = start;
                            defaultEnd = end;
                         }
                         if (strcmpi(wordBetweenQuotes,"Max") == 0)
                         {
                            free(wordBetweenQuotes);
                            wordBetweenQuotes = fileTokMultipleTags(xmlFile,"\"",&start,&end);
                           // printf("\nMax: %s", wordBetweenQuotes);
                            maxStart = start;
                            maxEnd = end;
                         }

                        if (strcmpi(wordBetweenQuotes,"BoneName") == 0)
                        {
                            free(wordBetweenQuotes);
                            wordBetweenQuotes = fileTokMultipleTags(xmlFile,"\"",&start,&end);

                            if (strcmpi(wordBetweenQuotes,"Bip01 L Breast") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[BREASTS].min_size != MIN_DEFAULT_SIZE)
                                    {

                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BREASTS].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[BREASTS].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BREASTS].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[BREASTS].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BREASTS].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }
                            else if (strcmpi(wordBetweenQuotes,"Bip01 R Breast") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[BREASTS].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BREASTS].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[BREASTS].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BREASTS].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[BREASTS].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BREASTS].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }


                            else if (strcmpi(wordBetweenQuotes,"Bip01 L Hip") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[BUTT].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BUTT].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[BUTT].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BUTT].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[BUTT].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BUTT].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }
                            else if (strcmpi(wordBetweenQuotes,"Bip01 R Hip") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[BUTT].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BUTT].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[BUTT].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BUTT].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[BUTT].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[BUTT].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }

                            else if (strcmpi(wordBetweenQuotes,"Bip01 L Thigh") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[THIGHS].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[THIGHS].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[THIGHS].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[THIGHS].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[THIGHS].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[THIGHS].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }
                            else if (strcmpi(wordBetweenQuotes,"Bip01 R Thigh") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[THIGHS].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[THIGHS].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[THIGHS].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[THIGHS].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[THIGHS].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[THIGHS].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }

                            else if (strcmpi(wordBetweenQuotes,"Bip01 L Calf") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[LEGS].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[LEGS].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[LEGS].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[LEGS].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[LEGS].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[LEGS].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }
                            else if (strcmpi(wordBetweenQuotes,"Bip01 R Calf") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[LEGS].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[LEGS].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[LEGS].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[LEGS].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[LEGS].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[LEGS].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }

                            else if (strcmpi(wordBetweenQuotes,"Bip01 Pelvis") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[PELVIS].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[PELVIS].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[PELVIS].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[PELVIS].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[PELVIS].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[PELVIS].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }
                            else if ((strcmpi(wordBetweenQuotes,"Bip01 Spine") == 0) /*|| (strcmpi(wordBetweenQuotes,"Bip01 Spine1") == 0) || (strcmpi(wordBetweenQuotes,"Bip01 Spine2") == 0)*/)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[SPINE].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[SPINE].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[SPINE].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[SPINE].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[SPINE].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[SPINE].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }


                            else if (strcmpi(wordBetweenQuotes,"Bip01 L UpperArm") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[ARMS].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[ARMS].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[ARMS].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }
                            else if (strcmpi(wordBetweenQuotes,"Bip01 R UpperArm") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[ARMS].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[ARMS].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[ARMS].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }

                            else if (strcmpi(wordBetweenQuotes,"Bip01 L Forearm") == 0)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[ARMS].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[ARMS].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[ARMS].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }
                            else if ((strcmpi(wordBetweenQuotes,"Bip01 R Forearm") == 0)/* || (strcmpi(wordBetweenQuotes,"Bip01 LUpArmTwist1") == 0) || (strcmpi(wordBetweenQuotes,"Bip01 RUpArmTwist1") == 0) || (strcmpi(wordBetweenQuotes,"Bip01 LUpArmTwist") == 0) || (strcmpi(wordBetweenQuotes,"Bip01 RUpArmTwist") == 0)*/)
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[ARMS].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[ARMS].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[ARMS].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[ARMS].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }
                            /*else if ((strcmpi(wordBetweenQuotes,"Bip01 R Hand") == 0) || (strcmpi(wordBetweenQuotes,"Bip01 L Hand") == 0))
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[HAND].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[HAND].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[HAND].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[HAND].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[HAND].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[HAND].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }
                            else if ((strcmpi(wordBetweenQuotes,"Bip01 L Foot") == 0) || (strcmpi(wordBetweenQuotes,"Bip01 R Foot") == 0))
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[FOOT].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[FOOT].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[FOOT].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[FOOT].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[FOOT].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[FOOT].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }
                            else if ((strcmpi(wordBetweenQuotes,"Bip01 Neck01") == 0))
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[NECK].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[NECK].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[NECK].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[NECK].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[NECK].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[NECK].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }
                             else if ((strcmpi(wordBetweenQuotes,"Bip01 R Clavicle") == 0) || (strcmpi(wordBetweenQuotes,"Bip01 L Clavicle") == 0))
                            {
                                if (validateMinMaxValues(minStart, minEnd, defaultStart, defaultEnd, maxStart, maxEnd, filesToPatch[i].fileName, wordBetweenQuotes))
                                {
                                    if (patcherState->bodyPartSize[CLAVICLE].min_size != MIN_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[HAND].min_size, minStart,minEnd,wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[CLAVICLE].default_size != DEFAULT_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[HAND].default_size, defaultStart, defaultEnd, wordBetweenQuotes);
                                    }

                                    if (patcherState->bodyPartSize[CLAVICLE].max_size != MAX_DEFAULT_SIZE)
                                    {
                                        writeNewBodyValues(xmlFile,patcherState->bodyPartSize[HAND].max_size, maxStart, maxEnd,wordBetweenQuotes);
                                    }
                                }
                            }*/

                            else
                            {
                                //printf(" - No match");
                            }
                            minStart = 0;
                            minEnd = 0;

                            defaultStart = 0;
                            defaultEnd = 0;

                            maxStart = 0;
                            maxEnd = 0;
                        }

                        free(wordBetweenQuotes);
                     }

                }
                fclose(xmlFile);
            }
            else
            {
                printf("\n File %s doesn't exist", xmlFileFullPath);
            }
            free(xmlFileFullPath);
        }
    }
}

void writeNewBodyValues(FILE* xmlFile, float newValue, long start, long end, char* boneName)
{
  long backupPos = ftell(xmlFile);
    long length = end - start;
    if (length < 14)
    {
        printf(": %s", boneName);
        printColor("space between quotes is too small to fit the new numbers. This change will not work", YELLOW);
        //printf("start%ld, end: %ld  - length = %ld", start, end, length);
        //PAUSE();
        return;
        length = 14;
    }
    char* newValues = (char*) malloc(length * sizeof(char));
    int i = 0;
    for(i = 0; i < length; i++)
    {
        newValues[i] = ' ';
    }

    if (newValue < -0.04)
    {
        sprintf(newValues,"%.1f %.1f %.1f",newValue,newValue,newValue);
    }
    else
    {
        sprintf(newValues,"%.2f %.2f %.2f",newValue,newValue,newValue);
    }

    fseek(xmlFile,start,SEEK_SET);

    if (fwrite(newValues,strlen(newValues),1,xmlFile) == 0)
    {
        printf("\nFailed to write on xml file.");
    }



    free(newValues);
    fseek(xmlFile,backupPos,SEEK_SET);
}

void extractMultipleFiles(char* listOfFileNames, int pazNum)
{
    char pazName[13];
    sprintf(pazName,"PAD%.5d.PAZ", pazNum);
    char* command = (char*) malloc(strlen("cd patcher_resources\\ & quickbms.exe 1>NULL 2>NULL2 -Y -f \"*%s*\" blackdesert.bms ../%s \"../../") + strlen(listOfFileNames) + strlen(pazName) + 10 + strlen(resoreplessFolder()));

    sprintf(command,"cd patcher_resources\\ & quickbms.exe 1>NULL 2>NULL2 -Y -f \"%s\" blackdesert.bms ../%s \"../%s", listOfFileNames, pazName, resoreplessFolder());
    //printf("\ncommand: %s\n", command);


    system(command);
}

void extractBreastSizePatcherFiles(FileBlock* filesToPatch, int filesToPatchCount, int echo)
{
    char* customizationboneparamdescFiles = NULL;
    int pazNum = 0;
    int differentPaz = 1;

    int i = 0;
    for (i = 0; i < filesToPatchCount; i++)
    {
        if (strstr(filesToPatch[i].fileName,"customizationboneparamdesc"))
        {
            if (pazNum == 0)
            {
                pazNum = filesToPatch[i].pazNum;
            }

            if (filesToPatch[i].pazNum != pazNum)
            {
                differentPaz++;
            }

            char* custboneparFile =  malloc(strlen(filesToPatch[i].fileName) + strlen(",**" + 1));
            if (customizationboneparamdescFiles == NULL)
            {
                sprintf(custboneparFile,"*%s*",filesToPatch[i].fileName);
                customizationboneparamdescFiles = custboneparFile;
            }
            else
            {
                sprintf(custboneparFile,",*%s*",filesToPatch[i].fileName);
                customizationboneparamdescFiles = concatenate(customizationboneparamdescFiles,custboneparFile);
                free(custboneparFile);
            }
        }
    }
    printf("\nExtracting customizationboneparamdesc files from PAD%.5d.PAZ",  pazNum);
    extractMultipleFiles(customizationboneparamdescFiles, pazNum);

    int* pazNumsAlreadyChecked = (int*) calloc(differentPaz, sizeof(int));
    pazNumsAlreadyChecked[0] = pazNum;

    int j = 0;
    for (i = 0; i < filesToPatchCount; i++)
    {
        if (strstr(filesToPatch[i].fileName,"customizationboneparamdesc"))
        {
            if (filesToPatch[i].pazNum != pazNum)
            {
                int alreadyChecked = 0;
                for (j = 0; j < differentPaz; j++) {
                    if (pazNumsAlreadyChecked[j] == filesToPatch[i].pazNum) {
                        alreadyChecked = 1;
                        break;
                    }
                }
                if (!alreadyChecked) {
                    printf("\nExtracting customizationboneparamdesc files from PAD%.5ld.PAZ",  filesToPatch[i].pazNum);
                    extractMultipleFiles(customizationboneparamdescFiles, filesToPatch[i].pazNum);
                    pazNum = filesToPatch[i].pazNum;
                    pazNumsAlreadyChecked[j] = pazNum;
                }
            }
        }
    }
    free(pazNumsAlreadyChecked);
    free(customizationboneparamdescFiles);
}

void checkExtractedFile(FileBlock* fileToPatch)
{
    char* root = resoreplessFolder();

    char* xmlFileFolderPath = concatenate(root,fileToPatch->folderName);
    char* xmlFileFullPath = concatenate(xmlFileFolderPath,fileToPatch->fileName);


    if (!fileExists(xmlFileFullPath))
    {
        extractFileFromPaz(fileToPatch->fileName,fileToPatch->pazNum,root, 0);

        printf("\n\nxmlFileFullPath: %s\n\n",xmlFileFullPath);

        if (!fileExists(xmlFileFullPath))
        {
            if (displayedErrorOnce == 0)
            {
                extractFileFromPaz(fileToPatch->fileName,fileToPatch->pazNum,root, 1);
            }

            if (!fileExists(xmlFileFullPath))
            {

                if (displayedErrorOnce == 0)
                {
                    printf("\nThe file \"%s\" should be in the file PAD%.5ld.PAZ\nbut it isn't or the .PAZ file simply doesn't exist in your \"Paz\\\" folder.\n", fileToPatch->fileName,fileToPatch->pazNum);
                    printColor("\nFailed to extract ", YELLOW);printf("%s. Ignoring file.\n", fileToPatch->fileName);
                    printf("\nSee above for more information.\n\n");
                    displayedErrorOnce = 1;
                }
                else
                {
                    printf("\n%s ", fileToPatch->fileName);
                    printf(" - ");printColor("Failed to extract ", YELLOW);printf(" - Ignoring file.\n");
                }
                fileToPatch->needPatch = 0;
            }
        }
    }


    free(xmlFileFolderPath);
    free(xmlFileFullPath);
    free(root);
}
