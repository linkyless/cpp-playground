#include <SFML/Graphics.hpp>
#include "Vec2.hpp"
#include "Boids.hpp"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(-3, 3);

constexpr float PI = 3.14159f;
constexpr float kSizeWidth = 800.0f;
constexpr float kSizeHeight = 600.0f;

int main() {

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Boids Sim");
    sf::Clock clock;
    Vec2 position(0, 0);
    Vec2 velocity(180.0f, 180.0f);
    Boid boid(position, velocity);
    sf::CircleShape triangle(13.0f, 3);
    triangle.setPosition({boid.position.x, boid.position.y});
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        while (const std::optional event = window.pollEvent()) {
            if (event -> is<sf::Event::Closed>())
                window.close();
        }
        
        window.clear(sf::Color::Black);
        boid.update(dt.asSeconds(), kSizeWidth, kSizeHeight);
        float angleRad = std::atan2f(boid.velocity.y, boid.velocity.x);
        float angleDeg = angleRad * 180.0f / PI;

        triangle.setPosition({boid.position.x, boid.position.y});
        triangle.setRotation(sf::degrees(angleDeg + 90.0f));
        window.draw(triangle);
        window.display();
        
    }
}