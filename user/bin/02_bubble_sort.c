#include "console.h"
#include "lib.h"

#define SIZE 10000

int data[SIZE];

void bubble_sort() {
    unsigned long start = user_get_time();
    user_printf("App 02: Bubble sort starts at %lu ms.\n", start);
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = i; j < SIZE - 1; j++) {
            if (data[j] > data[j + 1]) {
                int temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
        if ((i + 1) % 1000 == 0) {
            user_printf(
                "App 02: Ran %d times, used %lu ms.\n", i + 1, (unsigned long)(user_get_time() - start)
            );
        }
    }
}

int main() {
    for (int i = 0; i < SIZE; i++) {
        data[i] = SIZE - i;
    }

    bubble_sort();

    user_printf("App 02 Done!\n");
    return 0;
}
