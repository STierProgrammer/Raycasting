#include "game.h"
#include "game/map.h"
#include "raylib.h"
#include <stdlib.h>
#include <string.h>

void MapInit(Map *map, int width, int height)
{
    map->width  = width;
    map->height = height;
    map->map    = malloc(width * height * sizeof(uint8_t));
    memset(map->map, 0, width * height);
}

void MapDraw(Map *map)
{
    for (int i = 0; i < map->width * map->height; i++)
    {
        int posX = i % map->width;
        int posY = i / map->width;
        Color color = BLACK;
        if (map->map[i] == 1)
            color = WHITE;
        DrawRectangle(posX * map->tileSize + 1, posY * map->tileSize - 1, map->tileSize - 1, map->tileSize - 1, color);
    }
}

int LocToTileIdx(Map *map, Vector2 loc)
{
    return (int)loc.x / map->tileSize + map->width * ((int)loc.y / map->tileSize);
}

