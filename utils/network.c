#include "network.h"
#include <sys/socket.h>

ssize_t recvall(int fd, void *buf, size_t len)
{
    ssize_t r = 0;
    while (r < len)
    {
        ssize_t n = recv(fd, buf, len, 0);  
        if (n <= 0)
            return n;
        r += n;
    }
    return r;
}


