#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

constexpr float PI = 3.14159f;

struct Vec2 {
    float x, y;
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}
    
    float getModulus() const {
        return std::sqrt(x * x + y * y);
    }

    float dot(const Vec2 &other) const {
        return x * other.x + y * other.y;
    }

    Vec2 operator+(const Vec2 &other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2 &other) const {
        return Vec2(x - other.x, y - other.y);
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

    float getAngleDeg() {
        float angleRad = std::atan2f(x, y);
        float angleDeg = angleRad * 180.0f / PI;
        return angleDeg;
    }
};