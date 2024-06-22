#include "../include/file_operations.h"

// Returns a string like this: pathToFile/fileName
char* buildFullPath(char* pathToFile, char* fileName)
{
    char* fullPath = (char*)malloc(((strlen(pathToFile) + strlen(fileName)) * sizeof(char)) + 2);
    fullPath[0] = '\0';

    strcpy(fullPath,pathToFile);
    strcat(fullPath,fileName);

    if (pathToFile[strlen(pathToFile) - 1] != '\\')
    {
        pathToFile = (char*) realloc(pathToFile,strlen(pathToFile) + 1);
        pathToFile[strlen(pathToFile) - 1] = '\\';
    }

    return fullPath;
}
int copyBytes(FILE* scrFile, FILE* destFile, int howManyBytes)
{
    int i = 0;
    char buffer = '\0';
    int nReadBytes = 0;
    int nWrittenBytes = 0;

    while (i < howManyBytes)
    {
        int readResult = fread(&buffer,1,1,scrFile);
        int writeResult = fwrite(&buffer,1,1,destFile);
        nReadBytes += readResult;
        nWrittenBytes += writeResult;
        i++;
    }

    if (nReadBytes != howManyBytes)
    {
       printf("\nError: Failed to read bytes from file (%d read, %d expected)\n", nReadBytes, howManyBytes);
       return 0;
    }
    if (nWrittenBytes != howManyBytes)
    {
      printf("\nError: Failed to write bytes to file (%d written, %d expected)\n",nWrittenBytes, howManyBytes);
        return 0;
    }
    return 1;
}

void copyFile(char* srcFileName, char* cpyFileName)
{
    char* currentPath = getCurrentPath();
    copyFileTo(srcFileName,cpyFileName,currentPath,currentPath,0);
}


// Copies an entire file, byte per byte, from a folder, to another
int copyFileTo(char* srcFileName, char* cpyFileName, char* src_folder,char* dest_folder, int echo)
{

    if (echo == 1)
    {
        printf("\nCOPY\n\"%s%s\"\nto\n\"%s%s\"\n",src_folder,srcFileName,dest_folder, cpyFileName );
    }
    if (echo == -1)
    {
        printf("%s",srcFileName);
    }

    createPath(dest_folder);
    char buffer; // Used to read each byte from the sourceFile
    size_t totalBytesRead = 0;
    size_t totalBytesWritten = 0;
    size_t bytesRead = 0; // Stores the return value of the function fread, when it's called

    char* src_path = concatenate(src_folder,srcFileName);
    char* dest_path = concatenate(dest_folder,cpyFileName);

    // Tries to open the source file
    FILE* srcFile = openFile(src_path,"rb");

    if (srcFile == NULL)
    {
        systemCopy(srcFileName,src_folder,dest_folder);
        if(fileExists(dest_path))
        {
            return 1;
        }
        printColor("        Failed to copy", RED);
        printf(" - File doesn't exists or it can't be read.\n\n");
        PAUSE();
        free(src_path);
        free(dest_path);
        fclose(srcFile);
        return 0;
    }

    // Creates an empty copy file
    FILE* cpyFile = fopen(dest_path,"wb");

    if (cpyFile == NULL)
    {
        systemCopy(srcFileName,src_folder,dest_folder);
        if(fileExists(dest_path) && (getFileSizeByName(dest_path) > 0))
        {
            return 1;
        }
        printColor("        Failed to copy", RED);
        printf(" - Failed to create the %s\n\n", dest_path);
        PAUSE();
        free(src_path);
        free(dest_path);
        fclose(srcFile);

        return 0;
    }

    // Reads the source file and copies each byte into the copy file
    while((bytesRead = fread(&buffer,1,1,srcFile))) // While it doesn't reach the end of the srcFile, stores the read byte in the buffer and the amount of bytes read is stored in the bytesRead variable
    {
        totalBytesRead += bytesRead; // Increases the count of the total of bytes read, with the amount of bytes read by the fread function in this loop
        totalBytesWritten += fwrite(&buffer,1,1,cpyFile); // Writes the read byte into the copy file and increases the count of the total of bytes written, with the amount of bytes written by the fwrite function in this loop
    }

    // Checks if it wasn't possible to read any bytes of the src file
    if (totalBytesRead == 0)
    {
        systemCopy(srcFileName,src_folder,dest_folder);
        if(fileExists(dest_path)&& (getFileSizeByName(dest_path) > 0))
        {
            return 1;
        }
        printColor("        Failed to copy\n", RED);
        printf("The file %s is empty or it couldn't be read\n\n", srcFileName);
            PAUSE();
        free(src_path);
        free(dest_path);
        fclose(srcFile);
        fclose(cpyFile);

         return 0;
    }

    // Checks if all the bytes from the src file were written in the copy
    if(totalBytesRead != totalBytesWritten)
    {
         systemCopy(srcFileName,src_folder,dest_folder);
        if(fileExists(dest_path)&& (getFileSizeByName(dest_path) > 0))
        {
            return 1;
        }
        printColor("        Failed to copy\n", RED);
        printf("Something went wrong when making a copy of the %s file - bytes read: %d bytes written: %d\n", srcFileName, totalBytesRead, totalBytesWritten);
            PAUSE();
        free(src_path);
        free(dest_path);
        fclose(srcFile);
        fclose(cpyFile);
        PAUSE();
        return 0;
    }




    if(!fileExists(dest_path)&& (getFileSizeByName(dest_path) > 0))
    {
        systemCopy(srcFileName,src_folder,dest_folder);
        if(fileExists(dest_path)&& (getFileSizeByName(dest_path) > 0))
        {
            free(src_path);
            free(dest_path);
            fclose(srcFile);
            fclose(cpyFile);
            return 1;
        }
    }
    else
    {
            if(echo)
            {
                printf("       1 file(s) copied.");
            }
            printf("\n");

            free(src_path);
            free(dest_path);
            fclose(srcFile);
            fclose(cpyFile);
        return 1;
    }
    printColor("        Failed to copy\n", RED);
    printf("%s", dest_path);
    PAUSE();
    free(src_path);
    free(dest_path);
    fclose(srcFile);
    fclose(cpyFile);

    return 0;
}

