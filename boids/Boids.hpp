#pragma once
#include "Vec2.hpp"

struct Boid {
    Vec2 x;
    Vec2 v;
    Boid(Vec2 pos, Vec2 velocity) : x(pos), v(velocity) {}
}
