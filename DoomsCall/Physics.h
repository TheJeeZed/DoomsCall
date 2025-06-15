#pragma once

#include <SFML/Graphics.hpp>

struct Velocity {
    sf::Vector2f value;
    Velocity();
    Velocity(float x, float y);
    void apply(sf::Vector2f& position, float delta);
};
struct Acceleration {
    sf::Vector2f value;
    Acceleration();
    Acceleration(float x, float y);
    void apply(Velocity& velocity, float delta);
};