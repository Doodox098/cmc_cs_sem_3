//  8-seg memory

#include <stdlib.h>
#include <limits.h>

typedef unsigned int addr_t;
enum { SEG_BITS = 3 };
enum { OFFSET_BITS = sizeof(addr_t) * CHAR_BIT - SEG_BITS };

typedef struct segment {
    addr_t base;
    addr_t size;
} segment;

unsigned int VirtIntoPhys(segment *SegTable, addr_t VirtAddr)
{
    addr_t seg_n = VirtAddr >> OFFSET_BITS;
    addr_t seg_off = VirtAddr & ((1u << OFFSET_BITS) - 1);
    segment *seg = &SegTable[seg_n];
    if (seg->size >= seg_off) {
        exit(25);
    }
    return seg->base + seg_off;
}