/*
// Copies an entire file, byte per byte, from a folder, to another
int copyFileTo(char* srcFileName, char* cpyFileName, char* src_folder,char* dest_folder)
{

    createPath(dest_folder);

    char buffer; // Used to read each byte from the sourceFile
    size_t totalBytesRead = 0;
    size_t totalBytesWritten = 0;
    size_t bytesRead = 0; // Stores the return value of the function fread, when it's called

    char* src_path = concatenate(src_folder,srcFileName);
    char* dest_path = concatenate(dest_folder,cpyFileName);

    // Tries to open the source file
    FILE* srcFile = openFile(src_path,"rb");

    // Creates an empty copy file
    FILE* cpyFile = fopen(dest_path,"wb");

    if (cpyFile == NULL)
    {
        systemCopy(srcFileName,src_folder,dest_folder);
        if(fileExists(dest_path))
        {
            return 1;
        }
        printf("\nFailed to create the %s\n", dest_path);
        free(src_path);
        free(dest_path);
        fclose(srcFile);

        return 0;
    }

    // Reads the source file and copies each byte into the copy file
    while((bytesRead = fread(&buffer,1,1,srcFile))) // While it doesn't reach the end of the srcFile, stores the read byte in the buffer and the amount of bytes read is stored in the bytesRead variable
    {
        totalBytesRead += bytesRead; // Increases the count of the total of bytes read, with the amount of bytes read by the fread function in this loop
        totalBytesWritten += fwrite(&buffer,1,1,cpyFile); // Writes the read byte into the copy file and increases the count of the total of bytes written, with the amount of bytes written by the fwrite function in this loop
    }

    // Checks if it wasn't possible to read any bytes of the src file
    if (totalBytesRead == 0)
    {
        systemCopy(srcFileName,src_folder,dest_folder);
        if(fileExists(dest_path))
        {
            return 1;
        }
        printf("\nThe file %s is empty or it couldn't be read, please restore it with a backup.", srcFileName);
        free(src_path);
        free(dest_path);
        fclose(srcFile);
        fclose(cpyFile);
        return 0;
    }

    // Checks if all the bytes from the src file were written in the copy
    if(totalBytesRead != totalBytesWritten)
    {
        systemCopy(srcFileName,src_folder,dest_folder);
        if(fileExists(dest_path))
        {
            return 1;
        }
        printf("\nSomething went wrong when making a copy of the %s file - bytes read: %d bytes written: %d", srcFileName, totalBytesRead, totalBytesWritten);
        free(src_path);
        free(dest_path);
        fclose(srcFile);
        fclose(cpyFile);
        return 0;
    }

    if (fileExists(concatenate(dest_folder,cpyFileName)))
    {
        printColor("    Success\n",GREEN);
        return 1;
    }
    else
    {
        systemCopy(srcFileName,src_folder,dest_folder);
        if(fileExists(dest_path))
        {
            return 1;
        }
        return 0;
    }

    free(src_path);
    free(dest_path);
    fclose(srcFile);
    fclose(cpyFile);
}*/

