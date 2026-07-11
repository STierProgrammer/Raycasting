#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "darray.h"
#include "raylib.h"

typedef struct
{
    Vector2 loc;
    Vector2 vel;
    float   speed;
    float   health;
    const char *name;
} player_t;

#define square(x)   ((x) * (x))
#define abs(x)      ((x) >= 0 ? (x) : -(x))
#define min(a, b)   ((a) > (b) ? (b) : (a))
#define max(a, b)   ((a) > (b) ? (a) : (b))

int gcd(int a, int b) {
    if (a == 0) return b;
    if (b == 0) return a;
    if (a == b) return a;

    if (a % 2 == 0) {
        if (b % 2 == 0)
            return 2 * gcd(a / 2, b / 2);
        else
            return gcd(a / 2, b);
    } else {
        if (b % 2 == 0)
            return gcd(a, b / 2);
        else 
            return gcd(abs(a - b), min(a, b));
    }
}

#define printfln(fmt, ...) (fprintf(stdout, fmt "\n", ##__VA_ARGS__))

typedef struct {
    uint8_t*    map;
    int         width, height, tileSize;
} map_t;

void draw_map(map_t *map)
{
    for (int i = 0; i < map->width * map->height; i++)
    {
        int posX = i % map->width;
        int posY = i / map->width;
        Color color = BLACK;
        if (map->map[i] == 1)
        {
            color = WHITE;
        }

        DrawRectangle(posX * map->tileSize + 1, posY * map->tileSize - 1, map->tileSize - 1, map->tileSize - 1, color);
    }
}

typedef struct {
    Vector2 loc;
    Vector2 vel;
    float   speed;
} bullet_t;

typedef struct {
    bullet_t **items;
    size_t cap, len;
} bullets_t;

const int screenWidth = 800;
const int screenHeight = 450;

static int tileSize;
static int mapWidth;
static int mapHeight;

#define vec2(a, b) (Vector2){.x = (a), .y = (b)}

static inline int toTileIdx(Vector2 loc)
{
    return (int)loc.x / tileSize + mapWidth * ((int)loc.y / tileSize);
}

static inline Vector2 TileToLoc(int tileX, int tileY)
{
    return vec2((float)tileX * tileSize + (float)tileSize / 2, (float)tileY * tileSize + (float)tileSize / 2);
}

static inline Vector2 TileIdxToLoc(int tileIdx)
{
    int posX = tileIdx % mapWidth;
    int posY = tileIdx / mapWidth;
    return vec2((float)posX * tileSize + (float)tileSize / 2, (float)posY * tileSize + (float)tileSize / 2);
}

static inline int TileToTileIdx(int tileX, int tileY)
{
    return tileX + tileY * mapWidth;
}

static inline Vector2 RotateVector(Vector2 vec, float angle)
{
    return vec2(vec.x * cos(angle) - vec.y * sin(angle), vec.x * sin(angle) + vec.y * cos(angle));
}

void DrawVector(Vector2 start, Vector2 end)
{
    DrawLineEx(start, end, 2.0f, BLUE);
}

void createPlayer(player_t *player, const char *name, Vector2 loc)
{
    player->health = 100.0f;
    player->vel = vec2(0, 1);
    player->speed = 2.0f;
    player->loc = loc;
    player->name = name;
}

Vector2 VecAdd(Vector2 a, Vector2 b)
{
    return vec2(a.x + b.x, a.y + b.y);
}

Vector2 VecSub(Vector2 a, Vector2 b)
{
    return vec2(a.x - b.x, a.y - b.y);
}

Vector2 VecScale(Vector2 a, float b)
{
    return vec2(a.x * b, a.y * b);
}

Vector2 VecAbs(Vector2 a)
{
    return vec2(abs(a.x), abs(a.y));
}

float VecLen(Vector2 a)
{
    return sqrtf(square(a.x) + square(a.y));
}

Vector2 VecMax(Vector2 a, Vector2 b)
{
    return (a.x + a.y > b.x + b.y) ? a : b;
}

#define PLAYER_RADIUS 10.0f

int main(void)
{
    tileSize = gcd(screenWidth, screenHeight);
    mapWidth = screenWidth / tileSize;
    mapHeight = screenHeight / tileSize;
    
    map_t map = {
        .tileSize = tileSize,
        .height = screenHeight / tileSize,
        .width = screenWidth / tileSize,
        .map = malloc(mapHeight * mapWidth)
    };
    memset(map.map, 0, mapHeight * mapWidth);
    
    bullets_t bullets = {
        .len = 0,
        .cap = 16,
        .items = (bullet_t**)malloc(16 * sizeof(void*))
    };

    map.map[TileToTileIdx(5, 0)] = 1;
    map.map[TileToTileIdx(6, 0)] = 1;
    map.map[TileToTileIdx(6, 1)] = 1;

    InitWindow(screenWidth, screenHeight, "Raycasting");
    InitAudioDevice();
    DisableCursor();
    SetTargetFPS(60);

    Sound explosion = LoadSound("assets/sounds/explosion.wav");
    Sound shoot = LoadSound("assets/sounds/shoot.wav");

    player_t player;
    createPlayer(&player, "Sigma", TileToLoc(3, 4));

    double lastShot = 0.0;
    const double fireCooldown = 0.5;
    
    while (!WindowShouldClose())
    {
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
            
            int tileIdx = toTileIdx(VecAdd(new_loc, VecScale(player.vel, PLAYER_RADIUS)));
            Vector2 rectCenter = TileIdxToLoc(tileIdx);
            
            Vector2 relativeCenter = VecSub(player.loc, rectCenter);
            Vector2 offsetFromCorner = VecSub(
                    VecAbs(relativeCenter),
                    vec2((float)tileSize / 2, (float)tileSize / 2)
                );

            float distance = 
                min(max(offsetFromCorner.x, offsetFromCorner.y), 0)
                + VecLen(VecMax(offsetFromCorner, vec2(0, 0)))
                - PLAYER_RADIUS;

            if (distance > 0 || map.map[tileIdx] == 0)
            {
                player.loc = new_loc;
            }
        }

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

            for (size_t i = 0; i < bullets.len; i++)
            {
                bullet_t *bullet = bullets.items[i];
                DrawCircle(bullet->loc.x, bullet->loc.y, 10, BLUE);
            }
        
            DrawText(player.name, player.loc.x - 2.5 * PLAYER_RADIUS, player.loc.y - 3.5 * PLAYER_RADIUS, 20, GREEN);
            DrawCircle(player.loc.x, player.loc.y, (map.tileSize) / 4.0, RED);
            DrawText(TextFormat("Health: %.2f", player.health), 0, 0, 20,GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


