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


    // Copy the characters
    char *idx1;
    idx1 = &t[(header->size)];
    memcpy(idx1, str, size);
    // Add null character to end
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


/* Returns the length of the str */
size_t table_length(table_str t) {
    if (t == NULL) return -1;
    return get_header(t)->size;
}

/* Split the string by a delim and returns and array */
table_str *table_split(table_str t, const char *delim) {
    // The 1000 is just an arbitary size
    int max_size = 100;
    char **arr = (char **)malloc(sizeof(table_str) * max_size);
    int count = 0;
    char *token = strtok(t, delim);
    size_t size_of_token;
    table_str t1;

    while ((token = strtok(NULL, delim)) != NULL) {
        /* 
         * Create another string
         * (do check for null char) because strlen fails 
         */
        size_of_token = strlen(token);
        t1 = table_init(size_of_token);

        // +1 for the null char
        table_add(t1, token, size_of_token + 1);
         
        if (count > max_size) {
            // reallocate the entire array
            arr = (char **)realloc(arr, sizeof(table_str) * count + 1);
            if (arr == NULL) exit(-1);
        }
        arr[count++] = t1;
         
    }
        
    /*
        int i;
        for (i = 0; i < count; i++)
            printf("%d =  %s\n", i, arr[i]);
    */

    return arr;


}

int main() {
    table_str table;
    table = table_init(1);

    const char test_str[] = "Hello jhon its been a while";
    const char test_str1[] = "Scripting a game can be difficult when there are many states that need to handled, but only one script can be attached to a node at a time. Instead of creating a state machine within the player's control script, it would make development simpler if the states were separated out into different classes.There are many ways to implement a state machine with Godot, and some other methods are below:\ 
                            The player can have a child node for each state, which are called when utilized.\
                            Enums can be used in conjunction with a match statement.\
                            The state scripts themselves could be swapped out from a node dynamically at run-time.\
                            This tutorial will focus only on adding and removing nodes which have a state script attached. Each state script will be an implementation of a different state.";

    /* Do the work. */
    table_add(table, test_str , strlen(test_str));
    table_add(table, test_str1, strlen(test_str1));

    
    // split test
    table_split(table, " ");

    table_free(table);
}






