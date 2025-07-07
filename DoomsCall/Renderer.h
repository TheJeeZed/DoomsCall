#pragma once

#include "DoomsCall.h"

class HUDRender {
    sf::Sprite selected;
    sf::Sprite unselected;
    sf::Sprite filledbar;
    sf::Sprite hpbar;
    std::vector<sf::IntRect> items;
    sf::Sprite s;
public:
    HUDRender();
    void draw(sf::RenderWindow& window, Player& player);
};
class MapRender {
    std::vector<sf::IntRect> tiles;
    sf::Sprite s;
public:
    MapRender();
    void draw(sf::RenderWindow& window, Player& player, Map& map);
};
class ButtonRender {
    sf::Sprite buttonsprite;
public:
    ButtonRender();
    void draw(sf::RenderWindow& window,Button& button);
};
class SliderRender {
    sf::Sprite slidersprite;
public:
    SliderRender();
    void draw(sf::RenderWindow& window, Slider& slider);
};