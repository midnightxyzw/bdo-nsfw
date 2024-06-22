#ifndef PATCHER_H
#define PATCHER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/global.h"
#include "../include/utilities.h"
#include "../include/file_operations.h"
#include "../include/meta_explorer.h"
#include "../include/partcutdesc_generator.h"
#include "../include/paz_browser.h"
#include "../include/size_patcher.h"


void runPatcher(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo,PatcherState* patcherOptions);
FileBlock** markFilesToPatch(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, PatcherState* patcherState,int* filesToPatchCount);
//void addToFilesToPatch(FileBlock* fileBlockFound, FileBlock** filesToPatch, int* filesToPatchCount);

void patchMetaFile(FileBlock* filesToPatch, int filesToPatchCount, int operation, MetaFileInfo* metaFileInfo, int echo);
void copyPatcherResourcesFiles(FileBlock** filesToPatch, int filesToPatchCount, PatcherState* patcherState);
void modifyXMLFiles(FileBlock** filesToPatch, long filesToPatchCount, PatcherState* patcherState);

char*** getStarters();
int isStockingTexture(char* fileName);
void copyNudeTextures(PatcherState* patcherState);
int isRegular(char* fileName);
int isStarter(char* fileName);
int isPantieFile(char* fileName);
int isPantsFile(char* fileName);
int isCensorshipTextureFile(char* fileName);
int isCensorshipPACFile(char* fileName);
void validatePatcherResourcesFiles(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);
void printFileBeingCopied(FileBlock* fileToPatch,long filesToPatchCount, char** newclass_prefix, char** old_class_prefix, int i);
int bodyPartsSizeChanged(PatcherState* patcherState);
void weaponOptionRequirePatch(FileBlock* fileBlock, PatcherState* patcherState);
int isATreeDummerFile(char* fileName);
int isAFemaleNudeTexture(char* fileName, int use3DVaginas);
int isAFemaleNudeModel(char* fileName);
int isAMaleNudeTexture(char* fileName);
int isAMaleNudeModel(char* fileName);
void patchFromFile(int operation, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo,PatcherState* patcherState);
int canPatchClass(char* fileName, PatcherState* patcherState);
int needToPatchPartcutdesc(PatcherState* patcherState);
int needToPatchPartcutdesc(PatcherState* patcherState);
#endif

