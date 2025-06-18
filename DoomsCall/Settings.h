#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>

enum AssetType { BUTTONS, HUD, TILES, ITEMS ,PLAYER};

class Settings {
    sf::Image icon;
    static int length;
    static int width;
    static int maxFPS;
public:
    Settings();
    static int getlength();
    static int getwidth();
    static int getmaxFPS();
    const sf::Uint8* geticon();
};
class Assets {
private:
    static std::vector<sf::Texture> textures;
public:
    static void loadTextures();
    static sf::Texture& getTexture(AssetType type);
};