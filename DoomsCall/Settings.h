#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>

enum AssetType { BUTTONS, HUD, TILES, ITEMS };

class Settings {
    sf::Image icon;
    int length;
    int width;
    int maxFPS;
public:
    Settings();
    int getlength() const;
    int getwidth() const;
    int getmaxFPS() const;

    const sf::Uint8* geticon() const;
};
class Assets {
private:
    static std::vector<sf::Texture> textures;
public:
    static void loadTextures();
    static sf::Texture& getTexture(AssetType type);
};