#include "tcp.h"
#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define tcp_error(str) fprintf(stderr, "%s: " str, __FUNCTION__)

/* Get a TCP connection */
int
tcp_connect(tcp_t *tcp, const char *ip, int port) {
    int sock_fd = INVALID_SOCKET;

    /* Compat windows */
    socket_init();
    sock_fd = socket_connect(ip, port);

    if (sock_fd == INVALID_SOCKET) {
        tcp_error("socket connection failed.\n");
        return -1;
    }
    tcp->fd = sock_fd;

    return 0;
}

size_t
tcp_read(tcp_t *tcp, void *buffer, size_t size) {
    return read(tcp->fd, buffer, size);
}

size_t
tcp_write(tcp_t *tcp, const void *buffer, size_t size) {
    return write(tcp->fd, buffer, size);
}

void
tcp_close(tcp_t *tcp) {
    if (tcp->fd > 0) {
        close(tcp->fd);
    }

    /* Compat windows */
    socket_end();
}
