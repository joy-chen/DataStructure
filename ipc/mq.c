#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

struct msgbuf {
    long mtype;
    char mtext[10];
};

int main()
{
    int msgid;
    pid_t pid;

    msgid = msgget((key_t)1234, IPC_CREAT);
    //...

    pid = fork();
    if (pid == 0) {
        struct msgbuf msgbuf;
        sleep(1);
        msgrcv(msgid, &msgbuf, sizeof(msgbuf.mtext), 1, IPC_NOWAIT);
        printf("recv = %s\n", msgbuf.mtext);
    } else if (pid > 0) {
        char *buf = "DAME IT";
        struct msgbuf msgbuf;

        msgbuf.mtype = 1;
        memcpy(msgbuf.mtext, buf, 8);
        msgsnd(msgid, &msgbuf, sizeof(msgbuf.mtext), IPC_NOWAIT);
    } else {
    //...
    }

    return 0;
}
