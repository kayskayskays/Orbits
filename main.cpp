
#include <SFML/graphics.hpp>
#include "orbits.hpp"

int main() {

    const uint32_t window_width = 1000;
    const uint32_t window_height = 1000;
    const uint32_t frame_rate = 60;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Orbit", sf::Style::Default, settings);
    window.setFramerateLimit(frame_rate);

    while (window.isOpen()) {

        sf::Event event{};

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

        }

        window.clear(sf::Color::Black);
        window.display();

    }

    return 0;

}
