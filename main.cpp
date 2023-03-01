
#include <SFML/graphics.hpp>
#include "orbits.hpp"
#include "renderer.hpp"

int main() {

    const uint32_t window_width = 1000;
    const uint32_t window_height = 1000;
    const uint32_t frame_rate = 60;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Orbit", sf::Style::Default, settings);
    window.setFramerateLimit(frame_rate);

    Renderer renderer{window};
    Solver solver;
    solver.setSimulationUpdateRate(frame_rate);

    solver.addMoonObject({200.0f, 300.0f}, 10.0f);
    solver.addMoonObject({400.0f, 300.0f}, 10.0f);
    solver.addStarObject({500.0f, 200.0f}, 15.0f, 10000000);
    solver.addStarObject({100.0f, 100.0f}, 15.0f, 10000000);

    while (window.isOpen()) {

        sf::Event event{};

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

        }

        solver.update();
        window.clear(sf::Color::Black);
        renderer.render(solver);
        window.display();

    }

    return 0;

}
