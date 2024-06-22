#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include <stdio.h>
#include <stdlib.h>
#include "../include/global.h"

typedef struct
{
    long hash;
    long folderNum;
    long fileNum;
    long pazNum;
    long metaOffset;
    long fileOffset;
    long zsize;
    long size;
    char* folderName;
    char* fileName;
    char* originalPath;
    int needPatch;
    int changeID;
    int isPatched;
    int hasRealName;
    char* RealName;
    int triedToUpdateRealName;
    int isNonCashShopItem;
    int partNameAlreadyAssined;
} FileBlock;

typedef struct
{
    char* bytes;
    long size;
} MemFile;


typedef struct
{
    float min_size;
    float default_size;
    float max_size;
} BodyPartSize;


typedef struct
{
    int option[9];
    int pubicHair[5];
    int transparentKibelius;
    int weaponToRemove[13];
    int removeWeaponInOnly[13];
    BodyPartSize bodyPartSize[6];
} PatcherState_2_8;

typedef struct
{
    int option[9];
    int pubicHair[5];
    int transparentKibelius;
    int kibeliusWings;
    int weaponToRemove[13];
    int removeWeaponInOnly[13];
    BodyPartSize bodyPartSize[7];
} PatcherState_3_0;


typedef struct
{
    int option[9];
    int pubicHair[5];
    int transparentKibelius;
    int kibeliusWings;
    int weaponToRemove[14];
    int removeWeaponInOnly[14];
    BodyPartSize bodyPartSize[7];
} PatcherState_3_2;

typedef struct
{
    int option[CUSTOMIZE_MENU_N_OPTIONS];
    int pubicHair[9];
    int transparentKibelius;
    int kibeliusWings;
    int weaponToRemove[15];
    int removeWeaponInOnly[15];
    BodyPartSize bodyPartSize[BODY_SIZE_N_OPTIONS];
    int modAppliesToClass[15];
    int penisType[TOTAL_MALE_CLASSES];
} PatcherState_3_4_c;


typedef struct
{
    int option[CUSTOMIZE_MENU_N_OPTIONS];
    int pubicHair[TOTAL_FEMALE_CLASSES];
    int transparentKibelius;
    int kibeliusWings;
    int weaponToRemove[TOTAL_CLASSES];
    int removeWeaponInOnly[TOTAL_CLASSES];
    BodyPartSize bodyPartSize[BODY_SIZE_N_OPTIONS];
    int modAppliesToClass[TOTAL_CLASSES];
    int penisType[TOTAL_MALE_CLASSES];
} PatcherState;




typedef struct
{
    char** fileName;
    int marked_red;
} PazBrowserPatcherState;

typedef struct
{
    long pazCount;
    long filesCount;
    long fileBlocksCount;
    long fileBlocksStart;
    long fileBlocksEnd;
    long originalFileBlocksStart;
    long originalFileBlocksEnd;
    long actualFileBlocksCount;
    long missingFilesCount;
} MetaFileInfo;

typedef struct
{
    int* block;
    long size;
} MemFileInt;

typedef struct
{
    char header[256];
    char menuoptions[256][2048];
    int nOptions;
} MenuElements;

typedef struct
{
    char* fileName;
    int patched;
    int totalFails;
} FileToPatch;

#endif // DATA_TYPES_H
