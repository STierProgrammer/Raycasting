#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "darray.h"
#include "raylib.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "conf.h"
#include "game.h"
#include "vec.h"
#include <errno.h>
#include "utils.h"
#include <poll.h>
#include "mutils.h"

static player_t player;

void DrawVector(Vector2 start, Vector2 end)
{
    DrawLineEx(start, end, 2.0f, BLUE);
}

void createPlayer(player_t *player, Vector2 loc)
{
    player->health = 100.0f;
    player->vel = vec2(0, 1);
    player->speed = 2.0f;
    player->loc = loc;
}

#define PLAYER_RADIUS 10.0f

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

int main(void)
{
    Game game = {0};
    GameInit(&game);

    players_t players = {
        .cap = 16,
        .len = 0,
        .items = malloc(16 * sizeof(player_t))
    };

    ConnectToServer();

    fds[0].fd = client_fd;
    fds[0].events = POLLIN;
    
    double lastShot = 0.0;
    const double fireCooldown = 0.5;
    
    while (!WindowShouldClose())
    {
        RecvUpdate();
        Vector2 mouse = GetMousePosition();
        
        Vector2 dir = {
            mouse.x - player.loc.x,
            mouse.y - player.loc.y
        };

        float distance = sqrtf(square(mouse.x - player.loc.x) + square(mouse.y - player.loc.y));
        player.vel = (Vector2){ .x = dir.x / distance, .y = dir.y / distance };

        if (IsKeyDown(KEY_W))
        {
            Vector2 new_loc;
            new_loc.x = player.loc.x + player.vel.x * player.speed;
            new_loc.y = player.loc.y + player.vel.y * player.speed; 

            player.loc = new_loc;
        }

#if 0
        for (size_t i = 0; i < bullets.len; i++)
        {
            bullet_t *bullet = bullets.items[i];
            bullet->loc.x += bullet->vel.x * bullet->speed;
            bullet->loc.y += bullet->vel.y * bullet->speed;

            if (bullet->loc.x - 10 < 0 || bullet->loc.x + 10 > screenWidth || bullet->loc.y - 10 < 0 || bullet->loc.y + 10 > screenHeight)
            {
                PlaySound(explosion);
                da_remove_unordered(&bullets, i);
                free(bullet);
                continue;
            }

            int tileIdx = toTileIdx(bullet->loc);
            if (map.map[tileIdx] == 1)
            {
                PlaySound(explosion);
                da_remove_unordered(&bullets, i);
                free(bullet);
            }
        }
#endif
        
#if 0
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            double now = GetTime();

            if (now - lastShot >= fireCooldown)
            {
                lastShot = now;
                bullet_t *bullet = malloc(sizeof(bullet_t));
                bullet->loc = player.loc;
                bullet->speed = 4.0f;
                bullet->vel = player.vel;
                da_push(&bullets, bullet); 
                PlaySound(shoot);
            }
        }
#endif

        float slope = player.vel.y / player.vel.x;
        
        BeginDrawing();
            ClearBackground(GRAY);
            draw_map(&map);
            DrawLineEx(
                    player.loc, 
                    VecAdd(player.loc, VecScale(player.vel, 50.0f)),
                    2.0f, 
                    BLUE
                );

#if 0
            for (size_t i = 0; i < bullets.len; i++)
            {
                bullet_t *bullet = bullets.items[i];
                DrawCircle(bullet->loc.x, bullet->loc.y, 10, BLUE);
            }
#endif

            DrawText(player.name, player.loc.x - 2.5 * PLAYER_RADIUS, player.loc.y - 3.5 * PLAYER_RADIUS, 20, GREEN);
            DrawCircle(player.loc.x, player.loc.y, (map.tileSize) / 4.0, RED);
            DrawText(TextFormat("Health: %.2f", player.health), 0, 0, 20,GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
