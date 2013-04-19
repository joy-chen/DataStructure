#include <sys/socket.h>
#include <linux/in.h>
#include <stdio.h>
#include "tcp.h"


int main()
{
    int sd, cd;
    int ret;
    char buf[10];
    struct sockaddr_in s_addr, c_addr;
    int addr_len;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("Fail to socket");
        return -1;
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(SER_PORT);
    s_addr.sin_addr.s_addr = inet_addr(SER_IP);

    ret = bind(sd, (struct sockaddr *)&s_addr, sizeof(s_addr));
    if (ret < 0) {
        perror("Fail to bind");
        return -1;
    }

    ret = listen(sd, 10);
    if (ret < 0) {
        perror("Fail to listen");
        return -1;
    }

    cd = accept(sd, (struct sockaddr*)&c_addr, &addr_len);
    if (cd < 0) {
        perror("Fail to accecpt");
        goto accept_fail;
    }

    recv(cd, buf, 10, 0);
    if (cd < 0) {
        perror("Fail to recv");
        goto recv_fail;
    }
    buf[9] = '\0';
    printf("%s\n", buf);

    return 0;
recv_fail:
    close(cd);
accept_fail:
    close(sd);
    return -1;
}
