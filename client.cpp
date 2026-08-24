#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <iostream>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        printf("socket() failed: %s\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);  // 127.0.0.1
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        printf("connect() failed: %s\n", strerror(errno));
        return 1;
    }

    char msg[] = "Poopy butthole in the butt";
    write(fd, msg, strlen(msg));

    char rbuf[1024] = {};
    ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        printf("read() failed: %s\n", strerror(errno));
        return 1;
    }
    printf("server says: %s\n", rbuf);
    close(fd);

    return 0;
}