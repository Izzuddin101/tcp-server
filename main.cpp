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

// socket() -> bind() -> listen()
int main() {
    // - Start of Socket - 
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    // - End of Socket -

    // - Start of Bind -
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(0); // Wildcard addr, allowing any to connect
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));

    if (ret < 0) {
        printf("bind() failed: %s\n", strerror(errno));
        abort();
    }

    // - End of Bind -

    // - Start of Listen -
    ret = listen(fd, SOMAXCONN);

    if (ret < 0) {
        printf("listen() failed: %s\n", strerror(errno));
        abort();
    }
    // - End of Listen -

    // - Start of Accept -
    while(true) {
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int client_fd = accept(fd, (struct sockaddr*)&client_addr, &addrlen);
        if (client_fd < 0) {
            printf("accept() failed: %s\n", strerror(errno));
            continue;
        }
        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Read data from the client and return something
        char buffer[1024] = {};
        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

        if (bytes_read < 0) {
            printf("read() failed: %s\n", strerror(errno));
        } else {
            // Process the received data (example: echo it back)
            write(client_fd, buffer, bytes_read);
        }

        close(client_fd);
    }
    return 0;
}