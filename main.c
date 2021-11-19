#define BUFFER_IMPL
#include "table_str.h"


int main() {
    BufferRegion *r = buffer_region_create();
    Buffer b1 = buffer_create(r, "hello there", 11);
    buffer_create(r, "again", 5);
    Buffer b = buffer_region_get(r, 0);

    buffer_region_dump(r, stdout);
    buffer_region_free(r);
}
