#include <SFML/Graphics.hpp>
#include "Vec2.hpp"
#include "Boids.hpp"
#include "Simulation.hpp"
#include <random>
#include <vector>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(-3, 3);

int main() {

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Boids Sim");
    sf::Clock clock;

    Simulation sim;
   
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        while (const std::optional event = window.pollEvent()) {
            if (event -> is<sf::Event::Closed>())
                window.close();
            if (event -> is<sf::Event::MouseButtonPressed>()) {
                auto mousePos = sf::Mouse::getPosition(window);
                Boid boid({(float)mousePos.x, (float)mousePos.y}, {180.0f, 180.0f}, {0.0f, 0.0f});
                sim.addBoid(boid);
            }
        }

        window.clear(sf::Color::Black);

        sim.updateBoids(dt.asSeconds());
        sim.drawBoids(window);
        
        window.display();
        
    }
}