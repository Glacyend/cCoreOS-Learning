#include "console.h"

#include <stddef.h>

static const size_t LEN = 100;

int main() {
    unsigned long p = 3;
    unsigned long m = 998244353;
    size_t iter = 300000;
    unsigned long s[LEN];
    size_t cur = 0;
    s[cur] = 1;
    for (size_t i = 1; i <= iter; i++) {
        size_t next = (cur + 1 == LEN) ? 0 : cur + 1;
        s[next] = s[cur] * p % m;
        cur = next;
        if (i % 10000 == 0) {
            user_printf("power_3 [%zu/%zu]\n", i, iter);
        }
    }
    user_printf("%lu^%lu = %lu (mod %lu)\n", p, iter, s[cur], m);
    user_printf("Test power_3 OK!\n");
    return 0;
}
