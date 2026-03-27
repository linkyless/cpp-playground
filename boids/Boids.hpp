#pragma once
#include "Vec2.hpp"
#include <cmath>

constexpr float defaultRadius = 80.0f;
constexpr float kSizeWidth = 800.0f;
constexpr float kSizeHeight = 600.0f;
constexpr float maxVelocity = 700.0f;

struct Boid {
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    Boid(Vec2 position, Vec2 velocity, Vec2 acceleration) : position(position), velocity(velocity), acceleration(acceleration) {}
    
    const float radius = defaultRadius;

    void update(float deltaTime, const float width, const float height) {
        // Velocity
        velocity = velocity + acceleration * deltaTime;
        if (velocity.getModulus() > maxVelocity) {
            velocity = velocity.normalize() * maxVelocity;
        }
        // Position
        position = position + velocity * deltaTime;

        // wrap-around
        if (position.x > width) position.x = 0.0f;
        else if (position.x < 0) position.x = width;
        if (position.y > height) position.y = 0.0f;
        else if (position.y < 0) position.y = height;


    }
};
