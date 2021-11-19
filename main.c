#define BUFFER_IMPL
#define MAX_BUFFER_COUNT 300000
#define MAX_BUFFER_CAP 200

#include "table_str.h"


int main() {
    //BufferRegion *r = buffer_region_create();
    //Buffer b1 = buffer_create(r, "hello there", 11);
    //buffer_create(r, "again", 5);
    //Buffer b = buffer_region_get(r, 0);

    BufferRegion *r = buffer_region_create();
    char s[200];
    FILE* f = fopen("out.txt", "r");
    while (fgets(s, 100, f) != NULL) {
        buffer_create(r, s, strlen(s));
    }
    buffer_region_dump(r, stdout);
    buffer_region_free(r);
}
