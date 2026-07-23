#pragma once
#include "raylib.h"
#include <math.h>
#include "utils.h"

#define vec2(a, b) (Vector2){ .x = (a), .y = (b)}

static inline Vector2 RotateVector(Vector2 vec, float angle)
{
    return vec2(vec.x * cosf(angle) - vec.y * sinf(angle), vec.x * sinf(angle) + vec.y * cosf(angle));
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


