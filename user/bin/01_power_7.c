#include "console.h"
#include "lib.h"

#define MAX_EXP 10000

int power[MAX_EXP];

int main() {
    int mod = 123127;
    power[0] = 1;
    printf("7^0 = %d\n", power[0]);
    for (int i = 1; i < MAX_EXP; i++) {
        power[i] = power[i - 1] * 7 % mod;
        if (i % 1000 == 0) {
            printf("7^%d = %d\n", i, power[i]);
            yield();
        }
    }

    printf("App 01 done!\n");
    return 0;
}
