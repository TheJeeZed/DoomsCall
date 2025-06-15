#include "Physics.h"


Velocity::Velocity():value(0, 0){}
Velocity::Velocity(float x, float y) : value(x, y) {}
void Velocity::apply(sf::Vector2f& position, float delta) {
    position += value * delta;
}

Acceleration::Acceleration():value(0, 0){}
Acceleration::Acceleration(float x, float y) : value(x, y) {}
void Acceleration::apply(Velocity& velocity, float delta) {
    velocity.value += value * delta;
}
