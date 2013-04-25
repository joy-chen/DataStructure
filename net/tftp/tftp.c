#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <fcntl.h>

#define SER_PORT 69
#define LEN 516
#define DATA_LEN 512

int send_pack(const char *, int, struct sockaddr_in *);
int recv_pack(const char *, int, struct sockaddr_in *);

int main()
{
    int ret;
    int sd;
    unsigned char recv_buff[LEN];
    struct sockaddr_in s_addr, c_addr;
    char *filename;
    socklen_t addrlen = sizeof(c_addr);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("Fail to socket\n");
        goto socket_fail;
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(SER_PORT);
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(sd, (struct sockaddr *)&s_addr, sizeof(s_addr));
    if (ret < 0) {
        perror("Fail to bind\n");
        goto bind_fail;
    }

    ret = recvfrom(sd, recv_buff, LEN, 0, (struct sockaddr *)&c_addr, &addrlen);
    if (ret < 0) {
        perror("Fail to recvfrom\n");
        goto recvfrom_fail;
    }

    filename = recv_buff + 2;
    switch (recv_buff[1]) {
        case 1:
            printf("start send\n");
            ret = send_pack(filename, sd, &c_addr);
            if (ret < 0) {
                perror("Fail to send_pack\n");
                goto send_pack_fail;
            }
            printf("send success");
            break;
        case 2:
            printf("start receive\n");
            ret = recv_pack(filename, sd, &c_addr);
            if (ret < 0) {
                perror("Fail to recv_pack\n");
                goto recv_pack_fail;
            }
            printf("receive success\n");
            break;
        default:
            goto send_pack_fail;
    }

    return 0;

recv_pack_fail:
send_pack_fail:
recvfrom_fail:
bind_fail:
    close(sd);
socket_fail:
    return -1;
}

int recv_pack(const char *filename, int sd, struct sockaddr_in *c_addr)
{
    int ret;
    int fd;
    int num = 1;
    int recv_len;
    unsigned char recv_buff[LEN], send_buff[LEN];
    socklen_t addrlen = sizeof(struct sockaddr_in);

    fd = open(filename, O_RDWR|O_CREAT, S_IRWXG|S_IRWXU);
    if (fd < 0) {
        perror("Fail to open\n");
        goto open_fail;
    }

    send_buff[0] = 0;
    send_buff[1] = 4;
    send_buff[2] = 0;
    send_buff[3] = 0;

    ret = sendto(sd, send_buff, 4, 0, (struct sockaddr *)c_addr, sizeof(*c_addr));
    if (ret < 0) {
        perror("Fail to sendto\n");
        goto sendto_fail;
    }

    while(1) {
        recv_len = recvfrom(sd, recv_buff, LEN, 0, (struct sockaddr *)c_addr, &addrlen);
        if (recv_len < 0) {
            perror("Fail to recvfrom\n");
            goto recvfrom_fail;
        }
        printf("recv_num = %d\n", recv_buff[3]);

        send_buff[2] = recv_buff[2];
        send_buff[3] = recv_buff[3];

        ret = sendto(sd, send_buff, 4, 0, (struct sockaddr *)c_addr, sizeof(*c_addr));
        if (ret < 0) {
            perror("Fail to sendto\n");
            goto sendto_fail;
        }
        ret = write(fd, recv_buff + 4, recv_len - 4);
        if (ret < 0) {
            perror("Fail to write\n");
            goto write_fail;
        } else if (ret < DATA_LEN) {
            goto success;
        }
    }

success:
    close(fd);
    return 0;

write_fail:
    recvfrom_fail:
sendto_fail:
    close(fd);
open_fail:
    return -1;
}

int send_pack(const char *filename, int sd, struct sockaddr_in *c_addr)
{
    int ret;
    int fd;
    int file_sendn;
    unsigned short num = 1;
    unsigned char send_buff[LEN], recv_buff[LEN];
    socklen_t addrlen = sizeof(struct sockaddr_in);

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Fail to open\n");
        goto open_fail;
    }

    while (1) {
        file_sendn = read(fd, send_buff + 4, DATA_LEN);
        if (file_sendn < 0) {
            perror("Fail to read\n");
            goto read_fail;
        }

        send_buff[0] = 0;
        send_buff[1] = 3;
        send_buff[2] = num >> 8;
        send_buff[3] = num & 0xFF;

        ret = sendto(sd, send_buff, file_sendn + 4, 0, (struct sockaddr *)c_addr, sizeof(*c_addr));
        if (ret < 0) {
            perror("Fail to sendto\n");
            goto sendto_fail;
        }

        while (1) {
            ret = recvfrom(sd, recv_buff, LEN, 0, (struct sockaddr *)c_addr, &addrlen);
            if (ret < 0) {
                continue;
            }

            if (recv_buff[2] != send_buff[2] || recv_buff[3] != send_buff[3]) {
                ret = sendto(sd, send_buff, file_sendn, 0, (struct sockaddr *)c_addr, sizeof(*c_addr));
                if (ret < 0) {
                    goto sendto_fail;
                }
                continue;
            } else if (file_sendn < DATA_LEN) {
                printf("num = %d, success\n", num);
                goto success;
            } else {
                num++;
                break;
            }
        }
    }

success:
    return 0;

sendto_fail:
read_fail:
    close(fd);
open_fail:
    return -1;
}
