#include "conf.h"

#include "http.h"
#include "tcp.h"

#include <stdlib.h>
#include <string.h>

inline const char *
__http_header(const char *str, const char *header) {
    char *p, *end;
    if ((p = strstr(str, header)) == NULL)
        return NULL;
    if ((end = strstr(p, "\r\n")) == NULL)
        return NULL;
    *end = '\0';

    return p + strlen(header);
}

int
http_get(http_t *http) {
    size_t len = 0;

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

    /* 截至 \r\n\r\n 之前，包含所有 Headers */

    /**
     * 用于判断 Headers 结尾
     *
     * 0001 for ""
     * 0010 for "\r"
     * 0100 for "\r\n"
     * 1000 for "\r\n\r"
     * 0000 for "\r\n\r\n"
     */
    __int8_t flag = 0x01;
    for (char *s = http->buff;; s++) {
        if (tcp_read(&http->conn, s, 1) <= 0) {
            // Connection gone.
            return -1;
        }

        if (*s == '\r' && (flag & 0x05)) {
            flag = flag << 1;
        } else if (*s == '\n' && (flag & 0x0a)) {
            flag = flag << 1;
        } else {
            /* 请求头全部转小写 */
            if (*s > 'A' && *s < 'Z')
                *s += 0x20;
            /* 重置 flag */
            flag = 0x01;
        }

        /* 当 flag 后四位为0时说明 Headers 部分结束 */
        if (!(flag & 0x0f))
            break;
    }

    {
        const char *slen = __http_header(http->buff, "content-length:");
        len = atoi(slen);
    }

    tcp_read(&http->conn, http->buff, len);

    /* 关闭 tcp socket */
    // TODO 回收利用tcp连接以降低开销
    // TODO TFO优化，并提供编译选项
    tcp_close(&http->conn);

    return 0;
}