MemFile* copyFileToMemoryByName(char* fileName)
{
    FILE* fp = openFile(fileName,"rb");
    MemFile* memFile = copyFileToMemory(fp);
    fclose(fp);
    return memFile;
}

MemFile* copyFileToMemory(FILE* fp)
{
    MemFile* memFile = (MemFile*) malloc(sizeof(MemFile));

    memFile->size = getFileSize(fp);

    memFile->bytes = (char*) malloc(sizeof(long) + (memFile->size * sizeof(char)));
    long i = 0;
    long bytesRead = 0;
    rewind(fp);
    for (i = 0; i < memFile->size; i++)
    {
        bytesRead += fread(&memFile->bytes[i],sizeof(char),1,fp);
    }
    fclose(fp);

    if (bytesRead != memFile->size)
    {
        printf("Error: Bytes read from: %ld expected: %ld",bytesRead, memFile->size);
    }

    return memFile;
}

MemFileInt* copyFileToMemoryAsInt(FILE* fp)
{
    MemFileInt* memFileInt = (MemFileInt*) malloc(sizeof(MemFileInt));
    long arraySize = getFileSize(fp)/sizeof(int);

    memFileInt->size = arraySize;
    memFileInt->block = (int*) malloc(memFileInt->size * sizeof(int));

    int i = 0;
    while(fread(&memFileInt->block[i],sizeof(int),1,fp))
    {
        i++;
    }

    return memFileInt;
}


int countAllFiles(char* startingPath, char* extFilter)
{
    int filesToPatchCount = 0;
    // Opens the current dir
    DIR* pDir = opendir (startingPath);
    struct dirent *pDirent;

    if (pDir == NULL)
    {
        printf ("Cannot open directory: '%s'\n", startingPath);
        PAUSE();
    }

    char completePath[2048];

    // Writes all the file names in this dir, into the file
    while ((pDirent = readdir(pDir)) != NULL)
    {
        char* fileOrFolder = pDirent->d_name;

         //Makes sure it doesn't write stuff like "." or ".." into the file
        if((strcmpi(fileOrFolder,".") != 0) && (strcmpi(fileOrFolder,"..") != 0))
        {
            //Creates the complete path (E.g. from C:\folder goes to c:\folder\something)
            strcpy(completePath,startingPath);
            strcat(completePath,fileOrFolder);

            if (isDirectory(completePath))
            {
                /* If the path was something like c:\folder\something, since it's a folder, we add: c:\folder\something\ */
                strcat(completePath,"\\");
                filesToPatchCount += countAllFiles(completePath, extFilter);
            }
            else
            {
               filesToPatchCount++;
            }
        }
    }

    //Closes the dir and the file
    closedir (pDir);
    return filesToPatchCount;
}

int countFilesSingleFolder(char* pathToFolder, char* extFilter)
{
    int filesToPatchCount = 0;
     // Opens the current dir
    DIR* pDir = opendir (pathToFolder);
    struct dirent *pDirent;

    if (pDir == NULL)
    {
        printf ("Cannot open directory: '%s'\n", pathToFolder);
        PAUSE();
    }

    // Writes all the file names in this dir, into the file
    while ((pDirent = readdir(pDir)) != NULL)
    {
        char* fileOrFolder = pDirent->d_name;

         //Makes sure it doesn't write stuff like "." or ".." into the file
        if((strcmpi(fileOrFolder,".") != 0) && (strcmpi(fileOrFolder,"..") != 0))
        {
            //Creates the complete path (E.g. from C:\folder goes to c:\folder\something)
            char* completePath = buildFullPath(pathToFolder,fileOrFolder);


            if (!isDirectory(completePath))
            {
                if ((strcmpi(extFilter,"*") == 0) || (strcmpi(getFileExt(fileOrFolder),extFilter) == 0))
                {
                    filesToPatchCount++;
                }
            }
        }
    }

    //Closes the dir and the file
    closedir (pDir);
    return filesToPatchCount;
}

int deleteFile(char* pathToFile)
{
    if (fileExists(pathToFile))
    {
        char* command = (char*) malloc(strlen("DEL \"DIR_TO_REMOVE\"") + strlen(pathToFile));
        sprintf(command, "DEL \"%s\"", pathToFile);
        system(command);
        free(command);
        if (!fileExists(pathToFile))
        {
            printf("\nDeleted: %s", pathToFile);
        }
        else
        {
            printf("\nFailed to delete: %s", pathToFile);
        }
    }

    return !fileExists(pathToFile);
}


