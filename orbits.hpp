
#pragma once
#include <cmath>


struct StarObject {
    sf::Vector2f position;
    float radius = 15.0f;
    float mass = 0.0f;
    float gravity = 0.0f;
    sf::Color color = sf::Color::Red;

    StarObject() = default;
    StarObject(sf::Vector2f position_, float radius_, float mass_, float gravity_)
        : position {position_}
        , radius {radius_}
        , mass {mass_}
        , gravity {gravity_}
    {}
};


struct MoonObject {

    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    float radius = 10.0f;
    sf::Color color = sf::Color::White;

    MoonObject() = default;
    MoonObject(sf::Vector2f position_, float radius_)
        : position {position_}
        , position_last {position_}
        , acceleration {0.0f, 0.0f}
        , radius {radius_}
    {}

    void update(float dt) {
        sf::Vector2f displacement = position - position_last;
        position_last = position;
        position = position + displacement + acceleration * dt * dt;
        acceleration = {};
    }

    void accelerate(sf::Vector2f a) {
        acceleration += a;
    }

};

class Solver {

public:
    Solver() = default;

    StarObject& addStarObject(sf::Vector2f position, float radius, float gravity, float mass) {
        return stars.emplace_back(position, radius, mass, gravity);
    }

    MoonObject& addMoonObject(sf::Vector2f position, float radius) {
        return moons.emplace_back(position, radius);
    }

    void update() {
        time += frame_dt;
        const float step_dt = getStepDt();
        for (uint32_t i{sub_steps}; i--;) {

        }
    }

    void setSimulationUpdateRate(uint32_t rate) {
        frame_dt = 1.0f  / static_cast<float>(rate);
    }

    void setSubStepsCount(uint32_t sub_steps_) {
        sub_steps = sub_steps_;
    }

    [[nodiscard]]
    float getStepDt() const {
        return frame_dt / static_cast<float>(sub_steps);
    }

private:
    float time = 0.0f;
    float frame_dt = 0.0f;
    uint32_t sub_steps = 1;  // how many updates per frame
    std::vector<StarObject> stars;
    std::vector<MoonObject> moons;


    static float findGravitationalAcceleration(MoonObject& moon, StarObject& star) {
        const double newton_G = 6.67384e-11;
        sf::Vector2f displacement = moon.position - star.position;
        float distance = sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
        return static_cast<float>(newton_G * star.mass / (distance * distance));
    }

    sf::Vector2f findGravityVector(MoonObject& moon) {

        sf::Vector2f direction;
        sf::Vector2f displacement;
        float distance;
        float magnitude;

        for (auto& star : stars) {
            displacement = moon.position - star.position;
            distance = sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
            magnitude = findGravitationalAcceleration(moon, star);
            direction +=  displacement * magnitude / distance;
        }

        return direction;

    }

};
