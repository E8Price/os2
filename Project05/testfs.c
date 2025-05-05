#include <stdio.h>
#include "image.h"
#include "ctest.h"

#ifdef CTEST_ENABLE

void test_image_open_close(void)
{
    int fd = image_open("test.img", 1);
    CTEST_ASSERT(fd >= 0, "image_open should return valid file descriptor");

    int result = image_close();
    CTEST_ASSERT(result == 0, "image_close should return 0 on success");
}

int main(void)
{
    CTEST_VERBOSE(1);

    test_image_open_close();

    CTEST_RESULTS();
    CTEST_EXIT();
}

#else

int main(void)
{
    printf("Running normally\n");

    image_open("test.img", 1);
    printf("Opened image.\n");
    image_close();
    printf("Closed image.\n");
}

#endif
