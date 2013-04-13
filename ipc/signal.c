#include <signal.h>
#include <stdio.h>

void handdle(int n)
{
    printf("func = %s line = %d\n", __func__, __LINE__);
}

int main()
{
    int sign;

    signal(2, handdle);
    sleep(10);
}
