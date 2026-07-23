#include "game.h"
#include "raylib.h"
#include "utils.h"
#include "mutils.h"
#include "game/map.h"
#include "game/player.h"
#include "vec.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NO_DEBUG 1

#define PLAYER_NAMELEN_MAX  32

void GameInit(Game *game)
{
    printfln("Enter your game name: ");

    char *name = malloc(PLAYER_NAMELEN_MAX + 1);
    fgets(name, PLAYER_NAMELEN_MAX, stdin);
    name[PLAYER_NAMELEN_MAX] = 0;
    PlayerCreate(&game->currentPlayer, vec2(50, 50), vec2(1, 0), name);
#if NO_DEBUG
    SetTraceLogLevel(LOG_NONE);
#endif
    
    game->fps           = 60;
    game->screenHeight  = 450;
    game->screenWidth   = 800;    
    game->map.tileSize  = gcd(game->screenHeight, game->screenWidth);

    MapInit(&game->map, game->screenWidth / game->map.tileSize, game->screenHeight / game->map.tileSize);
    
    InitWindow(game->screenWidth, game->screenHeight, "Ze Game");
    InitAudioDevice();
    DisableCursor();
    SetTargetFPS(game->fps);

    memset(game->otherPlayers, 0, sizeof(game->otherPlayers)/sizeof(*game->otherPlayers));    

}

void GameDraw(Game *game)
{
    BeginDrawing();
        ClearBackground(GRAY);
    EndDrawing();
}

void GameClose(Game *game)
{
    CloseWindow();
}

void GameUpdate(Game *game)
{
    PlayerUpdate(&game->currentPlayer);    
    PlayerDraw(&game->currentPlayer);
    GameDraw(game);
}
