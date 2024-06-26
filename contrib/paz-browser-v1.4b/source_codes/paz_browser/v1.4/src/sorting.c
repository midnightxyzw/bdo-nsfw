#include "../include/sorting.h"

int sort_by_file_name_asc(void const *a, void const *b)
{
    FileBlock **aa = (FileBlock **)a;
    FileBlock **bb = (FileBlock **)b;

    char* fileNameA = (char*)(*aa)->fileName;
    char* fileNameB = (char*)(*bb)->fileName;

    return strcmp(fileNameA,fileNameB);
}

int sort_by_real_name_asc(void const *a, void const *b)
{
    FileBlock **aa = (FileBlock **)a;
    FileBlock **bb = (FileBlock **)b;

    char* fileNameA = (char*)(*aa)->fileName;
    char* fileNameB = (char*)(*bb)->fileName;

    if (!(*aa)->hasRealName)
    {
        fileNameA = concatenate("z",(char*)(*aa)->fileName);
    }
    else
    {
        fileNameA = (char*)(*aa)->RealName;
    }
    if (!(*bb)->hasRealName)
    {
        fileNameB = concatenate("z",(char*)(*bb)->fileName);
    }
    else
    {
        fileNameB = (char*)(*bb)->RealName;
    }

    return strcmp(fileNameA,fileNameB);
}

int sort_by_real_name_desc(void const *a, void const *b)
{
    FileBlock **aa = (FileBlock **)a;
    FileBlock **bb = (FileBlock **)b;

    char* fileNameA = (char*)(*aa)->fileName;
    char* fileNameB = (char*)(*bb)->fileName;

    if (!(*aa)->hasRealName)
    {
        fileNameA = concatenate("z",(char*)(*aa)->fileName);
    }
    else
    {
        fileNameA = (char*)(*aa)->RealName;
    }
    if (!(*bb)->hasRealName)
    {
        fileNameB = concatenate("z",(char*)(*bb)->fileName);
    }
    else
    {
        fileNameB = (char*)(*bb)->RealName;
    }

    return strcmp(fileNameB,fileNameA);
}


int sort_by_file_name_simple_pointer_asc(void const *a, void const *b)
{
    FileBlock *aa = (FileBlock *)a;
    FileBlock *bb = (FileBlock *)b;

    char* fileNameA = (char*)(aa)->fileName;
    char* fileNameB = (char*)(bb)->fileName;

    return strcmp(fileNameA,fileNameB);
}

int sort_by_file_name_desc(void const *a, void const *b)
{
    FileBlock **aa = (FileBlock **)a;
    FileBlock **bb = (FileBlock **)b;

    char* fileNameA = (char*)(*aa)->fileName;
    char* fileNameB = (char*)(*bb)->fileName;

    return strcmp(fileNameB,fileNameA);
}

int sort_by_hash_asc(void const *a, void const *b)
{
    FileBlock *aa = (FileBlock *)a;
    FileBlock *bb = (FileBlock *)b;

    long hashA = (long)(aa)->hash;
    long hashB = (long)(bb)->hash;

    return (hashA - hashB);
}

int sort_by_folder_name_asc(void const *a, void const *b)
{
    FileBlock **aa = (FileBlock **)a;
    FileBlock **bb = (FileBlock **)b;

    char* folderNameA = (char*)(*aa)->folderName;
    char* folderNameB = (char*)(*bb)->folderName;

    if (isANumber(folderNameA[0]) && folderNameA[1] == '_')
    {
        folderNameA = concatenate("0",folderNameA);
    }
    if (isANumber(folderNameB[0]) && folderNameB[1] == '_')
    {
        folderNameB = concatenate("0",folderNameB);
    }

    return strcmp(folderNameA,folderNameB);
}

int sort_by_folder_name_desc(void const *a, void const *b)
{
    FileBlock **aa = (FileBlock **)a;
    FileBlock **bb = (FileBlock **)b;

    char* folderNameA = (char*)(*aa)->folderName;
    char* folderNameB = (char*)(*bb)->folderName;

    if (isANumber(folderNameA[0]) && folderNameA[1] == '_')
    {
        folderNameA = concatenate("0",folderNameA);
    }
    if (isANumber(folderNameB[0]) && folderNameB[1] == '_')
    {
        folderNameB = concatenate("0",folderNameB);
    }

    return strcmp(folderNameA,folderNameB);
}


