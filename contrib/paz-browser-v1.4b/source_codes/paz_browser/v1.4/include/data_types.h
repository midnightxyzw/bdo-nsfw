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
    int isAFolder;
    int isSelected;
    int isNew;
    char* RealName;
    int partNameAlreadyAssined;
    int hasRealName;
    int isNonCashShopItem;
    int triedToUpdateRealName;
    int isClassExclusive;
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
    int option[CUSTOMIZE_MENU_N_OPTIONS];
    int pubicHair[PUBIC_HAIR_N_OPTIONS];
    int transparentKibelius;
    int kibeliusWings;
    int weaponToRemove[TOTAL_CLASSES];
    int removeWeaponInOnly[TOTAL_CLASSES];
    BodyPartSize bodyPartSize[BODY_SIZE_N_OPTIONS];
} PatcherState;

typedef struct
{
    char** fileName;
    int marked_red;
} PazBrowserPatcherState;

typedef struct
{
    long pazCount;
    long fileBlocksCount;
    long fileBlocksStart;
    long fileBlocksEnd;
    char** folderNames;
    int folderNamesCount;
    int pacFileBlocksStart;
    int pacFileBlocksEnd;
    int textureFileBlocksStart;
    int textureFileBlocksEnd;
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
