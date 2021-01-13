
#define ALLOC_SIZE 100

typedef char *table_str;

table_str table_init(size_t len);
int table_add(table_str t, const char *text, size_t len);
size_t table_length(table_str t);
void table_print(table_str t);


typedef struct Header {
    // how much it can hold
    size_t capacity;
    // how many it has
    size_t size;
} Header;

