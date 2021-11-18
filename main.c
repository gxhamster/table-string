#define TABLE_STR_IMPL
#include "table_str.h"


int main() {
    table_str table = table_init(10);

    char *test_str = "hello";

    /* Do the work. */
    table_cat(table, test_str , strlen(test_str));

    
    // split test
    table_split(table, " ");

    table_free(table);
}






