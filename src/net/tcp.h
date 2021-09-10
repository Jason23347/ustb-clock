#ifndef TCP_H
#define TCP_H

#include "config.h"

#include <stddef.h>
#include "fiber/libfiber.h"

typedef struct
{
    socket_t fd;
} tcp_t;

int tcp_connect(tcp_t *tcp, const char *ip, int port);
void tcp_close(tcp_t *tcp);

size_t tcp_read(tcp_t *tcp, void *buffer, size_t size);
size_t tcp_write(tcp_t *tcp, const void *buffer, size_t size);

#endif /* TCP_H */
