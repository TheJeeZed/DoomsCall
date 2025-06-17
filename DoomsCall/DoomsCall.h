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
    virtual ~Item() = 0;
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
    sf::RectangleShape shape;
public:
    Object(sf::Uint32 color,const sf::Vector2f& position, const sf::Vector2f& size);
    void draw(sf::RenderWindow& window) const;
    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void setSize(const sf::Vector2f& size);
    sf::Vector2f getSize() const;
    sf::FloatRect getBounds() const;
};
class DynamicObject:public Object {
protected:
    Velocity velocity;
    Acceleration acceleration;
    bool grounded;
    bool hitceiling;
    sf::View camera;
public:
    DynamicObject(sf::Uint32 color, const sf::Vector2f& position, const sf::Vector2f& size);
    void simulateMovement(Game& game, float deltatime);
    void setCameraPosition();
    void focus(sf::RenderWindow& window);
    sf::View& getCamera();
};
class Player : public DynamicObject {
private:
    int maxHP;
    int HP;
    float speed;
    Inventory inventory;
public:
    Player(sf::Uint32 color);
    void heal(int amount);
    int getHP();
    int getMaxHP();
    Inventory& getInventory();
    void handleInput();
};