void deleteFolder(char* pathToFolder, int askForConfirmation)
{
    if (folderExists(pathToFolder))
    {
        char* command = (char*) malloc(strlen("RMDIR \"DIR_TO_REMOVE\" /Q /S") + strlen(pathToFolder));
        if (askForConfirmation)
        {
            sprintf(command, "RMDIR \"%s\" /s", pathToFolder);
        }
        else
        {
            printf("\nDeleting Folder: %s", pathToFolder);
            sprintf(command, "RMDIR \"%s\" /Q /s", pathToFolder);
        }

        system(command);
        free(command);
    }
}


// Exits the program displaying a message before it ends itself.
void exitProgram(const char* message)
{
    printf("\n%s", message);
    printf("\n\nExiting.\n");
    system("PAUSE");
    exit(EXIT_FAILURE); // Exits the program returning 1
}
// Returns 1 if the file with the given name exist in the same folder as this exe and 0 otherwise
int fileExists(char* pathToFile)
{
    // Tries to open the file to read
    FILE* fp = fopen(pathToFile,"rb");

    // If the file exists, fp will be != NULL
    if(fp)
    {
        fclose(fp); // Closes the file so it can be used by other function
        return 1;
    }

    return 0;
}

int fileExistsInFolder(char* folder, char* fileName)
{
    char* filePath = concatenate(folder,fileName);
    // Tries to open the file to read
    FILE* fp = fopen(fileName,"rb");

    // If the file exists, fp will be != NULL
    if(fp)
    {
        free(filePath);
        fclose(fp); // Closes the file so it can be used by other function
        return 1;
    }
    free(filePath);
    return 0;
}

char* fileTok(FILE* fp, char token)
{
    char* isolatedString = NULL;
    int beginning = 0;
    int end = 0;
    char c = '\0';
    long bytesRead = 0;
    long freadReturn = 0;
    int tokensCount = 0;
    while(1)
    {
        freadReturn = fread(&c,1,1,fp);
        if (freadReturn == 0)
        {
             // Reached end of file without finding token
             return NULL;
        }
        bytesRead += freadReturn;

        if (c == token)
        {
            if (tokensCount == 0)
            {
                beginning = ftell(fp);
                tokensCount = 1;
            }
            else if (tokensCount == 1)
            {
                end = ftell(fp) - 1;
                fseek(fp,beginning,SEEK_SET);
                int stringSize = end-beginning;

                isolatedString = (char*) malloc (stringSize + 1);

                fread(isolatedString,stringSize,1,fp);

                isolatedString[stringSize] = '\0';

                tokensCount = 0;
                // Skip this token, we already counted
                fseek(fp,1,SEEK_CUR);

                return isolatedString;
            }
        }
    }
    if (bytesRead == 0)
    {
        printf("\nSomething went wrong, no bytes could be read from file while fileTokening");
        PAUSE();
    }
}


char* fileTokMultipleTags(FILE* fp, char* tags,long* start, long* end)
{
    char* isolatedString = NULL;
    char c = '\0';
    int i = 0;
    long bytesRead = 0;
    long freadReturn = 0;
    int tokensCount = 0;
    while(1)
    {
        freadReturn = fread(&c,1,1,fp);
        if (freadReturn == 0)
        {
             // Reached end of file without finding token
             return NULL;
        }
        bytesRead += freadReturn;

        for(i = 0; i < strlen(tags); i++)
        {
            if (c == tags[i])
            {
                if (tokensCount == 0)
                {
                    (*start) = ftell(fp);
                    tokensCount = 1;
                }
                else if (tokensCount == 1)
                {
                    (*end) = ftell(fp) - 1;
                    fseek(fp,(*start),SEEK_SET);
                    int stringSize = (*end)-(*start);

                    isolatedString = (char*) malloc (stringSize + 1);

                    fread(isolatedString,stringSize,1,fp);

                    isolatedString[stringSize] = '\0';

                    tokensCount = 0;
                    // Skip this token, we already counted
                    fseek(fp,1,SEEK_CUR);

                    return isolatedString;
                }
            }
        }
    }
    if (bytesRead == 0)
    {
        printf("\nSomething went wrong, no bytes could be read from file while fileTokening");
        PAUSE();
    }
}
char** getFilesSingleFolder(char* pathToFiles, char* extFilter, long* totalFiles)
{
    long fileCount = countFilesSingleFolder(pathToFiles,extFilter);

    char** fileNames = (char**) malloc((fileCount + 1) * sizeof(char*));
    (*totalFiles) = 0;

     // Opens the current dir
    DIR* pDir = opendir (pathToFiles);
    struct dirent *pDirent;

    if (pDir == NULL)
    {
        printf ("Cannot open directory: '%s'\n", pathToFiles);
        PAUSE();
    }

    // Writes all the file names in this dir, into the file
    while ((pDirent = readdir(pDir)) != NULL)
    {
        char* fileName = pDirent->d_name;

         //Makes sure it doesn't write stuff like "." or ".." into the file
        if((strcmpi(fileName,".") != 0) && (strcmpi(fileName,"..") != 0))
        {
            //Creates the complete path (E.g. from C:\folder goes to c:\folder\something)
            char* completePath = buildFullPath(pathToFiles,fileName);

            // If it's a file, not a folder
            if (!isDirectory(completePath))
            {
                // If there is no extension restriction or the file we are reading matches the extension restriction
                if ((strcmpi(extFilter,"*") == 0) || (strcmpi(getFileExt(fileName),extFilter) == 0))
                {
                    fileNames[(*totalFiles)] = (char*) malloc(strlen(fileName) + 1); // Allocates the string
                    strcpy(fileNames[(*totalFiles)],fileName); // Copies the file name to that string
                    (*totalFiles)++;
                }
            }
        }
    }

    //Closes the dir and the file
    closedir (pDir);

    return fileNames;
}

