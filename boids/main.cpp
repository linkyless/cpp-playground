#include <SFML/Graphics.hpp>
#include "Vec2.hpp"
#include "Boids.hpp"
#include "Simulation.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <random>
#include <vector>


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(-300, 300);

int main() {

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Boids Sim");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);

    sf::Clock clock;
    Simulation sim;
    sim.init();

    // Initial clear to dark background
    window.clear(sf::Color(10, 10, 20));
    window.display();

    static bool showPanel = 0;

    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        while (const std::optional event = window.pollEvent()) {

            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();

            // Click (Add 1 Boid)
            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto mousePos = sf::Mouse::getPosition(window);
                Boid boid({(float)mousePos.x, (float)mousePos.y},
                          {(float)dist(gen), (float)dist(gen)},
                          {0.0f, 0.0f});
                sim.addBoid(boid);
                sim.resizeForces();
            }

            // Enter (Add 10 Boids)
            if (auto key = event->getIf<sf::Event::KeyPressed>();
                key && key->code == sf::Keyboard::Key::Enter) {
                for (int i = 0; i < 10; i++) {
                    Boid boid({kSizeWidth / 2.0f, kSizeHeight / 2.0f},
                    {(float)dist(gen), (float)dist(gen)},
                    {0.0f, 0.0f});
                    sim.addBoid(boid);
                    sim.resizeForces();
                }
            }

            // Tab (Show imgui panel)
            if (auto key = event->getIf<sf::Event::KeyPressed>();
                key && key->code == sf::Keyboard::Key::Tab)
                showPanel = !showPanel;
        }

        ImGui::SFML::Update(window, dt);
        
        // ImGui panel
        if (showPanel) {
            ImGui::Begin("Panel");
            ImGui::SliderFloat("Separation", &wSeparation, 0.0f, 5.0f);
            ImGui::SliderFloat("Alignment",  &wAlignment,  0.0f, 5.0f);
            ImGui::SliderFloat("Cohesion",   &wCohesion,   0.0f, 5.0f);
            ImGui::Separator();
            ImGui::Text("Boids: %d", (int)sim.boids.size());
            ImGui::End();
        }

        sim.separation();
        sim.alignment();
        sim.cohesion();
        sim.updateBoids(dt.asSeconds());

        // no clear()
        sim.drawBoids(window);
        sim.resetAcceleration();

        ImGui::SFML::Render(window);
        
        window.display();
    }

    ImGui::SFML::Shutdown();
}