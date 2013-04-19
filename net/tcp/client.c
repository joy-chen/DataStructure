#include <sys/socket.h>
#include <linux/in.h>
#include "tcp.h"

int main()
{
    int cd;
    int ret;
    struct sockaddr_in s_addr, c_addr;
    char *str = "abc";

    cd = socket(AF_INET, SOCK_STREAM, 0);
    if (cd < 0) {
        perror("Fail to socket");
        return -1;
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(SER_PORT);
    s_addr.sin_addr.s_addr = inet_addr(SER_IP);

    ret = connect(cd, (struct sockaddr *)&s_addr, sizeof(s_addr));
    if (ret < 0) {
        perror("Fail to bind");
        return -1;
    }

    ret = send(cd, str, 4, 0);
    if (ret < 0) {
        perror("Fail to send");
        return -1;
    }

    close(cd);
    return 0;
}
