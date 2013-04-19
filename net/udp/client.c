#include "udp.h"

int main()
{
    int cs;
    int ret;
    char *buf = "joy";
    struct sockaddr_in s_addr;

    cs = socket(AF_INET, SOCK_DGRAM, 0);
    if (cs < 0) {
        perror("Fail to socket!");
        return -1;
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(SER_PORT);
    s_addr.sin_addr.s_addr = inet_addr(SER_IP);

    ret = connect(cs, (struct sockaddr *)&s_addr, sizeof(s_addr));
    if (ret < 0) {
        perror("Fail to connect!");
        goto connect_fail;
    }

    ret = sendto(cs, buf, 4, 0, (struct sockaddr *)&s_addr, sizeof(s_addr));
    if (ret < 0) {
        perror("Fail to sendto!");
        goto connect_fail;
    };

    return 0;

connect_fail:
    close(cs);
    return -1;
}
