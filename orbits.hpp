
#pragma once
#include <cmath>


struct StarObject {
    sf::Vector2f position;
    float radius = 15.0f;
    float mass = 0.0f;
    float gravity = 0.0f;
    sf::Color color{255, 117, 24};

    StarObject() = default;
    StarObject(sf::Vector2f position_, float radius_, float mass_)
        : position {position_}
        , radius {radius_}
        , mass {mass_}
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

    StarObject& addStarObject(sf::Vector2f position, float radius, float mass) {
        return stars.emplace_back(position, radius, mass);
    }

    MoonObject& addMoonObject(sf::Vector2f position, float radius) {
        return moons.emplace_back(position, radius);
    }

    void update() {
        time += frame_dt;
        const float step_dt = getStepDt();
        for (uint32_t i{sub_steps}; i--;) {
            applyGravity();
            updateMoons(step_dt);
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

    [[nodiscard]]
    std::vector<StarObject> getStars() const {
        return stars;
    }

    [[nodiscard]]
    std::vector<MoonObject> getMoons() const {
        return moons;
    }

private:
    float time = 0.0f;
    float frame_dt = 0.0f;
    uint32_t sub_steps = 1;  // how many updates per frame
    std::vector<StarObject> stars;
    std::vector<MoonObject> moons;

    void applyGravity() {

        sf::Vector2f gravity;

        for (auto& moon : moons) {
            gravity = findGravityVector(moon);
            moon.accelerate(gravity);
        }

    }

    static float findGravitationalAcceleration(MoonObject& moon, StarObject& star) {
        sf::Vector2f displacement = star.position - moon.position;
        float distance = sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
        return static_cast<float>(star.mass / (distance * distance));
    }

    [[nodiscard]]
    sf::Vector2f findGravityVector(MoonObject& moon) {

        sf::Vector2f direction;
        sf::Vector2f displacement;
        float distance;
        float magnitude;

        for (auto& star : stars) {
            displacement = star.position - moon.position;
            distance = sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
            magnitude = findGravitationalAcceleration(moon, star);
            direction +=  displacement * magnitude / distance;
        }

        return direction;
    }

    void updateMoons(float dt) {

        for (auto& moon : moons) {
            moon.update(dt);
        }

    }

};
