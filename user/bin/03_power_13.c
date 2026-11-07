#include "console.h"

#define MAX_EXP 20000

int power[MAX_EXP];

int main() {
    int mod = 123127;
    power[0] = 1;
    printf("13^0 = %d\n", power[0]);
    for (int i = 1; i < MAX_EXP; i++) {
        power[i] = power[i - 1] * 13 % mod;
        if (i % 1000 == 0) {
            printf("13^%d = %d\n", i, power[i]);
        }
    }

    return 0;
}
