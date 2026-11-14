#include "console.h"
#include "lib.h"

int main() {
    long current_time = user_get_time();
    long wait_for = current_time + 3000;
    while (user_get_time() < wait_for) {
        user_yield();
    }
    user_printf("Test sleep OK!\n");
    return 0;
}
