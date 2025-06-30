#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>

enum TextureType {EMPTY,TITLE,BUTTONS,SLIDER, HUD, TILES, ITEMS ,PLAYER};
enum ButtonType {PLAY,OPTIONS,EXIT,DISPLAY,SOUND,CONTROLS};

class Settings {
    sf::Image icon;
    static int inputdelay;
    static int length;
    static int width;
    static int maxFPS;
public:
    Settings();
    static int getlength();
    static int getwidth();
    static int getmaxFPS();
    static int getDelay();
    static void setDelay(int val);
    static void updateDelay();
    const sf::Uint8* geticon();
};
class Assets {
private:
    static std::vector<sf::Texture> textures;
public:
    static void loadTextures();
    static sf::Texture& getTexture(TextureType type);
};
class Button {
private:
    sf::FloatRect location;
    ButtonType type;
    bool ishovered;
public:
    Button(float x, float y, float scale, ButtonType type);
    void update(const sf::Vector2i& mousePos);
    bool isClicked(const sf::Event& event) const;
    ButtonType getType();
    bool isHovered();
    sf::Vector2f getPosition();
    sf::Vector2f getScale();
};
class Slider {
    std::vector<sf::FloatRect> locations;
    std::vector<bool> ishovered;
    bool isdragging;
    int sections;
    int selected;
public:
    Slider(float x,float y,float scale,int sections,int selected);
    void update(const sf::Vector2i& mousePos, sf::Event& event);
    int getSections();
    int getSelected();
    sf::Vector2f getPosition(int index);
    sf::Vector2f getScale(int index);
};