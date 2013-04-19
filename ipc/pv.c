#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <pthread.h>

int p(int semid)
{
    struct sembuf sops;

    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = 0;

    semop(semid, &sops, 1);
    return 0;
}

int v(int semid)
{
    struct sembuf sops;

    sops.sem_num = 0;
    sops.sem_op = 1;
    sops.sem_flg = 0;

    semop(semid, &sops, 1);
    return 0;
}

int sem_init(int semid, int val)
{
    int ret;

    ret = semctl(semid, 0, SETVAL, val);
    if (ret < 0)
        return -1;

    return 0;
}

void *p1_handle(void *arg)
{
    int semid = *(int *)arg;

    while (1) {
        p(semid);
        printf("p1\n");
        sleep(1);
        v(semid);
    }
}

void *p2_handle(void *arg)
{
    int semid = *(int *)arg;

    while(1) {
        p(semid);
        printf("p2\n");
        v(semid);
        sleep(1);
    }
}

int main()
{
    int semid;
    int pid;
    int ret;
    key_t key;

    key = ftok(".", 'a');
    if (key < 0) {
        perror("Fail to ftok!");
        return -1;
    }

    semid = semget(key, 1, IPC_CREAT);
    if (semid < 0) {
        perror("Fail to semget!");
        return -1;
    }

    ret = sem_init(semid, 1);
    if (ret < 0) {
        perror("Fail to sem_init!");
        goto sem_init_fail;
    }

    pid = fork();
    if (pid > 0) {
        p1_handle(&semid);
    } else if (pid == 0) {
        p2_handle(&semid);
    } else {
        perror("Fail to fork!");
        goto sem_init_fail;
    }

    return 0;
sem_init_fail:
    semctl(semid, 0, IPC_RMID);
    return -1;
}
