
#pragma once
#include "orbits.hpp"

class Renderer {

public:
    explicit
    Renderer(sf::RenderTarget& target_)
        : target {target_}
    {}

    void render(const Solver& solver) {

        const auto& stars = solver.getStars();
        const auto& moons = solver.getMoons();

        sf::CircleShape circle{1.0f};
        circle.setPointCount(32);
        circle.setOrigin(1.0f, 1.0f);

        for (const auto& star : stars) {
            circle.setPosition(star.position);
            circle.setScale(star.radius, star.radius);
            circle.setFillColor(star.color);
            target.draw(circle);
        }

        for (const auto& moon : moons) {
            circle.setPosition(moon.position);
            circle.setScale(moon.radius, moon.radius);
            circle.setFillColor(moon.color);
            target.draw(circle);
        }

    }

private:
    sf::RenderTarget& target;

};