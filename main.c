#include <stdio.h>
#include <stdlib.h>
#include "table_str.h"

Header *get_header(table_str t) {
    size_t header_size = sizeof(Header);
    Header *h = (Header *)(t - header_size);

    return h;
}

/*
    Add elements to string return len if success
    returns -1 on failure
*/
int table_add(table_str t, const char *str, size_t size) {
    Header *header = get_header(t);
    if (header->capacity < header->size + size) {
        // Realloc the str
        header->capacity += size;
        t = (table_str)realloc(t, (header->capacity) * sizeof(char));
        if (t == NULL) return -1;
    }

    // Have enough space
    size_t idx1;
    size_t idx2;
    idx1 = (header->size);
    idx2 = idx1 + size;
    
    // Copy the characters
    int i;
    int j;
    for (i = idx1, j = 0; i < idx2; i++) {
        t[i] = str[j];
        j++;
    }
    t[++i] = '\0';
    header->size += size;

    return header->size;
}

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

void table_free(table_str t) {
    Header *h = get_header(t);
    free(h);
}

int main() {
    table_str table;
    /* hello bob */
    table = table_init(100);
    Header *h = get_header(table);
    
    printf("Capcacity = %ld\n", h->capacity);
    const char test_str[] = "Hello jhon its been a while";
    table_add(table, test_str , sizeof(test_str));
    printf("string = %s\n", table);
    table_print(table);
    table_free(table);
}
