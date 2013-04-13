#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define LEN 20

int main()
{
    int shmid;
    pid_t pid;
    char *buf;

    shmid = shmget((key_t)123, LEN, IPC_CREAT);
    if (shmid < 0) {
        perror("Fail to shmget");
        return 0;
    }

    buf = (char *)shmat(shmid, NULL, 0666);
    if (buf == (void *)-1) {
        perror("Fail to shmat");
        return -1;
    }

    pid = fork();
    if (pid == 0) {
        sleep(1);
        printf("buf = %s\n", buf);
    } else if (pid > 0) {
        int i;

        for (i = 0; i < 10; i++) {
            *buf = i + '0';
            buf++;
        }
        buf[LEN - 1] = '\0';
        sleep(2);
        while(1);
    } else {
        perror("Fail to fork");
        shmdt((const void *)buf);
        return -1;
    }
}
