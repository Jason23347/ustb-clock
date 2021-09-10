#ifndef HTTP_H
#define HTTP_H

#include "buff.h"
#include "tcp.h"
#include "socket.h"

typedef struct {
    const char ip[16];
    int port;
    tcp_t conn;
    buff_t buff;
} http_t;

int http_get(http_t *http, buff_t *buff);

#endif /* HTTP_H */
