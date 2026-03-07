#include <SFML/Graphics.hpp>
#include "mandelbrot.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({kWidth, kHeight}), "Mandelbrot Set");
    
    double cx = -0.5; // R-Center
    double cy = 0.0; // Z-Center
    double width = 3.5; 
    double height = 3.0;

    bool needsRedraw = true;

    sf::Texture texture(sf::Vector2u(kWidth, kHeight));
    
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event -> is<sf::Event::Closed>())
                window.close();
            if (event -> is<sf::Event::MouseWheelScrolled>()) {
                auto mousePos = sf::Mouse::getPosition(window);
                
                // New Center
                cx = pixelToReal(cx, mousePos.x, width);
                cy = pixelToImag(cy, mousePos.y, height);
                
                auto scroll = event -> getIf<sf::Event::MouseWheelScrolled>();
                if (scroll -> delta > 0) {
                    // 10% zoom in
                    width *= kZoomIn;
                    height *= kZoomIn;
                }
                else {
                    // 10% zoom out
                    width *= kZoomOut;
                    height *= kZoomOut;
                }
                needsRedraw = true;
            }
        }

        if (needsRedraw) {
            sf::Image image(sf::Vector2u(kWidth, kHeight), sf::Color::Black);
            int maxIters = getMaxIters(width);

            for (int x = 0; x < kWidth; x++) {
                for (int y = 0; y < kHeight; y++) {
                    Complex c;
                    c.real = pixelToReal(cx, x, width);
                    c.imag = pixelToImag(cy, y, height);
                    int iters = itersOfMandelbrot(c, width);
                    int numcolor = (int)((double)iters / maxIters * 255); // color based on iteration count
                    sf::Color color = sf::Color(0, 0, numcolor, 255);
                    image.setPixel(sf::Vector2u(x, y), color);
                }
            }
            
            texture.update(image);
            needsRedraw = false;
        }

        window.clear(sf::Color::Black);
        sf::Sprite sprite(texture);
        window.draw(sprite);
        window.display();
    }

    return 0;

}