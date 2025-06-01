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

enum AssetType {HUD};
void Assets::loadTextures() {
    sf::Texture image;
    if (!image.loadFromFile("resources/icons/HUD.png")) {
        std::cerr << "FAIL";
    }
    textures.push_back(image);
}
std::vector<sf::Texture>& Assets::getTexture() {
    return textures;
}

Item::~Item() {

}


void Bandage::whenHeld(Player& player) {
}
void Bandage::whenUsed(Player& player) {
    player.heal(1);
}

Inventory::Inventory() {
    inventory = new Item * [9];
    for (int i = 0; i < 9; i++) {
        inventory[i] = nullptr;
    }
    selection = 0;
}
void Inventory::setSelection(int select) {
    selection = select;
}
int Inventory::getSelection() {
    return selection;
}
void Inventory::addItem(Item* item) {
    if (inventory[selection] == nullptr) {
        inventory[selection] = item;
    }
}
void Inventory::removeItem() {
    delete inventory[selection];
    inventory[selection] = nullptr;
}
Item* Inventory::getItem() {
    return inventory[selection];
}

Inventory::~Inventory() {
    for (int i = 0; i < 9; i++) {
        delete[] inventory[i];
    }
    delete[] inventory;
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
    inventory.addItem(new Bandage());
    maxhealth = 200;
    health = 0;
    ishuman = human;
    speed = 500.f;
}
void Player::heal(int amount) {
    health += amount;
    if (health > maxhealth) {
        health = maxhealth;
    }
}
void Player::handleInput(std::vector<Object> & objects,float deltatime) {

    if (!ishuman) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))inventory.setSelection(0);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))inventory.setSelection(1);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))inventory.setSelection(2);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))inventory.setSelection(3);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))inventory.setSelection(4);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))inventory.setSelection(5);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))inventory.setSelection(6);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))inventory.setSelection(7);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))inventory.setSelection(8);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && inventory.getItem())inventory.getItem()->whenUsed(*this);

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
void Player::drawHUD(sf::RenderWindow& window, std::vector<sf::Texture>& textures,sf::Vector2f playerview) {
    sf::Sprite selected(textures[HUD], sf::IntRect(34,0,32,32));
    selected.setPosition(playerview + sf::Vector2f(-390, -290));
    sf::Sprite unselected(textures[HUD], sf::IntRect(64, 0, 32, 32));
    unselected.setPosition(playerview + sf::Vector2f(-390, -290));
    sf::Sprite filledbar(textures[HUD], sf::IntRect(0, 0, 32, 16));
    filledbar.setPosition(playerview + sf::Vector2f(-390, -250));
    sf::Sprite hpbar(textures[HUD], sf::IntRect(0, 16, 32, 16));
    hpbar.setPosition(playerview + sf::Vector2f(-390, -250));
    for (int i = 0; i < 9; i++) {
        if (i == inventory.getSelection()) {
            window.draw(selected);
        }
        else {
            window.draw(unselected);
        }
        selected.move(32, 0);
        unselected.move(32, 0);
    }
    hpbar.scale(6, 1);
    filledbar.scale(6 * (static_cast<float>(health) / maxhealth), 1);
    window.draw(hpbar);
    window.draw(filledbar);
}

void Map::drawMap(sf::RenderWindow& window) {
    for (auto& obj : objects) {
        obj.draw(window);
    }
}