#pragma once

#include <chrono>

#include "Tiles.h"
#include "Physics.h"
#include "Settings.h"

class Item;
class Inventory;
class Object;
class DynamicObject;
class Player;

enum ItemType { MEDKIT,BANDAGE};

class Item {
protected:
    int count;
public:
    virtual void whenHeld(Player& player) = 0;
    virtual void whenUsed(Player& player) = 0;
    virtual ItemType getType() = 0;
    ~Item();
};
class Medkit :public Item {
public:
    void whenHeld(Player& player);
    void whenUsed(Player& player);
    ItemType getType();
};
class Bandage :public Item {
public:
    void whenHeld(Player& player);
    void whenUsed(Player& player);
    ItemType getType();
};
class Inventory {
private:
    int selection;
    std::vector<Item*> inventory;
public:
    Inventory();
    void setSelection(int select);
    int getSelection();
    void addItem(Item* item);
    void removeItem();
    Item* getItem();
    Item* getItem(int select);
    ~Inventory();
};

class Game {
private:
    int row;
    int col;
public:
    std::vector<std::vector<Tile*>> map;
    Game(int row, int col);
    int getRow();
    int getCol();
};

class Object {
protected:
    sf::Sprite shape;
public:
    Object(const sf::Vector2f& position);
    void draw(sf::RenderWindow& window) const;
    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
};
class DynamicObject:public Object {
protected:
    Velocity velocity;
    Acceleration acceleration;
    bool grounded;
    bool hitceiling;
public:
    DynamicObject(const sf::Vector2f& position);
    void simulateMovement(Game& game, float deltatime);
};
class Player : public DynamicObject {
private:
    sf::View camera;
    int maxHP;
    int HP;
    float speed;
    Inventory inventory;
public:
    Player();
    void heal(int amount);
    int getHP();
    int getMaxHP();
    Inventory& getInventory();
    void handleInput();
    void setCameraPosition();
    void focus(sf::RenderWindow& window);
    sf::View& getCamera();
};

