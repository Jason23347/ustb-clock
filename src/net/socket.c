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
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#endif /* defined(_WIN32) || defined(_WIN64) */
#include "socket.h"
#include "tui/draw.h"
#include <stdarg.h>

#ifndef NDEBUG
/** 在tty左上角输出错误信息。 */
void
socket_error(const char *format, ...) {
    va_list args;

    savecursor();
    gotoxy(0, 0);

    printf("socket: ");
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    loadcursor();
}
#else
#define socket_error(...)
#endif

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
        socket_error("%s: connect %s:%d error %s\r\n", __FUNCTION__, ip, port,
                     strerror(errno));
        return INVALID_SOCKET;
    }
    return fd;
}
