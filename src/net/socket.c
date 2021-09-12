#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#endif /* defined(_WIN32) || defined(_WIN64) */
#include "socket.h"

#define socket_error(...) fprintf(stderr, "socket: " __VA_ARGS__)

void
socket_init(void) {
#if defined(_WIN32) || defined(_WIN64)
    WORD version = 0;
    WSADATA data;

    FillMemory(&data, sizeof(WSADATA), 0);
    version = MAKEWORD(2, 0);

    if (WSAStartup(version, &data) != 0) {
        abort();
    }
#else
    signal(SIGPIPE, SIG_IGN);
#endif
}

void
socket_end(void) {
#if defined(_WIN32) || defined(_WIN64)
    WSACleanup();
#endif
}

void
socket_close(SOCKET fd) {
    close(fd);
}

SOCKET
socket_listen(const char *ip, int port) {
    SOCKET fd;
    int on;
    struct sockaddr_in sa;

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = inet_addr(ip);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == INVALID_SOCKET) {
        socket_error("create socket error, %s\r\n", strerror(errno));
        getchar();
        exit(1);
    }

#if defined(_WIN32) || defined(_WIN64)
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&on,
                   sizeof(on))) {
#else
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) {
#endif
        socket_error("setsockopt error %s\r\n", strerror(errno));
        exit(1);
    }

    if (bind(fd, (struct sockaddr *)&sa, sizeof(struct sockaddr)) < 0) {
        socket_error("bind error %s\r\n", strerror(errno));
        getchar();
        exit(1);
    }

    if (listen(fd, 128) < 0) {
        socket_error("listen error %s\r\n", strerror(errno));
        getchar();
        exit(1);
    }

    return fd;
}

SOCKET
socket_accept(SOCKET fd) {
    SOCKET cfd;
    struct sockaddr_in sa;
    int len = sizeof(sa);

    cfd = accept(fd, (struct sockaddr *)&sa, (socklen_t *)&len);
    return cfd;
}

SOCKET
socket_connect(const char *ip, int port) {
    SOCKET fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sa;
    socklen_t len = (socklen_t)sizeof(sa);

    if (fd == INVALID_SOCKET) {
        socket_error("create socket error %s\r\n", strerror(errno));
        return INVALID_SOCKET;
    }

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = inet_addr(ip);

    if (connect(fd, (const struct sockaddr *)&sa, len) < 0) {
        close(fd);
        socket_error("%s: connect %s:%d erorr %s\r\n", __FUNCTION__, ip, port,
                     strerror(errno));
        return INVALID_SOCKET;
    }
    return fd;
}
