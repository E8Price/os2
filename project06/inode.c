#include "image.h"
#include "block.h"
#include "free.h"
#include "inode.h"

int ialloc(void) {
    unsigned char map[BLOCK_SIZE];
    bread(INODE_MAP_BLOCK, map);
    int idx = find_free(map);
    if (idx < 0) return -1;
    set_free(map, idx, 1);
    bwrite(INODE_MAP_BLOCK, map);
    return idx;
}
