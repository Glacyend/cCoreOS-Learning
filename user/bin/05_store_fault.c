#include "console.h"

#include <stddef.h>

int main() {
    user_printf("store_fault app running...\n");
    user_printf("Into test store_fault, we will insert an invalid store opreation...\n");
    user_printf("Kernel should kill this application!\n");
    volatile char* p = (volatile char*)NULL;
    *p = 1;
    return 0;
}
