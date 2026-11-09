#include "console.h"
#include "lib.h"

int main() {
    isize start = user_get_time();
    isize target = start + 5000;

    user_printf("App 01: start = %zu ms, target = %zu ms\n", start, target);

    while (user_get_time() < target) {
        user_yield();
    }

    user_printf("App 01: woke up at %lu ms!\n", (unsigned long)user_get_time());
    user_printf("App 01: done!\n");
    return 0;
}
