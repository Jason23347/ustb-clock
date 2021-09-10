#ifndef SOCKET_H
#define SOCKET_H

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#define HAVE_NO_GETOPT
#define snprintf        _snprintf
extern int optind;
extern char *optarg;
int getopt(int argc, char *const argv[], char *opts);
#else
#define SOCKET          int
#define INVALID_SOCKET  -1
#endif /* defined(_WIN32) || defined(_WIN64) */

void socket_init(void);
void socket_end(void);
void socket_close(SOCKET fd);
SOCKET socket_listen(const char *ip, int port);
SOCKET socket_accept(SOCKET fd);
SOCKET socket_connect(const char *ip, int port);

#endif /* SOCKET_H */
