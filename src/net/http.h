#ifndef HTTP_H
#define HTTP_H

#include "tcp.h"
#include "socket.h"

#define MAX_BUFF_SIZE 2048

typedef struct {
    const char ip[16]; /* IPV4 地址，够用了 */
    int port;
    tcp_t conn;
    char buff[MAX_BUFF_SIZE];
} http_t;

int http_get(http_t *http);

#endif /* HTTP_H */