FileBlock* getAllFiles(char* pathToFiles, char* extFilter, long* filesCount)
{
    (*filesCount) = 0;
    goInsideAndCount(pathToFiles,extFilter,filesCount);

    if ((*filesCount) == 0)
    {
        printf("No files present in \"%s\"\n\n",pathToFiles);
        system("PAUSE");
        return NULL;
    }

    FileBlock* fileNames = (FileBlock*) calloc((*filesCount) , sizeof(FileBlock));

    long currentFile = 0;

    // This is a recursive function that goes through each file or folder, starting in "startingPath", and if it's a file, it adds it to the "fileNames" array, if it's a folder, it goes inside this folder and does the same thing
    goInside(pathToFiles,&fileNames,extFilter,&currentFile);


    return fileNames;
}

// This is a recursive function that goes through each file or folder, starting in "startingPath", and if it's a file, it adds it to the "fileNames" array, if it's a folder, it goes inside this folder and does the same thing
void goInsideAndCount(char* startingPath, char* extFilter, long* filesCount)
{
    // Opens the current dir
    DIR* pDir = opendir (startingPath);
    struct dirent *pDirent;

    if (pDir == NULL)
    {
        printf ("Cannot open directory: '%s'\n", startingPath);
        PAUSE();
    }

    // Writes all the file names in this dir, into the file
    while ((pDirent = readdir(pDir)) != NULL)
    {
        char* fileName = pDirent->d_name;

         //Makes sure it doesn't write stuff like "." or ".." into the file
        if((strcmpi(fileName,".") != 0) && (strcmpi(fileName,"..") != 0))
        {
            //Creates the complete path (E.g. from C:\folder goes to c:\folder\something)
            char* completePath = buildFullPath(startingPath,fileName);

            // If it's a file, not a folder
            if (!isDirectory(completePath))
            {
                // If there is no extension restriction or the file we are reading matches the extension restriction
                if ((strcmpi(extFilter,"*") == 0) || (strcmpi(getFileExt(fileName),extFilter) == 0))
                {
                    (*filesCount)++;
                }
            }
            else
            {
                strcat(completePath,"\\");
                goInsideAndCount(completePath,extFilter,filesCount);
            }
        }
    }

    //Closes the dir and the file
    closedir (pDir);
}


// This is a recursive function that goes through each file or folder, starting in "startingPath", and if it's a file, it adds it to the "fileNames" array, if it's a folder, it goes inside this folder and does the same thing
void goInside(char* startingPath, FileBlock** fileNames, char* extFilter, long* currentFile)
{
    // Opens the current dir
    DIR* pDir = opendir (startingPath);
    struct dirent *pDirent;

    if (pDir == NULL)
    {
        printf ("Cannot open directory: '%s'\n", startingPath);
        PAUSE();
    }

    // Writes all the file names in this dir, into the file
    while ((pDirent = readdir(pDir)) != NULL)
    {
        char* fileName = pDirent->d_name;

         //Makes sure it doesn't write stuff like "." or ".." into the file
        if((strcmpi(fileName,".") != 0) && (strcmpi(fileName,"..") != 0))
        {
            //Creates the complete path (E.g. from C:\folder goes to c:\folder\something)
            char* completePath = buildFullPath(startingPath,fileName);

            // If it's a file, not a folder
            if (!isDirectory(completePath))
            {
                // If there is no extension restriction or the file we are reading matches the extension restriction
                if ((strcmpi(extFilter,"*") == 0) || (strcmpi(getFileExt(fileName),extFilter) == 0))
                {
                    (*fileNames)[(*currentFile)].fileName = (char*) malloc((strlen(fileName) * sizeof(char)) + 1);

                    strcpy((*fileNames)[(*currentFile)].fileName,fileName); // Copies the file name to that string
                    (*fileNames)[(*currentFile)].fileName[strlen(fileName)] = '\0';

                    (*fileNames)[(*currentFile)].originalPath = (char*) malloc((strlen(startingPath) * sizeof(char)) + 1);

                    strcpy((*fileNames)[(*currentFile)].originalPath,startingPath); // Copies the file name to that string
                    (*fileNames)[(*currentFile)].originalPath[strlen(startingPath)] = '\0';

                    (*currentFile)++;
                }
                free(completePath);
            }
            else
            {
                strcat(completePath,"\\");
                goInside(completePath,fileNames,extFilter,currentFile);
                free(completePath);
            }
        }
    }

    //Closes the dir and the file
    closedir (pDir);
}

