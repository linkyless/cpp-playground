#include <SFML/Graphics.hpp>
#include "mandelbrot.hpp"
#define MAXITER 200

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Mandelbrot Set");
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event -> is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);

        sf::Image image(sf::Vector2u(800, 600), sf::Color::Black);

        for (int x = 0; x < 800; x++) {
            for (int y = 0; y < 600; y++) {
                Complex c;
                c.real = pixelToReal(x);
                c.imag = pixelToImag(y);
                
                int iters = itersOfMandelbrot(c);
                if (iters == MAXITER) {
                    image.setPixel(sf::Vector2u(x, y), sf::Color::White);                    
                }
            }
        }

        sf::Texture texture(image);
        sf::Sprite sprite(texture);
        window.draw(sprite);

        window.display();
    }

    return 0;

}