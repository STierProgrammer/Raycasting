#include <bits/posix1_lim.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include "conf.h"
#include "game.h"
#include "raylib.h"
#include "utils.h"
#include "darray.h"

typedef struct {
    int         sockfd;
    const char* name;
    size_t      namelen;
} client_t;

typedef struct {
    client_t *items;
    size_t cap, len;
} clients_t;

static clients_t clients;

client_t *find_client(int sockfd)
{
    for (size_t i = 0; i < clients.len; i++)
    {
        if (clients.items[i].sockfd == sockfd)
            return &clients.items[i];
    }
    return NULL;
}

int main()
{
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_family = AF_INET;

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("Error: %s", strerror(errno));
    
    const int enable = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        die("Error: %s",strerror(errno));
    if (listen(sockfd, 5) < 0)
        die("Error: %s", strerror(errno));

    struct pollfd fds[10];
    int nfds = 1;

    fds[0].fd = sockfd;
    fds[0].events = POLLIN;

    clients = (clients_t){
        .len = 0,
        .cap = 16,
        .items = malloc(sizeof(client_t) * 16)
    };

    for (;;)
    {
        int poll_count = poll(fds, nfds, -1);
        (void)poll_count;

        if (fds[0].revents & POLLIN)
        {
            struct sockaddr_in client_addr;
            socklen_t client_size = sizeof(client_addr);

            int client_sock = accept(sockfd, (struct sockaddr*)&client_addr, &client_size);
            if (client_sock < 0)
            {
                perror("Failed to accept a client");
                continue;
            }

            printfln("New client: %d", client_sock);
            fds[nfds].fd = client_sock;
            fds[nfds].events = POLLIN;
            nfds++;

            // TODO: Handle errors
            size_t namelen;
            ssize_t r = 0;
            for (;;)
            {
                r += recv(client_sock, (char*)&namelen + r, sizeof(namelen) - r, 0);
                if (r == sizeof(namelen))
                    break;
            }

            char *name = malloc(namelen + 1);
            r = 0;
            for (;;)
            {
                r += recv(client_sock, name + r, namelen - r, 0);
                if (r == namelen)
                    break;
            }
            name[namelen] = '\0';
            printfln("%s", name);
            client_t client = (client_t){
                .name    = name,
                .namelen = namelen,
                .sockfd  = client_sock
            };
            
            int packet_type = PACKET_PLAYER_JOIN;
            for (size_t i = 0; i < clients.len; i++)
            {
                send(clients.items[i].sockfd, &packet_type, sizeof(packet_type), 0);
                send(clients.items[i].sockfd, &client.namelen, sizeof(client.namelen), 0);
                send(clients.items[i].sockfd, client.name, client.namelen, 0);
            }

            da_push(&clients, client);
        }
        
        for (size_t i = 1; i < nfds; i++)
        { 
            if (fds[i].revents & POLLIN)
            { 
                Vector2 update_loc;
                ssize_t r = 0;
                for (;;)
                {
                    r += recv(fds[i].fd, &update_loc + r, sizeof(update_loc) - r, 0);
                    if (r == sizeof(update_loc))
                        break;
                }
            
                client_t *client = find_client(fds[i].fd);
                send(client->sockfd, &update_loc, sizeof(update_loc), 0);
            } 
        }
    }

    close(sockfd);

    return 0;
}
