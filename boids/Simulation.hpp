#pragma once
#include "Boids.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

struct Simulation {
    std::vector<Boid> boids;

    void addBoid(Boid boid) {
        boids.push_back(boid);
    }

    void updateBoids(float deltaTime) {
        for (auto &boid : boids) {
            // Position
            boid.update(deltaTime, kSizeWidth, kSizeHeight);
        }
    }

    void drawBoids(sf::RenderWindow &window) {
        for (auto &boid : boids) {
            // Position
            sf::CircleShape triangle(13.0f, 3);
            triangle.setPosition({boid.position.x, boid.position.y});

            // Rotation
            float angleDeg = boid.velocity.getAngleDeg();
            triangle.setRotation(sf::degrees(angleDeg + 90.0f));            
            window.draw(triangle);
        }
    }

};