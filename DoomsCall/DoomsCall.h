#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <iostream>

class Settings;
class Assets;
class Item;
class Inventory;
class Object;
class DynamicObject;
class Player;
class Map;

namespace phy {
    struct Velocity {
        sf::Vector2f value;
        Velocity(float x, float y);
        void apply(sf::Vector2f& position, float delta);
    };
    struct Acceleration {
        sf::Vector2f value;
        Acceleration(float x, float y);
        void apply(Velocity& velocity, float delta);
    };
}



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
class Assets{
private:
    std::vector<sf::Texture> textures;
public:
    void loadTextures();
    std::vector<sf::Texture>& getTexture();
};

enum ItemType{FIREARM,SIDEARM,SPECIAL,THROWABLE,MISC};
class Item {
protected:
    std::string name;
public:
    virtual void whenHeld(Player& player) = 0;
    virtual void whenUsed(Player& player) = 0;
    virtual ~Item() = 0;
};
class Bandage :public Item {
    void whenHeld(Player& player);
    void whenUsed(Player& player);
};
class Inventory {
private:
    int selection;
    Item** inventory;
public:
    Inventory();
    void setSelection(int select);
    int getSelection();
    void addItem(Item* item);
    void removeItem();
    Item* getItem();
    ~Inventory();
};

class Object {
protected:
    sf::RectangleShape shape;
    
    bool solid;
    
public:
    Object(sf::Uint32 color,bool sol,const sf::Vector2f& position, const sf::Vector2f& size);
    void draw(sf::RenderWindow& window) const;
    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void setSize(const sf::Vector2f& size);
    sf::Vector2f getSize() const;
    sf::FloatRect getBounds() const;
    bool intersects(const Object& other) const;
    bool issolid();
    
};
class DynamicObject:public Object {
protected:
    DynamicObject(sf::Uint32 color, bool sol, const sf::Vector2f& position, const sf::Vector2f& size);
    phy::Velocity velocity;
    phy::Acceleration acceleration;
    bool grounded;
    bool hitceiling;
public:
    void simulateMovement(std::vector<Object>& objects, float deltatime);
};
class Player : public DynamicObject {
private:
    bool ishuman;
    int maxhealth;
    int health;
    float speed;
    Inventory inventory;
public:
    Player(sf::Uint32 color,bool human);
    void heal(int amount);
    void handleInput(std::vector<Object>& objects,float delta);
    void drawHUD(sf::RenderWindow& window, std::vector<sf::Texture>& textures,sf::Vector2f playerview);
};
class Map {
public:
    std::vector<Object> objects;
    void drawMap(sf::RenderWindow& window);
};

