#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include "table_str.h"

/* Returns the header struct for the given string */
Header *get_header(table_str t) {
    size_t header_size = sizeof(Header);
    Header *h = (Header *)(t - header_size);

    return h;
}

void make_room_table(table_str t, size_t new_len) {
    Header *header = get_header(t); 
    void *sh;
    void *new_sh;

    sh = (void *)header;

    // Realloc the string
    header->capacity += new_len;
    new_sh = realloc(sh, sizeof(Header) + (header->capacity));
    if (new_sh == NULL) exit(-1);
    t = new_sh + sizeof(Header);
    header = (Header *) new_sh;
}

/*
    Add elements to string return len if success
    returns -1 on failure
*/
int table_add(table_str t, const char *str, size_t size) {
    void *sh;
    Header *header = get_header(t);
    /* 
        Get the starting point of the string
        If you try to use realloc direclty on t it fails
    */
    sh = (void *)header;
    void *new_sh;

    if (header->capacity < header->size + size) {
        // +1 for the null character
        make_room_table(t, size + 1);
        if (t == NULL) return -1;
    }

    // Have enough space

//      size_t idx1;
//      size_t idx2;
//      idx1 = (header->size);
//      idx2 = idx1 + size;
// 
//      // Copy the characters
//      int i;
//      int j;
//      for (i = idx1, j = 0; i < idx2; i++, j++)
//          t[i] = str[j];
    //  t[++i] = '\0';

    // Try to get this to work (Finally)
    // Copy the characters
    char *idx1;
    idx1 = &t[(header->size)];
    memcpy(idx1, str, size);
    t[header->size + size] = '\0';

    // Change the size of string to new size
    header->size += size;

    return header->size;
}


/* 
   Advised to use this function over printf because printf works based on \0 chars
   This function uses the length of the array from the header
 */
void table_print(table_str t) {
    int i;
    size_t size = get_header(t)->size;
    for (i = 0; i < size; i++) {
        printf("%c", t[i]);
    }
    printf("\n");
}

table_str table_init(size_t len) {
    size_t header_size = sizeof(Header);
    void *sh = (void *)malloc(header_size + len); 

    Header *h;    
    h  = (Header *) sh;
    h->size = 0;
    h->capacity = len;

    table_str s;
    s = (table_str )(sh + header_size);

    return s;
}

/* Free all the memory asscociated with the string including the header */
void table_free(table_str t) {
    Header *h = get_header(t);
    free(h);
}

int main() {
    table_str table;
    table = table_init(1);

    const char test_str[] = "Hello jhon its been a while";
    const char test_str1[] = "The BASIC implementation uses a preprocessor that can generate BASIC code that is compatible with both C64 BASIC (CBM v2) and QBasic. The C64 mode has been tested with cbmbasic (the patched version is currently required to fix issues with line input) and the QBasic mode has been tested with qb64";

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    /* Do the work. */
    table_add(table, test_str , strlen(test_str));
    table_add(table, test_str1, strlen(test_str1));

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken = %lf\n", cpu_time_used); 

    printf("String = %s\n", table);
    printf("Capacity = %lu\n", get_header(table)->capacity);
    table_print(table);

    table_free(table);
}
