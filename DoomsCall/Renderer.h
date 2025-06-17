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

class Screen {
public:
    virtual void input() = 0;
    virtual void update(float deltatime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual bool isSeeThrough() = 0;
    virtual bool isWorkThrough() = 0;
};
class GameScreen :public Screen {
    HUDRender hudrender;
    GameRender gamerender;
    Player player;
    Game game;
public:
    GameScreen(int row,int col);
    void input();
    void update(float deltatime);
    void render(sf::RenderWindow& window);
    bool isSeeThrough();
    bool isWorkThrough();
};
class ScreenStack {
    static std::vector<Screen*> screens;
public:
    void push_screen();
    void pop_screen();
    void input();
    void update();
    void render();
};