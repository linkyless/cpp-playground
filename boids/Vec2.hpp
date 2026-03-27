#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

struct Vec2 {
    float x, y;
    Vec2(float x, float y) : x(x), y(y) {}
    
    float getModulus() const {
        return sqrt(x * x + y * y);
    }

    float dot(const Vec2 &other) const {
        return x * other.x + y * other.y;
    }

    Vec2 operator+(const Vec2 &other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator*(const Vec2 &other) const {
        return Vec2(x * other.x, y * other.y);
    }

    Vec2 operator*(const float &k) const {
        return Vec2(x * k, y * k);
    }

    Vec2 normalize() {
        float magnitude = getModulus();
        if (magnitude < 1e-6f) return Vec2(x, y);
        return Vec2(x / magnitude, y / magnitude);
    }
};