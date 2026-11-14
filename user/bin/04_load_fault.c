#include "console.h"

#include <stddef.h>

int main() {
    user_printf("load_fault app running...\n");
    user_printf("Into test load_fault, we will insert an invalid load operation...\n");
    user_printf("Kernel should kill this application!\n");
    volatile char i = *(volatile char*)NULL;
    (void)i;
    return 0;
}
