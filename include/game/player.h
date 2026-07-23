#pragma once
#include "raylib.h"

typedef struct {
    Vector2 loc;
    Vector2 dir;
    float   speed;
    char    *name;
} Player;

void PlayerCreate(Player *player, Vector2 loc, Vector2 dir, char *name);
void PlayerDraw(Player *player);
void PlayerUpdate(Player *player);


