#include <stdio.h>

#define LEN 10
#define SWAP(a, b)  \
    (a) = (a) - (b);\
    (b) = (a) + (b);\
    (a) = (b) - (a)

int bubble(int array[], int len)
{
    int i, j;

    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - i - 1; j++) {
            if (array[j] > array[j + 1]) {
               SWAP(array[j], array[j + 1]);
            }
        }
    }

    return 0;
}

int main()
{
    int array[LEN];
    int i;

    srandom(time(NULL));
    for (i = 0; i < LEN; i++)
        array[i] = random() % 100;

    bubble(array, LEN);

    for (i = 0; i < LEN; i++)
        printf("%d ", array[i]);
    printf("\n");

    return 0;
}
