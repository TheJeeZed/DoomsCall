#include "DoomsCall.h"

Acceleration gravity(0, 800.f);

Item::~Item() {

}

void Medkit::whenHeld(Player& player) {
}
void Medkit::whenUsed(Player& player) {
    player.heal(150);
}
ItemType Medkit::getType() {
    return MEDKIT;
}

void Bandage::whenHeld(Player& player) {
}
void Bandage::whenUsed(Player& player) {
    player.heal(25);
}
ItemType Bandage::getType() {
    return BANDAGE;
}

Inventory::Inventory() {
    inventory.resize(9);
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
Item* Inventory::getItem(int select) {
    return inventory[select];
}
Inventory::~Inventory() {
    for (int i = 0; i < 9; i++) {
        delete[] inventory[i];
    }
}

Game::Game(int row, int col) {
    this->row = row;
    this->col = col;
    map.resize(row);
    for (int i = 0; i < row; i++) {
        map[i].resize(col);
        for (int j = 0; j < col; j++) {
            if (i % 2)
                map[i][j] = new Grass;
            else
                map[i][j] = new Spike;
        }
    }
}
int Game::getRow() {
    return row;
}
int Game::getCol() {
    return col;
}

Object::Object(sf::Uint32 color = 0, const sf::Vector2f& position = { 0.f, 0.f }, const sf::Vector2f& size = { 50.f, 50.f}) {
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

DynamicObject::DynamicObject(sf::Uint32 color, const sf::Vector2f& position = { 0.f, 0.f }, const sf::Vector2f& size = { 50.f, 50.f }):
Object(color,position,size){
    grounded = false;
    hitceiling = false;
}
void DynamicObject::simulateMovement(Game& game, float deltatime) {
    if (grounded || hitceiling) {
        velocity = Velocity(velocity.value.x, 0);
    }
    else {
        gravity.apply(velocity, deltatime);
    }
    sf::Vector2f center = shape.getPosition();
    sf::FloatRect bounds(center.x - 256 / 2, center.y - 256 / 2, 256, 256);
    int left = std::max(std::floor(bounds.left) / 32, 0.f);
    int top = std::max(std::floor(bounds.top) / 32, 0.f);
    int right = std::min(std::ceil((bounds.left + bounds.width) / 32), static_cast<float>(game.getRow()));
    int bottom = std::min(std::ceil((bounds.top + bounds.height) / 32), static_cast<float>(game.getCol()));
    hitceiling = false;
    grounded = false;
    sf::Vector2f movement = velocity.value * deltatime;
    sf::FloatRect futureBounds = getBounds();
    if (movement.x != 0.f) {
        futureBounds.left += movement.x;
        for (int i = top; i < bottom; i++) {
            for (int j = left; j < right; j++) {
                if (futureBounds.intersects(sf::FloatRect(32 * j, 32 * i, 32, 32)) && game.map[j][i]) {
                    movement.x = 0.f;
                    break;
                }
                if (movement.x == 0.f)break;
            }
        }
        shape.move(movement.x, 0.f);
    }
    if (movement.y != 0.f) {
        futureBounds = getBounds(); 
        futureBounds.top += movement.y;
        for (int i = top; i < bottom; i++) {
            for (int j = left; j < right; j++) {
                if (futureBounds.intersects(sf::FloatRect(32 * j, 32 * i, 32, 32)) && game.map[j][i]) {
                    if (movement.y < 0.f) {
                        hitceiling = true;
                    }
                    else {
                        grounded = true;
                    }
                    movement.y = 0.f;
                    break;
                }
                if (movement.y == 0.f)break;
            }
        }
        shape.move(0.f, movement.y);
    }
}
void DynamicObject::setCameraPosition() {
    camera.setCenter(this->getPosition());
}
void DynamicObject::focus(sf::RenderWindow& window) {
    window.setView(camera);
}
sf::View& DynamicObject::getCamera() {
    return camera;
}

Player::Player(sf::Uint32 color):DynamicObject(color) {
    inventory.addItem(new Medkit());
    maxHP = 200;
    HP = 0;
    speed = 500.f;
    camera = sf::View(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
}
void Player::heal(int amount) {
    HP += amount;
    if (HP > maxHP) {
        HP = maxHP;
    }
}
int Player::getHP() {
    return HP;
}
int Player::getMaxHP() {
    return maxHP;
}
Inventory& Player::getInventory() {
    return inventory;
}
void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))inventory.setSelection(0);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))inventory.setSelection(1);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))inventory.setSelection(2);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))inventory.setSelection(3);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))inventory.setSelection(4);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))inventory.setSelection(5);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))inventory.setSelection(6);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))inventory.setSelection(7);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))inventory.setSelection(8);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && inventory.getItem()) {
        inventory.getItem()->whenUsed(*this);
        inventory.removeItem();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        velocity.value.x = -speed;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        velocity.value.x = speed;
    else
        velocity.value.x = 0.f; 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && grounded && !hitceiling) {
        velocity.value.y = -speed * 1;  
        grounded = false;
    }
}
