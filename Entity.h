#pragma once
#include "Vec2.h"

struct Entity {
    Vec2 pos;
    Vec2 vel;
    char icon;
    int color;
    bool active;
};