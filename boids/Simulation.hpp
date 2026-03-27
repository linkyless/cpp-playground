#pragma once
#include "Boids.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

struct Simulation {
    std::vector<Boid> boids;
    std::vector<Vec2> separationForces;
    std::vector<Vec2> allignmentForces;

    void resizeForces() {
        separationForces.resize((int) boids.size());
        // ...
    }

    void addBoid(Boid boid) {
        boids.push_back(boid);
    }

    void updateBoids(float deltaTime) {
        for (int i = 0; i < (int) boids.size(); i++) {
            boids[i].acceleration = separationForces[i];
            boids[i].update(deltaTime, kSizeWidth, kSizeHeight);
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

    void separation() {
        Vec2 SeparationForce(0.0f, 0.0f);
        for (int i = 0; i < (int) boids.size(); i++) {
            for (int j = 0; j < (int) boids.size(); j++) {
                if (i == j) continue;
                
                Boid a = boids[i];
                Boid b = boids[j];
                Vec2 sep({a.position - b.position});

                float distance = sep.getModulus();
                if (distance < a.radius) {
                    SeparationForce = SeparationForce + sep;
                }
            }
            separationForces[i] = SeparationForce;
            SeparationForce.x = SeparationForce.y = 0.0f;
        }
    }

};