#include "game/multiplayer.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include "utils.h"
#include "conf.h"

void ClientConnentToServer(Game *game)
{
    struct sockaddr_in server_addr;
    if ((game->currentClientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("Error: %s", strerror(errno));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
        die("Error: %s", strerror(errno));

    int status;
    if ((status = connect(game->currentClientFd, (struct sockaddr*)&server_addr, sizeof(server_addr))))
        die("Error: %s", strerror(errno));

    size_t namelen = strlen(game->currentPlayer.name);
    send(game->currentClientFd, &namelen, sizeof(namelen), 0);
    send(game->currentClientFd, game->currentPlayer.name, namelen, 0);
}

// TODO: 
void ClientSendUpdate(Game *game);
void ClientRecvUpdate(Game *game);


