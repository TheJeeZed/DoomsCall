#include "Settings.h"

int Settings::length = 100;
int Settings::width = 100;
int Settings::maxFPS = 60;

Settings::Settings() {
    if (!icon.loadFromFile("DoomsCall.ico")) {
        std::cerr << "FAIL";
    }
    length = 0;
    width = 0;
    maxFPS = 120;
    std::ifstream fin("Option.txt");
    if (!fin) {
        std::cout << "Fail";
        return;
    }
    fin >> length;
    fin >> width;
    fin >> maxFPS;
    fin.close();
}
int Settings::getlength() {
    return length;
}
int Settings::getwidth() {
    return width;
}
int Settings::getmaxFPS() {
    return maxFPS;
}
const sf::Uint8* Settings::geticon() {
    return icon.getPixelsPtr();
}

std::vector<sf::Texture> Assets::textures;
void Assets::loadTextures() {
    sf::Texture image;
    if (!image.loadFromFile("resources/icons/Buttons.png")) {
        std::cerr << "FAIL";
    }
    textures.push_back(image);
    if (!image.loadFromFile("resources/icons/HUD.png")) {
        std::cerr << "FAIL";
    }
    textures.push_back(image);
    if (!image.loadFromFile("resources/Tiles.png")) {
        std::cerr << "FAIL";
    }
    textures.push_back(image);
    if (!image.loadFromFile("resources/Items.png")) {
        std::cerr << "FAIL";
    }
    textures.push_back(image);
    if (!image.loadFromFile("resources/entities/Player.png")) {
        std::cerr << "FAIL";
    }
    textures.push_back(image);
}
sf::Texture& Assets::getTexture(AssetType type) {
    return textures[static_cast<int>(type)];
}