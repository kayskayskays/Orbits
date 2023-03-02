
#pragma once
#include <cmath>


struct StarObject {
    sf::Vector2f position;
    float radius = 15.0f;
    float mass = 0.0f;
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

    void accelerate(sf::Vector2f acceleration_) {
        acceleration += acceleration_;
    }

    void setVelocity(sf::Vector2f v, float dt) {
        position_last = position - v * dt;
    }

    void addVelocity(sf::Vector2f v, float dt) {
        position_last -= v * dt;
    }

};

bool operator==(const MoonObject& rhs, const MoonObject& lhs) {
    return &rhs == &lhs;
}


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
            checkMoonCollisions();
            updateMoons(step_dt);
        }
    }

    void setMoonVelocity(MoonObject& moon, sf::Vector2f velocity) const {
        moon.setVelocity(velocity, getStepDt());
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

    int getIndex(const MoonObject& moon) {
        auto it = std::find(moons.begin(), moons.end(), moon);
        if (it != moons.end()) {
            return static_cast<int>(it - moons.begin());
        } else {
            return -1;
        }
    }

    void applyGravity() {

        sf::Vector2f gravity;

        for (auto& moon : moons) {
            gravity = findGravityVector(moon);
            moon.accelerate(gravity);
        }

    }

    float findGravitationalAcceleration(const MoonObject& moon, const StarObject& star) {
        sf::Vector2f displacement = star.position - moon.position;
        float distance = sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
        float minimum_distance = star.radius + moon.radius;
        if (distance < minimum_distance - 10) {
            int index = getIndex(moon);
            moons.erase(moons.begin() + index);
            return 0;
        }
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

    void checkMoonCollisions() {

        float response_coefficient = 0.75f;

        for (uint32_t i{0}; i < moons.size(); i++) {
            for (uint32_t j{i + 1}; j < moons.size(); j++) {
                auto& moon_1 = moons[i];
                auto& moon_2 = moons[j];
                sf::Vector2f v = moon_2.position - moon_1.position;
                float distance = sqrt(v.x * v.x + v.y * v.y);
                float min_distance = moon_1.radius + moon_2.radius;
                if (distance < min_distance) {
                    sf::Vector2f n = v / distance;
                    float delta = 0.5f * response_coefficient * (distance - min_distance);
                    float mass_ratio_1 = moon_1.radius / (moon_1.radius + moon_2.radius);
                    float mass_ratio_2 = moon_2.radius / (moon_1.radius + moon_2.radius);
                    moon_1.position += n * delta * mass_ratio_2;
                    moon_2.position -= n * delta * mass_ratio_1;
                }
            }
        }
    }

    void updateMoons(float dt) {

        for (auto& moon : moons) {
            moon.update(dt);
        }

    }

};
