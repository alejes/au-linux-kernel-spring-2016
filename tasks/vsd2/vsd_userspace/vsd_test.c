#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "vsd_device.h"

#define TEST(assertion) \
    if (!(assertion)) { \
        fprintf(stderr, "%s %d\n", __FILE__, __LINE__); \
        perror("Test failed"); \
        abort(); \
    }

static void run_one_test(off_t vsd_offset, size_t vsd_size) {
    unsigned char *vsd_rw_buf = malloc(vsd_size);
    size_t i = 0;
    for (; i < vsd_size; ++i) {
        vsd_rw_buf[i] = i % 255;
    }
    printf("[1]\n");
    TEST(vsd_write(vsd_rw_buf, vsd_offset, vsd_size) >= 0);
    printf("[2]\n");
    char* vsd_mem = vsd_mmap(vsd_offset);
    TEST(vsd_mem);
    printf("[3]\n");
    // Check that written data is visible by mmap
    TEST(!memcmp(vsd_rw_buf, vsd_mem, vsd_size));
    printf("[4]\n");
    // Check in opposite direction
    vsd_mem[10] = ~vsd_mem[10];
    TEST(vsd_read(vsd_rw_buf, vsd_offset, vsd_size) >= 0);
    printf("[5]\n");
    TEST(!memcmp(vsd_rw_buf, vsd_mem, vsd_size));
    printf("[6]\n");
    // While we are mapped vsd, its size can't be changed
    TEST(vsd_set_size(vsd_size / 2));
    printf("[7]\n");
    TEST(!vsd_munmap(vsd_mem, vsd_offset));
    printf("[8]\n");
    // Now ok
    TEST(!vsd_set_size(vsd_size / 2));
    printf("[9]\n");
    TEST(!vsd_set_size(vsd_size));
    printf("[23]\n");

    free(vsd_rw_buf);
}

int main()
{
    TEST(!vsd_init());

    const size_t PAGE_SIZE = (size_t)getpagesize();
    size_t vsd_size = 0;
    TEST(!vsd_get_size(&vsd_size));

    run_one_test(0, vsd_size);
    run_one_test(PAGE_SIZE, vsd_size - PAGE_SIZE);

    TEST(!vsd_deinit());
    printf("Ok\n");
    return 0;
}
