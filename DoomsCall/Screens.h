#pragma once

#include "Renderer.h"

class Screen {
public:
    virtual void input(sf::Event& event) = 0;
    virtual void update(sf::RenderWindow& window, float deltatime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual bool isWorkThrough() = 0;
    virtual bool isUpdateThrough() = 0;
    virtual bool isSeeThrough() = 0;
};
class MainScreen :public Screen {
    ButtonRender buttonrender;
    sf::Sprite title;
    Button start;
    Button option;
    Button exit;
public:
    MainScreen();
    void input(sf::Event& event);
    void update(sf::RenderWindow& window,float deltatime);
    void render(sf::RenderWindow& window);
    bool isWorkThrough();
    bool isUpdateThrough();
    bool isSeeThrough();
};
class SettingsScreen:public Screen {
    ButtonRender buttonrender;
    Button display;
    Button sound;
    Button controls;
    Button exit;
public:
    SettingsScreen();
    void input(sf::Event& event);
    void update(sf::RenderWindow& window, float deltatime);
    void render(sf::RenderWindow& window);
    bool isWorkThrough();
    bool isUpdateThrough();
    bool isSeeThrough();
};
class GameScreen :public Screen {
    HUDRender hudrender;
    GameRender gamerender;
    Player player;
    Game game;
public:
    GameScreen(int row, int col);
    void input(sf::Event& event);
    void update(sf::RenderWindow& window, float deltatime);
    void render(sf::RenderWindow& window);
    bool isWorkThrough();
    bool isUpdateThrough();
    bool isSeeThrough();
};
class PauseScreen:public Screen {
    ButtonRender buttonrender;
    Button start;
    sf::RectangleShape shade;
public:
    PauseScreen();
    void input(sf::Event& event);
    void update(sf::RenderWindow& window, float deltatime);
    void render(sf::RenderWindow& window);
    bool isWorkThrough();
    bool isUpdateThrough();
    bool isSeeThrough();
};
class ScreenStack {
    static std::vector<Screen*> screens;
    static int size;
public:
    static int getsize();
    static void push_screen(Screen* screen);
    static void pop_screen();
    static void input(sf::Event& event,int point);
    static void update(sf::RenderWindow& window, float deltatime, int point);
    static void render(sf::RenderWindow& window, int point);
};
