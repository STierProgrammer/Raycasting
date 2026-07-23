#pragma once
#include "game/map.h"
#include "game/player.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
    int         fps;
    int         screenWidth;
    int         screenHeight;
    Player      currentPlayer;
    Map         map;
    Player      otherPlayers[3];
    int         currentClientFd;
} Game;

void GameInit(Game *game);
void GameUpdate(Game *game);
void GameDraw(Game *game);
void GameClose(Game *game);

