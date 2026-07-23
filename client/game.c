#include "game.h"
#include "raylib.h"
#include "utils.h"
#include "mutils.h"
#include "game/map.h"

#include <stdio.h>

#define NO_DEBUG 1

void GameInit(Game *game)
{
    printfln("Enter your game name: ");
    fgets(game->currentPlayer.name, 32, stdin);
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
}

void GameDraw(Game *game)
{
    BeginDrawing();
        ClearBackground(GRAY);
    EndDrawing();
}
