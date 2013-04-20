#include <stdio.h>

#define LEN 20
#define SWAP(a, b)      \
        (a) = (a) ^ (b);\
        (b) = (a) ^ (b);\
        (a) = (a) ^ (b);

void quick(int array[], int len)
{
    int f, l, flag;

    if (len <= 1)
        return;

    f = 0;
    l = len - 1;
    flag = 0;
    while (f < l) {
        if (array[f] > array[l]) {
            SWAP(array[f], array[l]);
            flag = 1 - flag;
        }

        if (flag == 1)
            f++;
        else
            l--;
    }

    quick(array, f);
    quick(array + f + 1, len - f - 1);
}

int main()
{
    int i;
    int array[LEN];

    srandom(time(NULL));
    for (i = 0; i < LEN; i++)
        array[i] = random() % 100;

    quick(array, LEN);
    for (i = 0; i < LEN; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}
