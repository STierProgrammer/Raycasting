#pragma once
#include "mutils.h"
#include "raylib.h"
#include <math.h>
#include "utils.h"

#define vec2(a, b) (Vector2){ .x = (a), .y = (b)}

static inline Vector2 RotateVector(Vector2 vec, float angle)
{
    return vec2(vec.x * cosf(angle) - vec.y * sinf(angle), vec.x * sinf(angle) + vec.y * cosf(angle));
}

static inline Vector2 VecAdd(Vector2 a, Vector2 b)
{
    return vec2(a.x + b.x, a.y + b.y);
}

static inline Vector2 VecSub(Vector2 a, Vector2 b)
{
    return vec2(a.x - b.x, a.y - b.y);
}

static inline Vector2 VecScale(Vector2 a, float b)
{
    return vec2(a.x * b, a.y * b);
}

static inline float VecLen(Vector2 s, Vector2 e)
{
    return sqrtf(square(e.x - s.x) + square(e.y - s.y));
}