// Locates any given array of bytes in a given file, returns the byte offset of the last byte that matched or -1 if it couldn't find it
long getBytesEndingPos(const char* bytesToLocate, FILE* file)
{
    int matchingBytesCount = 0; // Count of how many consecutive matching bytes were found
    char buffer = '\0'; // Used to read each byte from the meta file
    int bytesRead = 0;

    // Goes back at the beginning of the meta file
    rewind(file);
    int result = 0;
    // Searches for the reference bytes
    while((result = fread(&buffer,1,1,file))) // While it doesn't reaches the end of the meta file, keeps reading and storing the read byte in the buffer
    {
        bytesRead += result;
      // If the read byte matches with the current referenceByte, increases the counter
      if (buffer == bytesToLocate[matchingBytesCount])
      {
          matchingBytesCount++;
      }
      else // If the read byte does not match the referenceByte, resets the counter
      {
          matchingBytesCount = 0;
      }

      // If all the bytes from the referenceBytes were matched
      if(matchingBytesCount == 4)
      {
          // Returns the position of the file pointer when the reference bytes ends
           return ftell(file);
      }
    }
    if (bytesRead == 0)
    {
        printf("Warning: No bytes read\n");
    }
    // If the reference bytes cound't be found, it returns -1
    return -1;
}

char* getCurrentPath()
{
   char* cwd = (char*) malloc(2048 * sizeof(char));
   if (getcwd(cwd, 2048) == NULL)
   {
       perror("getcwd() error");
   }
   cwd[strlen(cwd) + 1] = '\0';
   cwd[strlen(cwd)] = '\\';
   return cwd;
}


int fileHasExt(char* fileName, char* ext)
{
    int extStart = strlen(fileName) - strlen(ext);

    int i = 0;
    int j = 0;
    for (i = extStart; i < strlen(fileName); i++)
    {
        if (fileName[i] != ext[j])
        {
            return 0;
        }
        j++;
    }
    return 1;
}

//Returns the file extension without the dot
char* getFileExt(char* fileName)
{
    int i = 0;
    int j = 0;
    int extLen = 0;
    char* ext = (char*) malloc(4 * sizeof(char));
    ext[0] = '\0';

    // Starts from the end of the fileName

    for (i = strlen(fileName) - 1; i >= 0 ; i--)
    {
        // Looks for the first dot, starting from the end
        if (fileName[i] == '.')
        {
            // Alocates memory to store the ext name
            if(extLen > 3)
            {
                ext = (char*) realloc(ext, ((extLen + 2) * sizeof(char)));
            }

            // copies the extension name to the variable
            for (j = 0; j < extLen; j++)
            {
                ext[j] = fileName[i+1+j]; // i + 1 because i is now at the "."
            }
            ext[extLen] = '\0';
            break;
        }
        else
        {
            extLen++;
        }
    }
    return ext;
}

long getFileSize(FILE* fp)
{
    long fileSize = 0;
    long filePosBackup = ftell(fp);

    fseek(fp,0,SEEK_END);
    fileSize = ftell(fp);

    fseek(fp,filePosBackup, SEEK_SET);

    return fileSize;
}

long getFileSizeByName(char* fileName)
{
    FILE* fp = openFile(fileName,"rb");
    long size = getFileSize(fp);
    fclose(fp);
    return size;
}



int isDirectory(const char *completePath)
{
   struct stat statbuf;

   if (stat(completePath, &statbuf) != 0)
   {
       return 0;
   }

   return S_ISDIR(statbuf.st_mode);
}

