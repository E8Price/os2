#include "ctest.h"

#include "image.h"
#include "block.h"
#include "free.h"
#include "inode.h"

CTEST(test_free, find_and_set) {
    unsigned char m[BLOCK_SIZE] = {0};
    CTEST_ASSERT(find_free(m) == 0, "first free is 0");
    set_free(m, 0, 1);
    CTEST_ASSERT(find_free(m) == 1, "next free is 1");
}

CTEST(test_inode, simple_ialloc) {
    unsigned char zero[BLOCK_SIZE] = {0};
    CTEST_ASSERT(image_open("img", 1) >= 0, "open image");
    bwrite(INODE_MAP_BLOCK, zero);
    int i1 = ialloc();
    CTEST_ASSERT(i1 == 0, "first inode is 0");
    int i2 = ialloc();
    CTEST_ASSERT(i2 == 1, "second inode is 1");
    CTEST_ASSERT(image_close() >= 0, "close image");
}

int main(void) {
    CTEST_VERBOSE(1);
    test_test_free_find_and_set();
    test_test_inode_simple_ialloc();
    CTEST_RESULTS();
    CTEST_EXIT();
}
