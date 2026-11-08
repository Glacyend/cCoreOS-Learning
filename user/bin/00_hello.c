#include "console.h"
#include "lib.h"

int main() {
    for (int i = 0; i < 5; i++) {
        printf("Hello from app 00, iteration %d\n", i);
        yield();
    }
    printf("App 00 done!\n");
    return 0;
}
