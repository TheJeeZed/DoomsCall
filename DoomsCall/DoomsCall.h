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
class Screen;
class ScreenStack;

enum ItemType { MEDKIT,BANDAGE};
enum ButtonType {ENTERGAME,OPTION};

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
    DynamicObject(sf::Uint32 color, const sf::Vector2f& position, const sf::Vector2f& size);
    Velocity velocity;
    Acceleration acceleration;
    bool grounded;
    bool hitceiling;
public:
    void simulateMovement(std::vector<std::vector<Tile*>>& map, float deltatime);
};
class Player : public DynamicObject {
private:
    int maxHP;
    int HP;
    float speed;
    Inventory inventory;
    sf::View camera;
public:
    Player(sf::Uint32 color);
    void heal(int amount);
    int getHP();
    int getMaxHP();
    Inventory& getInventory();
    void handleInput();
    void setCameraPosition();
    void focus(sf::RenderWindow& window);
    sf::View& getCamera();
};

class EnterGameButton{
    sf::Sprite button[3];
public:
    EnterGameButton();
    void whenClicked();
    void render();
};
class Screen {
public:
    virtual void input() = 0;
    virtual void update() = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual bool isSeeThrough() = 0;
    virtual bool isWorkThrough() = 0;
};
class MainMenu:public Screen {
    EnterGameButton enter;
public:
    void input();
    void update();   
    void render(sf::RenderWindow& window);
    bool isSeeThrough();
    bool isWorkThrough();
};
class ScreenStack {
    static std::vector<Screen*> screens;
public:
    void push_screen();
    void pop_screen();
    void input();
    void update();
    void render();
};

class HUDRender {
    sf::Sprite selected;
    sf::Sprite unselected;
    sf::Sprite filledbar;
    sf::Sprite hpbar;
    std::vector<sf::IntRect> items;
    sf::Sprite s;
public:
    HUDRender();
    void draw(sf::RenderWindow& window,Player& player);
};
class GameRender {
    std::vector<sf::IntRect> tiles;
    sf::Sprite s;
public:
    GameRender();
    void draw(sf::RenderWindow& window, Player& player,Game& game);
};
class Renderer {
    HUDRender hudrender;
    GameRender gamerender;
public:
    void draw(sf::RenderWindow& window, Player& player, Game& game);
};