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

std::vector<sf::Texture> Assets::textures;
void Assets::loadTextures() {
    sf::Texture image;
    if (!image.loadFromFile("resources/icons/HUD.png")) {
        std::cerr << "FAIL";
    }
    textures.push_back(image);
    if (!image.loadFromFile("resources/Tiles.png")) {
        std::cerr << "FAIL";
    }
    textures.push_back(image);
}
sf::Texture& Assets::getTexture(AssetType type) {
    return textures[type];
}

Item::~Item() {

}

void Bandage::whenHeld(Player& player) {
}
void Bandage::whenUsed(Player& player) {
    player.heal(25);
}

void Medkit::whenHeld(Player& player) {
}
void Medkit::whenUsed(Player& player) {
    player.heal(150);
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

Tile::Tile(int x, int y, TileType type) {
    this->type = type;
    tile = sf::Sprite(Assets::getTexture(TILES), sf::IntRect(32 * type, 0, 32, 32));
    tile.setPosition(32.f * x, 32.f * y);
}
void Tile::draw(sf::RenderWindow& window) const {
    window.draw(tile);
}
sf::Vector2f Tile::getPosition() const {
    return tile.getPosition();
}
sf::FloatRect Tile::getBounds() const {
    return tile.getGlobalBounds();
}
bool Tile::intersects(const Object& other) const {
    return this->getBounds().intersects(other.getBounds());
}

Game::Game(int row, int col) {
    this->row = row;
    this->col = col;
    map.resize(row);
    for (int i = 0; i < row; i++) {
        map[i].resize(col);
        for (int j = 0; j < col; j++) {
            map[i][j] = new Tile(i, j, GRASS);
        }
    }
}
void Game::draw(sf::RenderWindow& window, sf::View& playerview) {
    sf::Vector2f center = playerview.getCenter();
    sf::Vector2f size = playerview.getSize();
    sf::FloatRect camera(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
    int left = std::max(std::floor(camera.left) / 32, 0.f);
    int top = std::max(std::floor(camera.top) / 32,0.f);
    int right = std::min(std::ceil((camera.left + camera.width)/32), static_cast<float>(row));
    int bottom = std::min(std::ceil((camera.top + camera.height)/32), static_cast<float>(col));
    for (int i = left; i < right; i++) {
        for (int j = top; j < bottom; j++) {
            map[i][j]->draw(window);
        }
    }
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
void DynamicObject::simulateMovement(std::vector<std::vector<Tile*>> map, float deltatime) {
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
        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[0].size(); j++) {
                if (futureBounds.intersects(map[i][j] -> getBounds())) {
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
        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[0].size(); j++) {
                if (futureBounds.intersects(map[i][j]->getBounds())) {
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

Player::Player(sf::Uint32 color,bool human = true):DynamicObject(color) {
    inventory.addItem(new Medkit());
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
void Player::handleInput() {

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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && grounded && !hitceiling) {
        velocity.value.y = -speed * 1;  
        grounded = false;
    }
}
void Player::drawHUD(sf::RenderWindow& window,sf::Vector2f playerview) {
    sf::Sprite selected(Assets::getTexture(HUD), sf::IntRect(34,0,32,32));
    selected.setPosition(playerview + sf::Vector2f(-390, -290));
    sf::Sprite unselected(Assets::getTexture(HUD), sf::IntRect(64, 0, 32, 32));
    unselected.setPosition(playerview + sf::Vector2f(-390, -290));
    sf::Sprite filledbar(Assets::getTexture(HUD), sf::IntRect(0, 0, 32, 16));
    filledbar.setPosition(playerview + sf::Vector2f(-390, -250));
    sf::Sprite hpbar(Assets::getTexture(HUD), sf::IntRect(0, 16, 32, 16));
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

