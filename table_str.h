#ifndef TABLE_STR_H
#define BUFFER_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


typedef char *Buffer;
#ifndef MAX_BUFFER_COUNT
#define MAX_BUFFER_COUNT 1000
#endif


#define NULL_CHECK(B) if (B == NULL) exit(-1);
#define MAX_BUFFER_CAP 5

// Structs
typedef struct {
    char *start_ptr;
    size_t len;
    size_t cap;
} BufferHeader;

typedef struct {
    Buffer cur_buf;
    int reached_end;
    size_t idx;
} BufferRegionIter;

typedef struct {
    size_t buffer_count;
    size_t cur_idx;
    char *next_buffer_pos;
    BufferHeader headers[MAX_BUFFER_COUNT];
    char buffer_locations[MAX_BUFFER_COUNT];
} BufferRegion;

// Function declarations
BufferRegion *buffer_region_create();
void buffer_region_free(BufferRegion *r);
Buffer buffer_create(BufferRegion *r, const char *str, size_t len);
Buffer buffer_region_get(BufferRegion *r, size_t idx);
void buffer_region_dump(BufferRegion *r, FILE *stream);

// Private functions

// Implementation
#ifdef BUFFER_IMPL
BufferRegion *buffer_region_create()
{
    BufferRegion *region = (BufferRegion *)malloc(sizeof(BufferRegion));
    region->buffer_count = 0;
    region->cur_idx = 0;
    region->next_buffer_pos = region->buffer_locations; return region;
}

void buffer_region_free(BufferRegion *r)
{
    NULL_CHECK(r);
    r->buffer_count = 0;
    r->cur_idx = 0;
    r->next_buffer_pos = NULL;
    memset(r->headers, 0, MAX_BUFFER_COUNT);
    memset(r->buffer_locations, 0, MAX_BUFFER_COUNT);

    free(r);
}

Buffer buffer_region_get(BufferRegion *r, size_t idx)
{
    if (idx > MAX_BUFFER_COUNT || idx > r->buffer_count-1 || idx < 0)
        return NULL;

    BufferHeader h = r->headers[idx];
    return h.start_ptr;
}

void buffer_region_dump(BufferRegion *r, FILE *stream)
{
    NULL_CHECK(r);
    int i;
    for (i = 0; i < r->buffer_count; i++) {
        fprintf(stream, "%s\n", r->headers[i].start_ptr);
    }
}

Buffer buffer_create(BufferRegion *r, const char *str, size_t len)
{
    if (len > MAX_BUFFER_CAP) {
        fprintf(stderr, "ERROR: string length is greater than max capacity\n");
        exit(-1);
    }
    r->headers[r->cur_idx].len = len;
    r->headers[r->cur_idx].cap = MAX_BUFFER_CAP;
    r->headers[r->cur_idx].start_ptr = r->next_buffer_pos;
    memcpy(r->headers[r->cur_idx].start_ptr, str, len+1);
    r->next_buffer_pos = r->headers[r->cur_idx].start_ptr + r->headers[r->cur_idx].len + 1;
    r->buffer_count += 1;
    return r->headers[r->cur_idx++].start_ptr;
}

#endif

#endif
