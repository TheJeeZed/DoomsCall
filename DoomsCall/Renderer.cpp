#include "Renderer.h"

HUDRender::HUDRender() {
    selected = sf::Sprite(Settings::getTexture(HUD), sf::IntRect(32, 0, 40, 40));
    unselected = sf::Sprite(Settings::getTexture(HUD), sf::IntRect(72, 0, 40, 40));
    filledbar = sf::Sprite(Settings::getTexture(HUD), sf::IntRect(0, 0, 32, 16));
    hpbar = sf::Sprite(Settings::getTexture(HUD), sf::IntRect(0, 16, 32, 16));
    selected.setPosition(sf::Vector2f(10, 10));
    unselected.setPosition(sf::Vector2f(10, 10));
    filledbar.setPosition(sf::Vector2f(15,60));
    hpbar.setPosition(sf::Vector2f(15, 60));
    for (int i = 0; i < 2; i++)
    {
        items.push_back(sf::IntRect(32 * i, 0, 32, 32));
    }
    s.setTexture(Settings::getTexture(ITEMS));
    hpbar.scale(6, 1);
}
void HUDRender::draw(sf::RenderWindow& window, Player& player) {
    filledbar.setScale(1, 1);
    for (int i = 0; i < 9; i++) {
        if (i == player.getInventory().getSelection()) {
            window.draw(selected);
        }
        else {
            window.draw(unselected);
        }
        if (player.getInventory().getItem(i)) {
            ItemType t = player.getInventory().getItem(i)->getType();
            s.setPosition(unselected.getPosition() + sf::Vector2f(4, 4));
            s.setTextureRect(items[t]);
            window.draw(s);
        }
        selected.move(40, 0);
        unselected.move(40, 0);
    }
    filledbar.scale(6 * (static_cast<float>(player.getHP()) / player.getMaxHP()), 1);
    window.draw(hpbar);
    window.draw(filledbar);
    selected.setPosition(sf::Vector2f(10, 10));
    unselected.setPosition(sf::Vector2f(10, 10));
}
MapRender::MapRender() {
    for (int i = 0; i < 2; i++)
    {
        tiles.push_back(sf::IntRect(32 * i, 0, 32, 32));
    }
    s.setTexture(Settings::getTexture(TILES));
}
void MapRender::draw(sf::RenderWindow& window, Player& player, Map& map) {
    sf::Vector2f center = player.getCamera().getCenter();
    sf::Vector2f size = player.getCamera().getSize();
    sf::FloatRect camera(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
    int left = std::max(std::floor(camera.left) / 32, 0.f);
    int top = std::max(std::floor(camera.top) / 32, 0.f);
    int right = std::min(std::ceil((camera.left + camera.width) / 32), static_cast<float>(map.getRow()));
    int bottom = std::min(std::ceil((camera.top + camera.height) / 32), static_cast<float>(map.getCol()));
    for (int i = top; i < bottom; i++) {
        for (int j = left; j < right; j++) {
            if (map.map[j][i]) {
                TileType t = map.map[j][i]->getType();
                s.setTextureRect(tiles[t]);
                s.setPosition(32 * j, 32 * i);
                window.draw(s);
            }
        }
    }
}
ButtonRender::ButtonRender() {
    buttonsprite.setTexture(Settings::getTexture(BUTTONS));
}
void ButtonRender::draw(sf::RenderWindow& window,Button& button) {
    buttonsprite.setTextureRect(sf::IntRect(32 * button.getType(), 32 * button.isHovered(), 32, 32));
    buttonsprite.setPosition(button.getPosition());
    buttonsprite.setScale(button.getScale());
    window.draw(buttonsprite);
}
SliderRender::SliderRender() {
    slidersprite.setTexture(Settings::getTexture(SLIDER));
}
void SliderRender::draw(sf::RenderWindow& window, Slider& slider) {
    slidersprite.setScale(slider.getScale(0));
    for (int i = 0; i < slider.getSections(); i++) {
        slidersprite.setPosition(slider.getPosition(i));
        slidersprite.setTextureRect(sf::IntRect(16 * (i == slider.getSelected()),0 , 16, 16));
        window.draw(slidersprite);
    }
}
