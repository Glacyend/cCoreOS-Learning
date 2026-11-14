#include "console.h"
#include "lib.h"

#include <stddef.h>

static const size_t PAGE_SIZE = 0x1000;

int main() {
    user_printf("Test sbrk start.\n");
    long origin_brk = user_sbrk(0);
    user_printf("Origin break point = %lX\n.", (unsigned long)origin_brk);
    long brk = user_sbrk((int)PAGE_SIZE);
    if (brk != origin_brk) {
        return -1;
    }
    brk = user_sbrk(0);
    user_printf("One page allocated, break point = %lX\n.", (unsigned long)brk);
    user_printf("Try writing to allocated page.\n");
    char* new_page = (char*)(size_t)origin_brk;
    for (size_t i = 0; i < PAGE_SIZE; i++) {
        new_page[i] = 1;
    }
    user_printf("Write OK!\n");
    user_sbrk((int)PAGE_SIZE * 10);
    brk = user_sbrk(0);
    user_printf("10 pages allocated, break point = %lX\n", (unsigned long)brk);
    user_sbrk((int)PAGE_SIZE * -11);
    brk = user_sbrk(0);
    user_printf("11 pages deallocated, break point = %lX\n", (unsigned long)brk);
    user_printf("Try deallocating one more page, should be failed.\n");
    long ret = user_sbrk(-(int)PAGE_SIZE);
    if (ret != -1) {
        user_printf("Test sbrk failed!\n");
        return -1;
    }
    user_printf("Test sbrk almost OK!\n");
    user_printf("Now write to deallocated page, should cause page fault.\n");
    for (size_t i = 0; i < PAGE_SIZE; i++) {
        new_page[i] = 2;
    }
    return 0;
}
