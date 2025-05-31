#include "DoomsCall.h"

phy::Acceleration gravity(0, 800.f);

phy::Velocity::Velocity(float x = 0, float y = 0) : value(x, y) {}
void phy::Velocity::apply(sf::Vector2f& position, float delta) {
    position += value * delta;
}

phy::Acceleration::Acceleration(float x = 0, float y = 0) : value(x, y) {}
void phy::Acceleration::apply(Velocity& velocity, float delta) {
    velocity.value += value * delta;
}

Settings::Settings() {
    if (!icon.loadFromFile("DoomsCall.ico")) {
        std::cout << "FAIL";
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
int Settings::getlength() const {
	return length;
}
int Settings::getwidth() const {
	return width;
}
int Settings::getmaxFPS() const {
    return maxFPS;
}
const sf::Uint8* Settings::geticon() const {
    return icon.getPixelsPtr();
}


Object::Object(sf::Uint32 color = 0, bool sol = false, const sf::Vector2f& position = { 0.f, 0.f }, const sf::Vector2f& size = { 50.f, 50.f}) {
    solid = sol;
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color::Color(color));
}
void Object::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}
void Object::setPosition(const sf::Vector2f& position) {
    shape.setPosition(position);
}
void Object::setPosition(float x, float y) {
    shape.setPosition(x, y);
}
sf::Vector2f Object::getPosition() const {
    return shape.getPosition();
}
void Object::setSize(const sf::Vector2f& size) {
    shape.setSize(size);
}
sf::Vector2f Object::getSize() const {
    return shape.getSize();
}
sf::FloatRect Object::getBounds() const {
    return shape.getGlobalBounds();
}
bool Object::intersects(const Object& other) const {
    return this->getBounds().intersects(other.getBounds());
}
bool Object::issolid() {
    return solid;
};

DynamicObject::DynamicObject(sf::Uint32 color, bool sol = false, const sf::Vector2f& position = { 0.f, 0.f }, const sf::Vector2f& size = { 50.f, 50.f }):
Object(color,sol,position,size){
    grounded = false;
    hitceiling = false;
}
void DynamicObject::simulateMovement(std::vector<Object>& objects, float deltatime) {
    if (grounded || hitceiling) {
        velocity = phy::Velocity(velocity.value.x, 0);
    }
    else {
        gravity.apply(velocity, deltatime);
    }
    hitceiling = false;
    grounded = false;
    sf::Vector2f movement = velocity.value * deltatime;
    sf::FloatRect futureBounds = getBounds();
    if (movement.x != 0.f) {
        futureBounds.left += movement.x;
        for (auto& obj : objects) {
            if (obj.issolid() && futureBounds.intersects(obj.getBounds())) {
                movement.x = 0.f;
                break;
            }
        }
        shape.move(movement.x, 0.f);
    }
    if (movement.y != 0.f) {
        futureBounds = getBounds(); 
        futureBounds.top += movement.y;
        for (auto& obj : objects) {
            if (obj.issolid() && futureBounds.intersects(obj.getBounds())) {
                /*if (futureBounds.top + futureBounds.height > obj.getBounds().top) {
                    grounded = true;
                }*/
                if (movement.y < 0.f) {
                    hitceiling = true;
                }
                else {
                    grounded = true;
                }
                movement.y = 0.f;
                break;
            }
        }
        shape.move(0.f, movement.y);
    }
}

Player::Player(sf::Uint32 color,bool human = true):DynamicObject(color) {
    ishuman = human;
    speed = 500.f;
}
void Player::handleInput(std::vector<Object> & objects,float deltatime) {

    if (!ishuman) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        velocity.value.x = -speed;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        velocity.value.x = speed;
    else
        velocity.value.x = 0.f; 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && grounded && !hitceiling) {
        velocity.value.y = -speed * 1;  
        grounded = false;
    }
}

void Map::drawMap(sf::RenderWindow& window) {
    for (auto& obj : objects) {
        obj.draw(window);
    }
}