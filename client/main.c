#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/poll.h>
#include <unistd.h>
#include "raylib.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "game.h"
#include <poll.h>

#if 0
static int client_fd;

void ConnectToServer(void)
{
    struct sockaddr_in server_addr;
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("Error: %s", strerror(errno));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
        die("Error: %s", strerror(errno));

    int status;
    if ((status = connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))))
        die("Error: %s", strerror(errno));

    size_t namelen = strlen(player.name);
    send(client_fd, &namelen, sizeof(namelen), 0);
    send(client_fd, player.name, namelen, 0);
}

struct pollfd fds[1];

void SendUpdate()
{
    send(client_fd, &player.loc, sizeof(player.loc), 0); 
}

void RecvUpdate()
{
    int poll_count = poll(fds, 1, 0);
    static int packet_type = -1;
    if (fds[0].revents & POLLIN)
    {
        ssize_t r = 0;
        for (;;)
        {
            r += recv(fds[0].fd, (char*)&packet_type + r, sizeof(packet_type) - r, 0);
            if (r == sizeof(packet_type))
                break;
        }
    }

    if (packet_type == PACKET_PLAYER_JOIN)
    {
        size_t namelen = 0;
        ssize_t r = 0;
        for (;;)
        {
            r += recv(fds[0].fd, (char*)&namelen + r, sizeof(namelen) - r, 0);
            if (r == sizeof(namelen))
                break;
        }       
        
        char *name = malloc(namelen + 1);
        r = 0;
        for (;;)
        {
            r += recv(fds[0].fd, (char*)name + r, namelen - r, 0);
            if (r == namelen)
                break;
        }       
        name[namelen] = 0;
        printfln("%s", name);
        packet_type = -1;
    }
}
#endif

int main(void)
{
    Game game = {0};
    GameInit(&game);
     
    while (!WindowShouldClose())
    {
        GameUpdate(&game);
    }   

    GameClose(&game);
    return 0;
}
