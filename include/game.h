#pragma once
#include "game/map.h"
#include "raylib.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
    Vector2 loc;
    Vector2 vel;
    float   speed;
} bullet_t;

typedef struct {
    bullet_t **items;
    size_t cap, len;
} bullets_t;

typedef struct
{
    Vector2 loc;
    Vector2 vel;
    float   speed;
    float   health;
    char    name[32];
} player_t;

typedef struct {
    Vector2 loc;
    size_t  namelen;
    char    name[];
} update_t;

typedef struct {
    player_t **items;
    size_t cap, len;
} players_t;

typedef struct {
    players_t players;
} game_state_t;

typedef struct {
    int         fps;
    int         screenWidth;
    int         screenHeight;
    player_t    currentPlayer;
    players_t   otherPlayers;
    Map         map;
} Game;

void GameInit(Game *game);
void GameUpdate(Game *game);
void GameDraw(Game *game);
void GameClose(Game *game);

