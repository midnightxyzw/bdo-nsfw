#ifndef UTILITIES_H
#define UTILITIES_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <pthread.h>
#include "../include/data_types.h"
#include "../include/file_operations.h"
#include "../include/meta_explorer.h"



typedef enum {RED,YELLOW,GREEN,PINK,CYAN,MAGENTA,BLUE,DARK_GREEN,WHITE} Color;
void resizeWindow(int width, int height);
void printMainHeader();
long locateHash(long hash,MemFileInt* metaFileInMemory,MetaFileInfo* metaFileInfo);
char* concatenate (char* str1, char*str2); // Merges 2 strings into 1
char* concatenate3(char* str1, char*str2, char* str3);
char* endian_convert(int num);
int hexToInt(char* hex);
char* intToHex(int x);
void printColor(char* stringToPrint, Color COLOR);
void printColorBackground(char* stringToPrint, Color COLOR);
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
int containsStrict(char* fullString, char* substring);
int beginsWith(char* fullString, char* substring);
int containsStartEnd(char* fullString, char* substring, int* start, int* end);
char* getClassPrefix(char* fileName);
char* removeClassPrefix(char* fileName);
char* getLastFolder(char* folderName);

void createLogFile();
char* bdoRootFolder();
void generateReferenceFile();
int metaFileChangedSize();
int createMenu(MenuElements menu);
char* createArrowMenu(MenuElements menu, int* cursor);
int compare(void const *a, void const *b);
FileBlock* binarySearchFileBlockByHash(long hash, FileBlock* fileBlocks, int fileblocksCount);
FileBlock* binarySearchFileBlock(char* key, FileBlock* fileBlocks, int fileblocksCount);
int binarySearchByHash(long key,long* hashes, int hashesCount);
FileBlock** getTexturesFrom(FileBlock* fileBlock, char* fileLocation, int* return_texture_count, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);
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

void extractTexturesFrom(FileBlock* fileBlockSelected, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo, char* fileBlockExtractedLocation, int openAfterExtracted);

void copyBlankFiles(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);

int extractFileCustomPath(FileBlock* fileBlockSelected, char* extractionLocation, int echo);

void getFilesFromTexture(char* textureName, FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);

int isAFemaleFile(char* fileName);
int isAMaleFile(char* fileName);
int haveTheSameGender(char* fileName1, char* fileName2);

char* getOperationName(int operation);

int isPatched(FileBlock* fileBlock, MetaFileInfo* metaFileInfo, FILE* metaFile);

int isStockingTexture(char* fileName);

long countPatchedFiles(FileBlock* fileBlocks, MetaFileInfo* metaFileInfo);
int isClean(char* metaFileName);
void listPatchedFiles(FileBlock* fileBlocks,MetaFileInfo* metaFileInfo);

void preventFileRecheck(int forced);

int getWindowsVersion();
int isALetter(char c);
int isANumber(char c);

void openFolder(char* folderToOpen);
char* removeFileExt(char* fileName);
char* askExtractLocation(char** previousExtractLocation);
char* promptNewExtractionLocation();

extern char* extractionLocation;
extern int finishedExtracting;
extern long totalFiles;
extern long timeToExtract;
extern long filesCount;

void *multiThreadExtraction(void *thread_argument);
long extractAllGameFiles(char* _extractionLocation, long totalFiBlocks, pthread_t* thread,pthread_attr_t* attr);


int isRegular(char* fileName);
#endif // UTILITIES_H
