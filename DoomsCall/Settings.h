#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>

enum TextureType {EMPTY,TITLE,BUTTONS, HUD, TILES, ITEMS ,PLAYER};
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