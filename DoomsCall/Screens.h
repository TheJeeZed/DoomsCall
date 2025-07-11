#pragma once

#include "Renderer.h"

class Screen {
public:
    virtual void input(sf::RenderWindow& window, sf::Event& event) = 0;
    virtual void update(float deltatime) = 0;
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
    void input(sf::RenderWindow& window, sf::Event& event);
    void update(float deltatime);
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
    void input(sf::RenderWindow& window, sf::Event& event);
    void update(float deltatime);
    void render(sf::RenderWindow& window);
    bool isWorkThrough();
    bool isUpdateThrough();
    bool isSeeThrough();
};
class SoundSettingsScreen :public Screen {
    SliderRender sliderrender;
    ButtonRender buttonrender;
    Slider master;
    Slider effect;
    Slider music;
    Slider ambiant;
    Button exit;
public:
    SoundSettingsScreen();
    void input(sf::RenderWindow& window, sf::Event& event);
    void update(float deltatime);
    void render(sf::RenderWindow& window);
    bool isWorkThrough();
    bool isUpdateThrough();
    bool isSeeThrough();
};
class MapScreen :public Screen {
    HUDRender hudrender;
    MapRender maprender;
    Player player;
    Map map;
    DropsPile drops;
public:
    MapScreen(int row, int col);
    void input(sf::RenderWindow& window, sf::Event& event);
    void update(float deltatime);
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
    void input(sf::RenderWindow& window, sf::Event& event);
    void update(float deltatime);
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
    static void input(sf::RenderWindow& window, sf::Event& event,int point);
    static void update(float deltatime, int point);
    static void render(sf::RenderWindow& window, int point);
};
class Game {
    Settings settings;
public:
    void run();
};

