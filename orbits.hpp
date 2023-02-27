
#pragma once


struct StarObject {
    sf::Vector2f position;
    sf::Vector2f gravity;
    float radius = 15.0f;
    sf::Color color = sf::Color::Red;

    StarObject() = default;
    StarObject(sf::Vector2f position_, sf::Vector2f gravity_, float radius_)
        : position {position_}
        , gravity {gravity_}
        , radius {radius_}
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

    StarObject& addStarObject(sf::Vector2f position, sf::Vector2f gravity, float radius) {
        return stars.emplace_back(position, gravity, radius);
    }

    MoonObject& addMoonObject(sf::Vector2f position, float radius) {
        return moons.emplace_back(position, radius);
    }



private:
    std::vector<StarObject> stars;
    std::vector<MoonObject> moons;

};
