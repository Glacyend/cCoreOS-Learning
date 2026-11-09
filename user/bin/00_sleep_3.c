#include "console.h"
#include "lib.h"

int main() {
    isize start = user_get_time();
    isize target = start + 3000;

    user_printf("App 00: start = %zu ms, target = %zu ms\n", start, target);

    while (user_get_time() < target) {
        user_yield();
    }

    user_printf("App 00: Woke up at %lu ms!\n", (unsigned long)user_get_time());
    user_printf("App 00: Done!\n");
    return 0;
}
