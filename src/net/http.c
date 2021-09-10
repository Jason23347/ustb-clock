#include "http.h"
#include "buff.h"
#include "tcp.h"

#define HTTP_TIMEOUT 30

int
http_get(http_t *http, buff_t *buff) {
    if (tcp_connect(&http->conn, http->ip, http->port) == -1) {
        return -1;
    }

    const char text[] =
        "GET / HTTP/1.1\r\n"
        "User-Agent: Mozilla/5.0\r\n\r\n";

    if (tcp_write(&http->conn, text, sizeof(text)) == -1) {
        return -1;
    }

    for (char *p = buff->str; buff->len < buff->max;) {
        int n = tcp_read(&http->conn, p, 64);
        if (n <= 0) {
            break;
        }
        buff->len += n;
        p += n;
    }

    tcp_close(&http->conn);

    return 0;
}
