#include "http.h"
#include "tcp.h"

#include <string.h>

#define HTTP_TIMEOUT 30

int
http_get(http_t *http) {
    /* 开启一个 tcp socket */
    if (tcp_connect(&http->conn, http->ip, http->port) == -1) {
        return -1;
    }

    /* 反正发点啥服务器都返回主页 */
    const char text[] = "GET / HTTP/1.1\r\n"
                        "User-Agent: Mozilla/5.0\r\n\r\n";

    /* 发送 HTTP 请求 */
    if (tcp_write(&http->conn, text, sizeof(text)) == -1) {
        return -1;
    }

    /* 因为没有实现对 Content-Length 的支持，
       所以最多接受 MAX_BUFF_SIZE 个字节的数据。 */
    size_t len = 0;
    char tmp[64]; /* buffer for buffer */
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

    /* 关闭 tcp socket */
    tcp_close(&http->conn);

    return 0;
}
