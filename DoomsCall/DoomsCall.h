#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <iostream>


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
    float speed;
public:
    Player(sf::Uint32 color,bool human);
    void handleInput(std::vector<Object>& objects,float delta);
};
class Map {
public:
    std::vector<Object> objects;
    void drawMap(sf::RenderWindow& window);
};