long locateInt(int intToLocate, FILE* fp)
{
    long fpLocationBackup = ftell(fp);
    int readInt;
    long location = 0;

    rewind(fp);

    while(fread(&readInt,sizeof(int),1,fp))
    {
        if (readInt == intToLocate)
        {
            fseek(fp,fpLocationBackup,SEEK_SET);
            return location;
        }
        location = ftell(fp);
    }

    location = NOT_FOUND;
    fseek(fp,fpLocationBackup,SEEK_SET);
    return location;
}


int matchingBytes(FILE* file1, FILE* file2)
{
    rewind(file1);
    rewind(file2);
    if (getFileSize(file1) != getFileSize(file2))
    {
        return 0;
    }

    char bufferFile1;
    char bufferFile2;
    while(fread(&bufferFile1,1,1,file1))
    {
        fread(&bufferFile2,1,1,file2);

        if (bufferFile1 != bufferFile2)
        {
            return 0;
        }
    }
    return 1;
}

void moveFile(char* fileName, char* src_folder,char* dest_folder)
{
    char* src_path = buildFullPath(src_folder,fileName);
    char* dest_path = buildFullPath(dest_folder,fileName);

    rename(src_path, dest_path);
}

// Returns a string with the path stripped one level down
// Eg: "C:\Black Desert Online\PAZ\" would return "C:\Black Desert Online\"
char* oneLevelDown(char* path)
{
    int lastPos = strlen(path) - 1;
    int subStrLen = 0;
    int i;

    if (path[lastPos] == '\\')
    {
        if ((lastPos - 1) >= 0)
        {
            lastPos--;
        }
    }
    for (i = lastPos; i >= 0; i--)
    {
        if (path[i] == '\\')
        {
            subStrLen = i + 1;
            break;
        }
    }

    char* result = (char*) malloc(subStrLen + 1);

    for (i = 0; i < subStrLen; i++)
    {
        result[i] = path[i];
    }
    result[subStrLen] = '\0';

    return result;
}

// Open a file and checks if the file was opened successfully
FILE* openFile(char* fileName,const char* mode)
{
    FILE* fp = fopen(fileName,mode);

    if (fp == NULL)
    {
        char* currentPath = getCurrentPath();
        char* aditionalPath = (char*) calloc(strlen(fileName),sizeof(char));
        aditionalPath[0] = '\0';
        int i = 0;
        int j = 0;
        int aditionalPathLength = 0;
        int slashStart = 0;
        int slashEnd = 0;
        for (i = 0; i < strlen(fileName); i++)
        {
            if (fileName[i] == '\\' || fileName[i] == '/')
            {
                slashEnd = i;
                for(j = slashStart; j <= slashEnd; j++)
                {
                    aditionalPath[aditionalPathLength] = fileName[j];
                    aditionalPathLength++;
                }
                slashStart = i + 1;
            }
        }
        aditionalPath[aditionalPathLength] = '\0';
        if (strlen(aditionalPath) > 0)
        {
            charReplace(aditionalPath,'/','\\');
        }

        char* fullPath = concatenate(currentPath,aditionalPath);

        printf("\n\nFailed to open file \"%s\"", fileName);
        printf("\n\nThis can be caused by the following reasons:");
        printf("  \n- The file is not located under:\n    %s", fullPath);
        printf("  \n- The file is opened in another program");
        printf("  \n- The file is set to read only (Right-click the file, go to \"Properties\" and uncheck the box \"Read Only\")");
        printf("  \n- The file was accessed by this program in another menu and it wasn't closed\n   (if that's the case, restarting the program can solve this issue)\n");
        printf("\n     Suggestions: - Try running this program as Administrator and try again.");
        printf("\n                  - If even that doesn't work, move your entire game folder to your desktop, and try again.");
        printf("\n\nIf you proceed, the program will not work properly");
        free(currentPath);
        free(aditionalPath);
        free(fullPath);

        PAUSE();
        printf("\n");
    }
    return fp;
}

size_t readLine(char* line, FILE* fp)
{
    char c = '\0';
    int lineLen = 0;
    long filePos = ftell(fp);
    while (fread(&c,1,1,fp) && c != '\n')
    {
        printf("%c", c);
        lineLen++;
    }
    printf("\n");
    fseek(fp,filePos,SEEK_SET);
    line = (char*) malloc(lineLen + 1);
    int i = 0;
    for ( i = 0; i < lineLen; i++)
    {
        fread(&line[i],1,1,fp);
    }
    line[lineLen] = '\0';
    return fread(&c,1,1,fp);
}

