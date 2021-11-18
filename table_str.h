#ifndef TABLE_STR_H
#define TABLE_STR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ALLOC_SIZE 100
#define TABLE_STR_OFFSET(t) t + sizeof(Header)
#define NULL_CHECK(t) if (t == NULL) exit(-1)

typedef char *table_str;

// Structs
typedef struct Header {
    size_t capacity;
    size_t size;
} Header;

// Function declarations
table_str table_init(size_t len);
int table_cat(table_str t, const char *text, size_t len);
size_t table_length(table_str t);
void table_print(table_str t);
table_str *table_split(table_str t, const char *delim);

// Private functions
Header *get_header(table_str t);
void make_room_table(table_str t, size_t new_len);

// Implementation
#ifdef TABLE_STR_IMPL

Header *get_header(table_str t) {
    Header *h = (Header *)(t - sizeof(Header));

    return h;
}

/* Free all the memory asscociated with the string including the header */
void table_free(table_str t) {
    NULL_CHECK(t);
    Header *h = get_header(t);
    memset(t, 0, h->size);
    free(h);
}

void make_room_table(table_str t, size_t new_len) {
    NULL_CHECK(t);

    Header *header = get_header(t);
    void *new_sh;


    if (new_len < header->size || new_len < header->capacity) {
        fprintf(stderr, "ERROR: New length is smaller than current size and capacity");
        exit(-1);
    }

    header->capacity += new_len;
    new_sh = realloc(header, sizeof(Header) + (header->capacity));
    NULL_CHECK(new_sh);

    Header *new_header = (Header *)new_sh;
    new_header->size = header->size;
    new_header->capacity = header->capacity;

    strcpy(TABLE_STR_OFFSET(new_sh), t);
    table_free(t);
    t = new_sh + sizeof(Header);
}


int table_cat(table_str t, const char *str, size_t size) {
    NULL_CHECK(t);
    void *sh;
    Header *header = get_header(t);

    sh = (void *)header;
    void *new_sh;

    if (header->capacity < header->size + size) {
        // +1 for the null character
        make_room_table(t, size + 1);
    }


    // Copy the characters
    char *idx1;
    idx1 = &t[(header->size)];
    memcpy(idx1, str, size);
    t[header->size + size] = '\0';

    header->size += size;

    return header->size;
}


/*
   Advised to use this function over printf because printf works based on \0 chars
   This function uses the length of the array from the header
 */
void table_print(table_str t) {
    NULL_CHECK(t);
    int i;
    size_t size = get_header(t)->size;
    for (i = 0; i < size; i++) {
        printf("%c", t[i]);
    }
    printf("\n");
}

table_str table_init(size_t len) {
    void *sh = (void *)malloc(sizeof(Header)+ len);
    NULL_CHECK(sh);

    Header *h  = (Header *) sh;
    h->size = 0;
    h->capacity = len;

    table_str s = (table_str)(TABLE_STR_OFFSET(sh));
    memset(s, 0, h->size);

    return s;
}



/* Returns the length of the str */
size_t table_length(table_str t) {
    NULL_CHECK(t);
    return get_header(t)->size;
}

/* Split the string by a delim and returns and array */
table_str *table_split(table_str t, const char *delim) {
    NULL_CHECK(t);

    int max_size = 100;
    char **arr = (char **)malloc(sizeof(table_str) * max_size);
    int count = 0;
    char *token = strtok(t, delim);
    size_t size_of_token;
    table_str t1;

    while ((token = strtok(NULL, delim)) != NULL) {
        size_of_token = strlen(token);
        t1 = table_init(size_of_token);

        // +1 for the null char
        table_cat(t1, token, size_of_token + 1);

        if (count > max_size) {
            arr = (char **)realloc(arr, sizeof(table_str) * count + 1);
            if (arr == NULL) exit(-1);
        }
        arr[count++] = t1;

    }

    return arr;
}

#endif

#endif
