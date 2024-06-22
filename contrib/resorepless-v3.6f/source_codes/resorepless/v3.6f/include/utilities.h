#ifndef UTILITIES_H
#define UTILITIES_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include "../include/data_types.h"
#include "../include/file_operations.h"
#include "../include/meta_explorer.h"
#include "../include/patcher.h"

typedef struct
{
    long value;
    long length;
} PubicHairOffset;

typedef enum {RED,YELLOW,GREEN} Color;

void resizeWindow(int width, int height);
void runMetaInjector();
int compare_meta_offset(void const *a, void const *b) ;
int compare_need_patch(void const *a, void const *b);
int sort_by_folder_name_and_file_name(void const *a, void const *b);
int compare_middle_digits(void const *a, void const *b);
void printMainHeader();
long locateHash(long hash,MemFileInt* metaFileInMemory,MetaFileInfo* metaFileInfo);
char* concatenate (char* str1, char*str2); // Merges 2 strings into 1
char* concatenate3(char* str1, char*str2, char* str3);
char* endian_convert(int num);
int hexToInt(char* hex);
char* intToHex(int x);
void printColor(char* stringToPrint, Color COLOR);
void addToStringArray(char* fileNameToAdd, char*** ref_stringArray, int* ref_arrraySize);
int alreadyInStringArray(char* fileName,char** stringArray, int arrraySize);
long locateIntInMemory(int intToLocate,MemFileInt* arrayInMemory, MetaFileInfo* metaInfo);
void charReplace(char* str,char token,char replace);
char* substr(char* str,int start, int length);
void printFileBlock(FileBlock fileBlock);
int indexOf(char token, char* str, int skips);
char** getBackupList(long* backupCount);
int selectBackup(char** backupNames, long backupCount);
void createBackup();
int backupExists();
char* getLatestBackup();
void restoreBackup(char* backupName);
int isNumOrLetter(char c);
void undoLastChanges();
void createPath(char* pathToCreate);
void askConfirmation();
char* getTwoDigits(char* fileName);
char* getMiddleDigits(char* fileName);
char* getFourDigits(char* fileName);
int contains(char* fullString, char* substring);
int beginsWith(char* fullString, char* substring);
int containsStrict(char* fullString, char* substring);
int containsStartEnd(char* fullString, char* substring, int* start, int* end);
char* getClassPrefix(char* fileName);
char* removeClassPrefix(char* fileName);
char* getLastFolder(char* folderName);
char* bdoRootFolder();
char* resoreplessFolder();
void generateReferenceFile();
int metaFileChangedSize();
int createMenu(MenuElements menu);
char* createArrowMenu(MenuElements menu, int* cursor);
int compare(void const *a, void const *b);
FileBlock* binarySearchFileBlock(char* key, FileBlock* fileBlocks, int fileblocksCount);
FileBlock* getTexturesFrom(FileBlock* fileBlock, char* fileLocation, int* return_texture_count, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);
void backupMenu();

void PAUSE();
PatcherState* loadPatcherState(char* patcherStateFileName);
void savePatcherState(PatcherState* patcherState,char* patcherStateFileName);

char* getClassAsString(FileBlock* fileBlock);
char* getArmorPartAsString(FileBlock* fileBlock);

FileBlock* searchFileBlock(char* fileName, FileBlock* fileBlocks, int fileBlockCount);
FileBlock* searchFileBlockcontains(char* fileName, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);

int extractFile(FileBlock* fileBlockSelected);
void extractFileFromPaz(char* fileName, long pazNum, char* dest, int echo);

int getClassMacro(char* fileName);
char* captalizeFirstLetter(char* str);

PazBrowserPatcherState* load_paz_browser_patcher_state(int* paz_browser_patcher_state_n_files, int operation);
void save_paz_browser_patcher_state(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, int operation);
int getFileColor(long fileHash, int operation);

void clearFileBlocks(FileBlock* fileBlocks, MetaFileInfo *metaFileInfo);

void extractTexturesFrom(FileBlock* fileBlockSelected, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, int openAfterExtracted);

void copyBlankFiles(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, char* destFolder);

int extractFileCustomPath(FileBlock* fileBlockSelected, char* extractionLocation, int echo);

void getFilesFromTexture(char* textureName, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);

int isAFemaleFile(char* fileName);
int isAMaleFile(char* fileName);
int haveTheSameGender(char* fileName1, char* fileName2);

char* getOperationName(int operation);

//int isPatched(FileBlock* fileBlock, MetaFileInfo* metaFileInfo, FILE* metaFile);

//long countPatchedFiles(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);
//int isClean(char* metaFileName);
//void listPatchedFiles(FileBlock* fileBlocks,MetaFileInfo* metaFileInfo);

void preventFileRecheck(int forced);

int getWindowsVersion();
int isALetter(char c);

char* classMacroToPrefix(int classMacro);

int getPenisMacro(char* fileName);
int getPubicHairMacro(char* fileName);
int haveSameClass(char* fileNameA, char* fileNameB);

void printRename(char* fileNameA, char* fileNameB, int longestLength);

char* getLatestFolder(char* folderPath);

char* replaceExt(char* fileName,char* newExt);

//void createDDSBin(char* pathToDDS);
void reconstructDDS(char* pubicHairFolder, char* pubicHairTextureName, char* characterTextureFolder, char* targetTextureName);

char** readCommaSeparatedFile(char* pathToFile, int* totalEntriesReturn);
void generateTextureBinFiles();

void printCursor(int cursor, int valueToCompare);
void printOnOffSwitch(int variable);
void generateTextureBinFiles();
int similarity(char* fileNameA, char* fileNameB);
#endif // UTILITIES_H
