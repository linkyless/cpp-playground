#include <SFML/Graphics.hpp>
#include "Vec2.hpp"

int main() {

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Boids Sim");
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event -> is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::Black);
        window.display();
    }
}