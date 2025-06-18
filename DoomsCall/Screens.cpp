#include "Screens.h"

int ScreenStack::size = 0;
std::vector<Screen*> ScreenStack::screens;

GameScreen::GameScreen(int row, int col) :game(row, col) {
    player.setPosition({ 375.f, -375.f });
}
void GameScreen::input(sf::Event& event) {
    static bool uWasPressed = false;
    bool uIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::U);
    if (uIsPressed && !uWasPressed) {
        ScreenStack::push_screen(new PauseScreen);
    }
    uWasPressed = uIsPressed;
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) {
        ScreenStack::push_screen(new PauseScreen);
    }
    player.handleInput();
}
void GameScreen::update(float deltatime) {
    player.simulateMovement(game, deltatime);
}
void GameScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    player.setCameraPosition();
    player.focus(window);
    gamerender.draw(window, player, game);
    hudrender.draw(window, player);
    player.draw(window);
    window.display();
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

PauseScreen::PauseScreen() {
    shade.setSize(sf::Vector2f(Settings::getlength(),Settings::getwidth()));
}
void PauseScreen::input(sf::Event& event) {
    static bool uWasPressed = false;
    bool uIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::U);
    if (uIsPressed && !uWasPressed) {
        ScreenStack::pop_screen();
    }
    uWasPressed = uIsPressed;
}
void PauseScreen::update(float deltatime) {
}
void PauseScreen::render(sf::RenderWindow& window) {
    window.draw(shade);
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
