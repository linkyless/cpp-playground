#pragma once
#include "Vec2.hpp"
#include <cmath>

struct Boid {
    Vec2 position;
    Vec2 velocity;
    Boid(Vec2 position, Vec2 velocity) : position(position), velocity(velocity) {}
    
    void update(float deltaTime, const float width, const float height) {
        position = position + velocity * deltaTime;
        if (position.x > width) position.x = 0.0f;
        else if (position.x < 0) position.x = width;

        if (position.y > height) position.y = 0.0f;
        else if (position.y < 0) position.y = height;
    }
    
};
