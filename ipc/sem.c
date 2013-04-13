#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

void *pthread1(void *arg)
{
    sem_t *sem;

    sem = (sem_t *)arg;
    while (1) {
        sem_wait(&sem[1]);
        printf("func = %s line = %d\n", __func__, __LINE__);
        sem_post(&sem[0]);
        sleep(1);
    }
}

void *pthread2(void *arg)
{
    sem_t *sem;

    sem = (sem_t *)arg;
    while (1) {
        sem_wait(&sem[0]);
        printf("func = %s line = %d\n", __func__, __LINE__);
        sem_post(&sem[1]);
        sleep(1);
    }
}

int main()
{
    pthread_t pid1, pid2;
    sem_t sem[2];
    int ret;

    sem_init(&sem[0], 0, 1);
    sem_init(&sem[1], 0, 0);
    ret = pthread_create(&pid1, NULL, pthread1, sem);
    ret = pthread_create(&pid2, NULL, pthread2, sem);

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);

    return 0;
}
