#include "console.h"

#define MAX_EXP 20000

int data[MAX_EXP];

int main() {
    int mod = 123127;
    data[0] = 1;
    user_printf("13^0 = %d\n", data[0]);
    for (int i = 1; i < MAX_EXP; i++) {
        data[i] = data[i - 1] * 13 % mod;
        if (i % 1000 == 0) {
            user_printf("13^%d = %d\n", i, data[i]);
        }
    }

    user_printf("App 03 Done!\n");
    return 0;
}
