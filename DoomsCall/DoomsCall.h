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

enum class TileType { GRASS };
enum class AssetType { HUD,TILES};
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
    static std::vector<sf::Texture> textures;
public:
    static void loadTextures();
    static sf::Texture& getTexture(AssetType type);
};

class Item {
protected:
    int count;
public:
    virtual void whenHeld(Player& player) = 0;
    virtual void whenUsed(Player& player) = 0;
    virtual ~Item() = 0;
};
class Bandage :public Item {
public:
    void whenHeld(Player& player);
    void whenUsed(Player& player);
};
class Medkit :public Item {
public:
    void whenHeld(Player & player);
    void whenUsed(Player & player);
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


class Tile {
    sf::Sprite tile;
    TileType type;
public:
    Tile(int x, int y, TileType type);
    void draw(sf::RenderWindow& window) const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    bool intersects(const Object& other) const;
};
class Game {
private:
    int row;
    int col;
public:
    std::vector<std::vector<Tile*>> map;
    Game(int row, int col);
    void draw(sf::RenderWindow& window,sf::View& playerview);
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
    void simulateMovement(std::vector<std::vector<Tile*>> map, float deltatime);
};
class Player : public DynamicObject {
private:
    bool ishuman;
    int maxHP;
    int HP;
    float speed;
    Inventory inventory;
    sf::View camera;
public:
    Player(sf::Uint32 color,bool human);
    void heal(int amount);
    int getHP();
    int getMaxHP();
    Inventory& getInventory();
    void handleInput();
    void setCameraPosition();
    void focus(sf::RenderWindow& window);
    sf::View& getCamera();
    void drawHUD(sf::RenderWindow& window,sf::Vector2f playerview);
};

class HUD {
    sf::Sprite selected;
    sf::Sprite unselected;
    sf::Sprite filledbar;
    sf::Sprite hpbar;
public:
    HUD();
    void draw(sf::RenderWindow& window,Player& player);
};

