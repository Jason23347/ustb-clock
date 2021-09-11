#include "http.h"
#include "tcp.h"

#include <string.h>

#define HTTP_TIMEOUT 30

int
http_get(http_t *http) {
    if (tcp_connect(&http->conn, http->ip, http->port) == -1) {
        return -1;
    }

    const char text[] = "GET / HTTP/1.1\r\n"
                        "User-Agent: Mozilla/5.0\r\n\r\n";

    if (tcp_write(&http->conn, text, sizeof(text)) == -1) {
        return -1;
    }

    size_t len = 0;
    char tmp[64];
    for (char *p = http->buff; len < MAX_BUFF_SIZE;) {
        int n = tcp_read(&http->conn, tmp, 64);
        if (n <= 0) {
            break;
        }
        strncpy(p, tmp,
                ((len + n) > MAX_BUFF_SIZE) ? (MAX_BUFF_SIZE - len) : n);
        len += n;
        p += n;
    }

    tcp_close(&http->conn);

    return 0;
}
