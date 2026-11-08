#include "console.h"
#include "lib.h"

#define MAX_EXP 12000

int power[MAX_EXP];

int main() {
    int mod = 123127;
    power[0] = 1;
    printf("5^0 = %d\n", power[0]);
    for (int i = 1; i < MAX_EXP; i++) {
        power[i] = power[i - 1] * 5 % mod;
        if (i % 1000 == 0) {
            printf("5^%d = %d\n", i, power[i]);
            yield();
        }
    }

    printf("App 02 done!\n");
    return 0;
}
