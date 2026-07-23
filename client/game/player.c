#include "game/player.h"
#include "raylib.h"
#include "vec.h"

#define PLAYER_RADIUS 10.0f

void PlayerCreate(Player *player, Vector2 loc, Vector2 dir, char *name)
{
    player->loc = loc;   
    player->dir = dir;
    player->name = name;
    player->speed = 10.0f;
}

void PlayerDraw(Player *player)
{
    DrawLineEx(player->loc, VecAdd(player->loc, VecScale(player->dir, 50.0f)), 2.0f, BLUE);
    DrawCircle((int)player->loc.x, (int)player->loc.y, PLAYER_RADIUS, RED);
}

void PlayerUpdate(Player *player)
{
    Vector2 mouse = GetMousePosition();
    Vector2 dir = VecSub(mouse, player->loc);
    float distance = VecLen(mouse, player->loc); 

    if (distance > 0)
    {
        player->dir = VecScale(dir, 1 / distance);
    }

    if (IsKeyDown(KEY_W))
    {
        Vector2 velocity = VecScale(player->dir, player->speed);
        Vector2 newLoc = VecAdd(player->loc, velocity);
        player->loc = newLoc;
    }
}


