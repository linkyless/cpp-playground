#pragma once
#include "Boids.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

float wSeparation = 2.65f;
float wAlignment  = 1.55f;
float wCohesion   = 1.49f;

// Maps a value from [minIn, maxIn] to [minOut, maxOut]
float mapRange(float value, float minIn, float maxIn, float minOut, float maxOut) {
    float t = (value - minIn) / (maxIn - minIn);
    if (t < 0.f) t = 0.f;
    if (t > 1.f) t = 1.f;
    return minOut + t * (maxOut - minOut);
}

struct Simulation {
    std::vector<Boid> boids;
    std::vector<Vec2> separationForces;
    std::vector<Vec2> alignmentForces;
    std::vector<Vec2> cohesionForces;

    // Trail effect: semi-transparent overlay drawn each frame
    sf::RectangleShape fadeOverlay;

    void init() {
        fadeOverlay.setSize({kSizeWidth, kSizeHeight});
        fadeOverlay.setFillColor(sf::Color(10, 10, 20, 120)); // dark navy, low alpha
    }

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
        // Draw fade overlay for trail effect
        window.draw(fadeOverlay);

        for (auto &boid : boids) {
            // Color by speed: slow = deep blue, fast = hot white-cyan
            float speed = boid.velocity.getModulus();
            uint8_t r = (uint8_t) mapRange(speed, 0.f, 300.f, 20.f,  180.f);
            uint8_t g = (uint8_t) mapRange(speed, 0.f, 300.f, 80.f,  230.f);
            uint8_t b = (uint8_t) mapRange(speed, 0.f, 300.f, 180.f, 255.f);
            sf::Color boidColor(r, g, b, 230);

            sf::ConvexShape triangle;
            triangle.setPointCount(3);
            triangle.setPoint(0, {0.f,  -12.f}); // tip
            triangle.setPoint(1, {8.f,   8.f});  // back-right
            triangle.setPoint(2, {-8.f,  8.f});  // back-left

            triangle.setFillColor(boidColor);
            triangle.setOutlineColor(sf::Color(255, 255, 255, 60));
            triangle.setOutlineThickness(0.6f);

            triangle.setOrigin({0.f, 0.f});
            triangle.setPosition({boid.position.x, boid.position.y});

            float angleDeg = boid.velocity.getAngleDeg();
            triangle.setRotation(sf::degrees(angleDeg + 90.0f));

            window.draw(triangle);
        }
    }

    void separation() {
        #pragma omp parallel for
        for (int i = 0; i < (int) boids.size(); i++) {
            Vec2 SeparationForce(0.0f, 0.0f);
            for (int j = 0; j < (int) boids.size(); j++) {
                if (i == j) continue;
                Boid a = boids[i];
                Boid b = boids[j];
                Vec2 sep(a.position - b.position);
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
        #pragma omp parallel for
        for (int i = 0; i < (int) boids.size(); i++) {
            Vec2 AlignmentForce(0.0f, 0.0f);
            int visited = 0;
            for (int j = 0; j < (int) boids.size(); j++) {
                if (i == j) continue;
                Boid a = boids[i];
                Boid b = boids[j];
                Vec2 sep(a.position - b.position);
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
        #pragma omp parallel for
        for (int i = 0; i < (int) boids.size(); i++) {
            Vec2 CohesionForce(0.0f, 0.0f);
            int visited = 0;
            for (int j = 0; j < (int) boids.size(); j++) {
                if (i == j) continue;
                Boid a = boids[i];
                Boid b = boids[j];
                Vec2 sep(a.position - b.position);
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