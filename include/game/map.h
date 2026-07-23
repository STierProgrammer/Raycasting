#pragma once
#include "raylib.h"
#include <stdint.h>

typedef struct {
    uint8_t*    map;
    int         width;
    int         height;
    int         tileSize;
} Map;

void    MapInit(Map *map, int width, int height);
void    MapDraw(Map *map);
int     LocToTileIdx(Map *map, Vector2 loc);