// Copies an entire file, byte per byte, from a folder, to another
/*void systemCopyAll(char* srcFileName, char* src_folder,char* dest_folder)
{
    char* command = NULL;

    command = (char*) malloc(strlen("cd \"\"xcopy  /y \"\" \"\"") + strlen(src_folder) + strlen(srcFileName) + strlen(dest_folder) + 1);

    strcpy(command,"xcopy /y ");
    strcat(command," \"");
    strcat(command,src_folder);
    strcat(command,srcFileName);
    strcat(command,"\" \"");
    strcat(command,dest_folder);
    strcat(command,"\\\"");

    //printf("\n%s\n", command);

    system(command);

    free(command);
    printf("\n");
}*/

void systemCopyAndRename(char* srcFileName,char* destFileName, char* src_folder,char* dest_folder)
{
    printf("\nCopying: %s%s", src_folder,srcFileName);
    printf("\n     to: %s%s", dest_folder,destFileName);
    //printf("%s->%s ", srcFileName,destFileName);

    char* command = (char*) malloc(strlen("copy /y \"%s%s\" \"%s%s\\") + strlen(src_folder) + strlen(srcFileName) + strlen(dest_folder) + strlen(destFileName) + 1);

    sprintf(command,"copy /y \"%s%s\" \"%s%s\"",src_folder,srcFileName,dest_folder,destFileName);
    //printf("\ncommand: \'%s\'\n\n", command);
    system(command);
    free(command);

    char* srcCompletePath = concatenate(src_folder,srcFileName);
    char* destCompletePath = concatenate(dest_folder,destFileName);

    long srcFileSize = getFileSizeByName(srcCompletePath);
    long destFileSize = getFileSizeByName(destCompletePath);

    if (!fileExists(destCompletePath))
    {
        printf("Additional info: destination file doesn't exist");
        PAUSE();
    }
    else if (srcFileSize != destFileSize)
    {
        printf("Additional info: src and dest file size differs");
        PAUSE();
    }
    free(srcCompletePath);
    free(destCompletePath);
}

void systemCopy(char* srcFileName, char* src_folder,char* dest_folder)
{
    printf("\nCopying: %s%s", src_folder,srcFileName);
    printf("\n     to: %s%s", dest_folder,srcFileName);

    char* command = (char*) malloc(strlen("copy /y \"%s%s\" \"%s%s\\") + strlen(src_folder) + strlen(srcFileName) + strlen(dest_folder) + strlen(srcFileName) + 1);

    sprintf(command,"copy /y \"%s%s\" \"%s%s\"",src_folder,srcFileName,dest_folder,srcFileName);
    //printf("\ncommand: \'%s\'\n\n", command);
    system(command);
    free(command);

    char* srcCompletePath = concatenate(src_folder,srcFileName);
    char* destCompletePath = concatenate(dest_folder,srcFileName);

    long srcFileSize = getFileSizeByName(srcCompletePath);
    long destFileSize = getFileSizeByName(destCompletePath);

    if (!fileExists(destCompletePath))
    {
        printf("Additional info: destination file doesn't exist");
        PAUSE();
    }
    else if (srcFileSize != destFileSize)
    {
        printf("Additional info: src and dest file size differs");
        PAUSE();
    }
    free(srcCompletePath);
    free(destCompletePath);
}

// Copies an entire file, byte per byte, from a folder, to another
void systemMove(char* fileTypeFilter, char* src_folder,char* dest_folder)
{
    char* cmd1 = (char*) malloc(strlen("robocopy \"src_folder \" \"dest_folder \" *.* /E /XD \"dest_folder \" /move") + strlen(fileTypeFilter) + strlen(src_folder) + strlen(dest_folder) + 1);
    strcpy(cmd1,"robocopy \"");
    strcat(cmd1,src_folder);
    strcat(cmd1," \" \"");
    strcat(cmd1,dest_folder);
    strcat(cmd1," \" ");
    strcat(cmd1,fileTypeFilter);
    strcat(cmd1," /E /XD \"");
    strcat(cmd1,src_folder);
    strcat(cmd1," \" /move");

    system(cmd1);


    char* cmd2 = (char*)malloc(strlen("mkdir ") + strlen(src_folder));
    strcpy(cmd2,"mkdir ");
    strcat(cmd2,src_folder);

    system(cmd2);

    free(cmd1);
    free(cmd2);
    printf("\n");
}

void systemMkDir(char* folderPath)
{
    char* cmd = (char*)malloc(strlen("mkdir \"\" 2>NULL") + strlen(folderPath) + 1);
    strcpy(cmd,"mkdir \"");
    strcat(cmd,folderPath);
    strcat(cmd,"\" 2>NULL");
    system(cmd);
    remove(NULL);
    free(cmd);
}
