#ifndef SORTING_H
#define SORTING_H

#include <string.h>
#include "../include/data_types.h"
#include "../include/utilities.h"

#define ASCENDANT 0
#define DESCENDANT 1

typedef enum {
    SORT_BY_FILE_NAME,
    SORT_BY_REAL_NAME,
    SORT_BY_FILE_NUM,
    SORT_BY_EXTENSION,
    SORT_BY_NAME,
    SORT_BY_FOLDER_NAME_AND_FILE_NAME,
} SortingType;


int sort_by_file_name_asc(void const *a, void const *b);
int sort_by_file_name_desc(void const *a, void const *b);

int sort_by_real_name_desc(void const *a, void const *b);
int sort_by_real_name_asc(void const *a, void const *b);

int sort_by_file_name_simple_pointer_asc(void const *a, void const *b);

int sort_by_hash_asc(void const *a, void const *b);

int sort_by_folder_name_asc(void const *a, void const *b);
int sort_by_folder_name_desc(void const *a, void const *b);

int sort_by_folder_name_asc(void const *a, void const *b);
int sort_by_folder_name_dsc(void const *a, void const *b);

int sort_by_file_num_asc(void const *a, void const *b);
int sort_by_file_num_desc(void const *a, void const *b);

int sort_by_extension_asc(void const *a, void const *b);
int sort_by_extension_desc(void const *a, void const *b);

int sort_by_name_asc(void const *a, void const *b);
int sort_by_name_desc(void const *a, void const *b);

int sort_by_folder_name_and_file_name_asc(void const *a, void const *b);

int long_sort(void const *a, void const *b);

int sort_by_extension_pac_dds_priority_simple_pointer_desc(void const *a, void const *b);
#endif // SORTING_H

