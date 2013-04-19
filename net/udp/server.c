#include <stdio.h>
#include "udp.h"

int main()
{
    int ss;
    int ret;
    char buf[10];
    int recv_len;
    struct sockaddr_in s_addr, c_addr;

    ss = socket(AF_INET, SOCK_DGRAM, 0);
    if (ss < 0) {
        perror("Fail to socket!");
        return -1;
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(SER_PORT);
    s_addr.sin_addr.s_addr = inet_addr(SER_IP);

    ret = bind(ss, (struct sockaddr *)&s_addr, sizeof(s_addr));
    if (ret < 0) {
        perror("Fail to bind!");
        goto bind_fail;
    }

    ret = recvfrom(ss, buf, 10, 0, (struct sockaddr *)&c_addr, &recv_len);
    if (ret < 0) {
        perror("Fail to recvfrom");
        goto bind_fail;
    }

    buf[9] = '\0';
    printf("%s\n", buf);

    return 0;

bind_fail:
    close(ss);
    return -1;
}
