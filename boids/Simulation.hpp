#pragma once
#include "Boids.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

float wSeparation = 1.43f;
float wAlignment  = 1.30f;
float wCohesion   = 1.05f;

struct Simulation {
    std::vector<Boid> boids;
    std::vector<Vec2> separationForces;
    std::vector<Vec2> alignmentForces;
    std::vector<Vec2> cohesionForces;
    
    void resetAcceleration() {
        for (auto &boid : boids) 
            boid.acceleration.x = boid.acceleration.y = 0.0f;
    }

    void resizeForces() {
        separationForces.resize((int) boids.size());
        alignmentForces.resize((int) boids.size());
        cohesionForces.resize((int) boids.size());
    }

    void addBoid(Boid boid) {
        boids.push_back(boid);
    }

    void updateBoids(float deltaTime) {
        for (int i = 0; i < (int) boids.size(); i++) {
            boids[i].acceleration = boids[i].acceleration + (separationForces[i] * wSeparation);
            boids[i].acceleration = boids[i].acceleration + (alignmentForces[i] * wAlignment);
            boids[i].acceleration = boids[i].acceleration + (cohesionForces[i] * wCohesion);
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

    void alignment() {
        Vec2 AlignmentForce(0.0f, 0.0f);
        int visited = 0;
        for (int i = 0; i < (int) boids.size(); i++) {
            visited = 0;
            AlignmentForce.x = AlignmentForce.y = 0.0f;
            for (int j = 0; j < (int) boids.size(); j++) {
                if (i == j) continue;
                Boid a = boids[i];
                Boid b = boids[j];
                Vec2 sep({a.position - b.position});
                float distance = sep.getModulus();
                if (distance < a.radius) {
                    visited++;
                    AlignmentForce = AlignmentForce + boids[j].velocity;
                }
            }
            if (visited == 0) continue;
            AlignmentForce = AlignmentForce * (1.0f / (float) visited);
            alignmentForces[i] = AlignmentForce - boids[i].velocity;
        }
    }

    void cohesion() {
        Vec2 CohesionForce(0.0f, 0.0f);
        int visited = 0;
        for (int i = 0; i < (int) boids.size(); i++) {
            visited = 0;
            CohesionForce.x = CohesionForce.y = 0.0f;
            for (int j = 0; j < (int) boids.size(); j++) {
                if (i == j) continue;
                Boid a = boids[i];
                Boid b = boids[j];
                Vec2 sep({a.position - b.position});
                float distance = sep.getModulus();
                if (distance < a.radius) {
                    visited++;
                    CohesionForce = CohesionForce + boids[j].position;
                }
            }
            if (visited == 0) continue;
            CohesionForce = CohesionForce * (1.0f / (float) visited);
            cohesionForces[i] = CohesionForce - boids[i].position;
        }
    }

};