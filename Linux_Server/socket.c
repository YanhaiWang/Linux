#include <bits/socket.h>
#include <sys/un.h>

struct sockaddr {
    sa_family_t sa_family;
    char sa_data[14];
};