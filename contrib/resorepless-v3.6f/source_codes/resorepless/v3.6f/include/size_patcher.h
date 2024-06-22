#ifndef BREAST_SIZE_PATCHER_H
#define BREAST_SIZE_PATCHER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/global.h"
#include "../include/utilities.h"
#include "../include/data_types.h"
#include "../include/file_operations.h"

void patchBreastSizePatcherFiles(FileBlock* filesToPatch, long filesToPatchCount, PatcherState* patcherState);
void writeNewBodyValues(FILE* xmlFile, float newValue, long start, long end, char* boneName);
void extractBreastSizePatcherFiles(FileBlock* filesToPatch, int filesToPatchCount, int echo);
int validateMinMaxValues(long minStart, long minEnd, long defaultStart, long defaultEnd, long maxStart, long maxEnd, char* fileName, char* boneName);
void checkExtractedFile(FileBlock* fileToPatch);
#endif // BREAST_SIZE_PATCHER_H
