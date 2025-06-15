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
void DynamicObject::simulateMovement(std::vector<std::vector<Tile*>>& map, float deltatime) {
    if (grounded || hitceiling) {
        velocity = Velocity(velocity.value.x, 0);
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
                if (futureBounds.intersects(sf::FloatRect(32 * i, 32 * j, 32, 32)) && map[i][j]) {
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
                if (futureBounds.intersects(sf::FloatRect(32 * i, 32 * j, 32, 32)) && map[i][j]) {
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
void Player::setCameraPosition() {
    camera.setCenter(this->getPosition());
}
void Player::focus(sf::RenderWindow& window) {
    window.setView(camera);
}
sf::View& Player::getCamera() {
    return camera;
}

EnterGameButton::EnterGameButton() {
    button[0] = sf::Sprite(Assets::getTexture(BUTTONS),sf::IntRect(0,0,32,32));
    button[1] = sf::Sprite(Assets::getTexture(BUTTONS), sf::IntRect(32, 0, 32, 32));
    button[2] = sf::Sprite(Assets::getTexture(BUTTONS), sf::IntRect(64, 0, 32, 32));
    button[0].setPosition(400, 300);
    button[1].setPosition(432, 300);
    button[2].setPosition(464, 300);
}
void EnterGameButton::whenClicked() {
    
}

void MainMenu::input() {

}
void MainMenu::update() {

}
void MainMenu::render(sf::RenderWindow& window) {
}
bool MainMenu::isSeeThrough() {
    return false;
}
bool MainMenu::isWorkThrough() {
    return false;
}

HUDRender::HUDRender() {
    selected = sf::Sprite(Assets::getTexture(AssetType::HUD), sf::IntRect(32, 0, 40, 40));
    unselected = sf::Sprite(Assets::getTexture(AssetType::HUD), sf::IntRect(72, 0, 40, 40));
    filledbar = sf::Sprite(Assets::getTexture(AssetType::HUD), sf::IntRect(0, 0, 32, 16));
    hpbar = sf::Sprite(Assets::getTexture(AssetType::HUD), sf::IntRect(0, 16, 32, 16));
    for (int i = 0; i < 2; i++)
    {
        items.push_back(sf::IntRect(32 * i, 0, 32, 32));
    }
    s.setTexture(Assets::getTexture(ITEMS));
    hpbar.scale(6, 1);
}
void HUDRender::draw(sf::RenderWindow& window, Player& player) {
    filledbar.setScale(1, 1);
    selected.setPosition(player.getCamera().getCenter() + sf::Vector2f(-390, -290));
    unselected.setPosition(player.getCamera().getCenter() + sf::Vector2f(-390, -290));
    filledbar.setPosition(player.getCamera().getCenter() + sf::Vector2f(-390, -240));
    hpbar.setPosition(player.getCamera().getCenter() + sf::Vector2f(-390, -240));
    for (int i = 0; i < 9; i++) {
        if (i == player.getInventory().getSelection()) {
            window.draw(selected);
        }
        else {
            window.draw(unselected);
        }
        if (player.getInventory().getItem(i)) {
            ItemType t = player.getInventory().getItem(i)->getType();
            s.setPosition(unselected.getPosition()+sf::Vector2f(4,4));
            s.setTextureRect(items[t]);
            window.draw(s);
        }
        selected.move(40, 0);
        unselected.move(40, 0);
    }
    filledbar.scale(6 * (static_cast<float>(player.getHP()) / player.getMaxHP()), 1);
    window.draw(hpbar);
    window.draw(filledbar);
}

GameRender::GameRender() {
    for (int i = 0; i < 2; i++)
    {
        tiles.push_back(sf::IntRect(32*i,0,32,32));
    }
    s.setTexture(Assets::getTexture(TILES));
}
void GameRender::draw(sf::RenderWindow& window, Player& player,Game& game) {
    sf::Vector2f center = player.getCamera().getCenter();
    sf::Vector2f size = player.getCamera().getSize();
    sf::FloatRect camera(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
    int left = std::max(std::floor(camera.left) / 32, 0.f);
    int top = std::max(std::floor(camera.top) / 32, 0.f);
    int right = std::min(std::ceil((camera.left + camera.width) / 32), static_cast<float>(game.getRow()));
    int bottom = std::min(std::ceil((camera.top + camera.height) / 32), static_cast<float>(game.getCol()));
    for (int i = top; i < bottom; i++) {
        for (int j = left; j < right; j++) {
            if (game.map[j][i]) {
                TileType t = game.map[j][i]->getType();
                s.setTextureRect(tiles[t]);
                s.setPosition(32 * j, 32 * i);
                window.draw(s);
            }
        }
    }
}

void Renderer::draw(sf::RenderWindow& window, Player& player, Game& game) {
    window.clear(sf::Color::Black);
    player.setCameraPosition();
    player.focus(window);
    gamerender.draw(window, player, game);
    hudrender.draw(window,player);
    player.draw(window);
    window.display();
}