int sort_by_extension_asc(void const *a, void const *b)
{
    FileBlock **aa = (FileBlock **)a;
    FileBlock **bb = (FileBlock **)b;

    char* fileNameA = (char*)(*aa)->fileName;
    char* fileNameB = (char*)(*bb)->fileName;

    char* extA = getFileExt(fileNameA);
    char* extB = getFileExt(fileNameB);

    char* extAndNameA = concatenate(extA,fileNameA);
    char* extAndNameB = concatenate(extB,fileNameB);


    int result = strcmp(extAndNameA,extAndNameB);

    free(extAndNameA);
    free(extAndNameB);

    return result;
}

int sort_by_extension_pac_dds_priority_simple_pointer_desc(void const *a, void const *b)
{
    FileBlock *aa = (FileBlock *)a;
    FileBlock *bb = (FileBlock *)b;

    char* fileNameA = (char*)(aa)->fileName;
    char* fileNameB = (char*)(bb)->fileName;

    char fileExtA[4];
    char fileExtB[4];

    int lenA = strlen(fileNameA);
    int lenB = strlen(fileNameB);

    fileExtA[0] = fileNameA[lenA - 3];
    fileExtA[1] = fileNameA[lenA - 2];
    fileExtA[2] = fileNameA[lenA - 1];
    fileExtA[3] = '\0';

    fileExtB[0] = fileNameB[lenB - 3];
    fileExtB[1] = fileNameB[lenB - 2];
    fileExtB[2] = fileNameB[lenB - 1];
    fileExtB[3] = '\0';

    if (fileExtA[0] == '.')
    {
        fileExtA[0] = fileExtA[1];
    }
    if (fileExtB[0] == '.')
    {
        fileExtB[0] = fileExtB[1];
    }

    if (fileExtA[0] == 'p' && fileExtA[1] == 'a' && fileExtA[2] == 'c')
    {
        fileExtA[0] = '0';
    }
    if (fileExtA[0] == 'd' && fileExtA[1] == 'd' && fileExtA[2] == 's')
    {
        fileExtA[0] = '1';
    }

    if (fileExtB[0] == 'p' && fileExtB[1] == 'a' && fileExtB[2] == 'c')
    {
        fileExtB[0] = '0';
    }
    if (fileExtB[0] == 'd' && fileExtB[1] == 'd' && fileExtB[2] == 's')
    {
        fileExtB[0] = '1';
    }

    return strcmp(fileExtA,fileExtB);
}

int sort_by_extension_desc(void const *a, void const *b)
{
    FileBlock **aa = (FileBlock **)a;
    FileBlock **bb = (FileBlock **)b;

    char* fileNameA = (char*)(*aa)->fileName;
    char* fileNameB = (char*)(*bb)->fileName;

    char* extA = getFileExt(fileNameA);
    char* extB = getFileExt(fileNameB);

    char* extAndNameA = concatenate(extB,fileNameA);
    char* extAndNameB = concatenate(extA,fileNameB);


    int result = strcmp(extAndNameA,extAndNameB);

    free(extAndNameA);
    free(extAndNameB);

    return result;
}

int sort_by_file_num_asc(void const *a, void const *b)
{
    FileBlock **aa = (FileBlock **)a;
    FileBlock **bb = (FileBlock **)b;

    long fileNumA = (long)(*aa)->fileNum;
    long fileNumB = (long)(*bb)->fileNum;

    return (fileNumA-fileNumB);
}

int sort_by_file_num_desc(void const *a, void const *b)
{
    FileBlock **aa = (FileBlock **)a;
    FileBlock **bb = (FileBlock **)b;

    long fileNumA = (long)(*aa)->fileNum;
    long fileNumB = (long)(*bb)->fileNum;

    return (fileNumB-fileNumA);
}

int sort_by_name_asc(void const *a, void const *b)
{
    char* aa = *((char**) a);
    char* bb = *((char**) b);

    if (isANumber(aa[0]) && aa[1] == '_')
    {
        aa = concatenate("0",aa);
    }
    if (isANumber(bb[0]) && bb[1] == '_')
    {
        bb = concatenate("0",bb);
    }
    return strcmp(aa, bb);
}

int sort_by_name_desc(void const *a, void const *b)
{
    char* aa = *((char**) a);
    char* bb = *((char**) b);

    if (isANumber(aa[0]) && aa[1] == '_')
    {
        aa = concatenate("0",aa);
    }
    if (isANumber(bb[0]) && bb[1] == '_')
    {
        bb = concatenate("0",bb);
    }
    return strcmp(bb, aa);
}

int sort_by_folder_name_and_file_name_asc(void const *a, void const *b) {
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

int long_sort(void const *a, void const *b)
{
    return ( *(long*)a - *(long*)b );
}



