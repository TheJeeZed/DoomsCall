#include "Screens.h"

int ScreenStack::size = 0;
std::vector<Screen*> ScreenStack::screens;

GameScreen::GameScreen(int row, int col) :game(row, col) {
    player.setPosition({ 375.f * 0, -475.f});
}
void GameScreen::input(sf::Event& event) {
    static bool uWasPressed = false;
    bool uIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::U);
    if (uIsPressed && !uWasPressed) {
        ScreenStack::push_screen(new PauseScreen(player.getCamera()));
    }
    uWasPressed = uIsPressed;
    player.handleInput();
}
void GameScreen::update(float deltatime) {
    player.simulateMovement(game, deltatime);
}
void GameScreen::render(sf::RenderWindow& window) {
    player.setCameraPosition();
    player.focus(window);
    gamerender.draw(window, player, game);
    hudrender.draw(window, player);
    player.draw(window);
}
bool GameScreen::isWorkThrough() {
    return false;
}
bool GameScreen::isUpdateThrough() {
    return false;
}
bool GameScreen::isSeeThrough() {
    return false;
}

PauseScreen::PauseScreen(sf::View& camera):start(camera.getCenter().x, camera.getCenter().y,2,PLAY) {
    shade.setSize(sf::Vector2f(Settings::getlength(),Settings::getwidth()));
}
void PauseScreen::input(sf::Event& event) {
    static bool uWasPressed = false;
    bool uIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::U);
    if (uIsPressed && !uWasPressed) {
        ScreenStack::pop_screen();
    }
    uWasPressed = uIsPressed;
    if (start.isClicked(event)) {
        ScreenStack::pop_screen();
    }
}
void PauseScreen::update(float deltatime) {
}
void PauseScreen::render(sf::RenderWindow& window) {
    start.update(sf::Mouse::getPosition(window) - sf::Vector2i(400, 300) + sf::Vector2i(start.getPosition().x,start.getPosition().y));
    buttonrender.draw(window, start);
}
bool PauseScreen::isWorkThrough() {
    return false;
}
bool PauseScreen::isUpdateThrough() {
    return false;
}
bool PauseScreen::isSeeThrough() {
    return true;
}

int ScreenStack::getsize() {
    return size;
}
void ScreenStack::push_screen(Screen* screen) {
    screens.push_back(screen);
    size++;
}
void ScreenStack::pop_screen() {
    screens.pop_back();
    size--;
}
void ScreenStack::input(sf::Event& event,int point) {
    if (screens[point]->isWorkThrough()) {
        input(event,point - 1);
    }
    screens[point]->input(event);
}
void ScreenStack::update(float deltatime,int point) {
    if (screens[point]->isUpdateThrough()) {
        update(deltatime,point - 1);
    }
    screens[point]->update(deltatime);
}
void ScreenStack::render(sf::RenderWindow& window, int point) {
    if (screens[point]->isSeeThrough()) {
        render(window,point - 1);
    }
    screens[point]->render(window);
}
