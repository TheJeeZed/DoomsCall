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
class GameRender {
    std::vector<sf::IntRect> tiles;
    sf::Sprite s;
public:
    GameRender();
    void draw(sf::RenderWindow& window, Player& player, Game& game);
};
class ButtonRender {
    sf::Sprite s;
public:
    ButtonRender();
    void draw(sf::RenderWindow& window,Button& button);
};
