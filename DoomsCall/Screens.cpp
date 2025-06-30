#include "Screens.h"

int ScreenStack::size = 0;
std::vector<Screen*> ScreenStack::screens;

MainScreen::MainScreen():
    start(Settings::getlength() / 2-64, Settings::getwidth() / 2-32, 2, PLAY),
    option(Settings::getlength() / 2, Settings::getwidth() / 2-32, 2, OPTIONS),
    exit(Settings::getlength() / 2-32, Settings::getwidth() / 2+32, 2, EXIT) {
    title = sf::Sprite(Assets::getTexture(TITLE));
    title.setPosition(Settings::getlength() / 2 - 190,40);
}
void MainScreen::input(sf::RenderWindow& window, sf::Event& event) {
    start.update(sf::Mouse::getPosition(window));
    option.update(sf::Mouse::getPosition(window));
    exit.update(sf::Mouse::getPosition(window));
    if (start.isClicked(event)) {
        ScreenStack::push_screen(new GameScreen(2048,2048));
    }
    if (option.isClicked(event)) {
        ScreenStack::push_screen(new SettingsScreen);
    }
    if (exit.isClicked(event)) {
        ScreenStack::pop_screen();
    }
}
void MainScreen::update(float deltatime) {
}
void MainScreen::render(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.draw(title);
    buttonrender.draw(window, start);
    buttonrender.draw(window, option);
    buttonrender.draw(window, exit);
}
bool MainScreen::isWorkThrough() {
    return false;
}
bool MainScreen::isUpdateThrough() {
    return false;
}
bool MainScreen::isSeeThrough() {
    return false;
}

SettingsScreen::SettingsScreen() :
    display(Settings::getlength() / 2 - 96, Settings::getwidth() / 2 - 32, 2, DISPLAY),
    sound(Settings::getlength() / 2 - 32, Settings::getwidth() / 2 - 32, 2, SOUND),
    controls(Settings::getlength() / 2 + 32, Settings::getwidth() / 2-32, 2, CONTROLS),
    exit(Settings::getlength() / 2 - 32, Settings::getwidth() / 2 + 32, 2, EXIT) {
}
void SettingsScreen::input(sf::RenderWindow& window, sf::Event& event) {
    display.update(sf::Mouse::getPosition(window));
    sound.update(sf::Mouse::getPosition(window));
    controls.update(sf::Mouse::getPosition(window));
    exit.update(sf::Mouse::getPosition(window));
    if (sound.isClicked(event)) {
        ScreenStack::push_screen(new SoundSettingsScreen);
    }
    if (exit.isClicked(event)) {
        ScreenStack::pop_screen();
    }
}
void SettingsScreen::update(float deltatime) {
}
void SettingsScreen::render(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    buttonrender.draw(window, display);
    buttonrender.draw(window, sound);
    buttonrender.draw(window, controls);
    buttonrender.draw(window, exit);
}
bool SettingsScreen::isWorkThrough() {
    return false;
}
bool SettingsScreen::isUpdateThrough() {
    return false;
}
bool SettingsScreen::isSeeThrough() {
    return false;
}

SoundSettingsScreen::SoundSettingsScreen() :
    master(Settings::getlength() / 2 - 160, Settings::getwidth() / 2 - 144, 1, 20,19),
    effect(Settings::getlength() / 2 - 160, Settings::getwidth() / 2 - 80, 1, 20,19),
    music(Settings::getlength() / 2 - 160, Settings::getwidth() / 2 - 16, 1, 20,19),
    ambiant(Settings::getlength() / 2 - 160, Settings::getwidth() / 2 + 48, 1,20,19),
    exit(Settings::getlength() / 2 - 32, Settings::getwidth() - 64, 2, EXIT) {
}
void SoundSettingsScreen::input(sf::RenderWindow& window, sf::Event& event) {
    master.update(sf::Mouse::getPosition(window),event);
    effect.update(sf::Mouse::getPosition(window),event);
    music.update(sf::Mouse::getPosition(window),event);
    ambiant.update(sf::Mouse::getPosition(window),event);
    exit.update(sf::Mouse::getPosition(window));
    if (exit.isClicked(event)) {
        ScreenStack::pop_screen();
    }
}
void SoundSettingsScreen::update(float deltatime) {
}
void SoundSettingsScreen::render(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    sliderrender.draw(window, master);
    sliderrender.draw(window, effect);
    sliderrender.draw(window, music);
    sliderrender.draw(window, ambiant);
    buttonrender.draw(window, exit);
}
bool SoundSettingsScreen::isWorkThrough() {
    return false;
}
bool SoundSettingsScreen::isUpdateThrough() {
    return false;
}
bool SoundSettingsScreen::isSeeThrough() {
    return false;
}

GameScreen::GameScreen(int row, int col) :game(row, col) {
    player.setPosition({ 375.f * 0, -475.f});
}
void GameScreen::input(sf::RenderWindow& window, sf::Event& event) {
    static bool uWasPressed = false;
    bool uIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::U);
    if (uIsPressed && !uWasPressed) {
        ScreenStack::push_screen(new PauseScreen());
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
    player.draw(window);
    window.setView(window.getDefaultView());
    hudrender.draw(window, player);
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

PauseScreen::PauseScreen() :start(Settings::getlength() / 2, Settings::getwidth() / 2 , 2, PLAY) {
    shade.setSize(sf::Vector2f(Settings::getlength(),Settings::getwidth()));
}
void PauseScreen::input(sf::RenderWindow& window, sf::Event& event) {
    start.update(sf::Mouse::getPosition(window));
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
    window.setView(window.getDefaultView());
    start.update(sf::Mouse::getPosition(window));
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
    Settings::setDelay(Settings::getmaxFPS() / 6);
    screens.push_back(screen);
    size++;
}
void ScreenStack::pop_screen() {
    Settings::setDelay(Settings::getmaxFPS() / 6);
    screens.pop_back();
    size--;
}
void ScreenStack::input(sf::RenderWindow& window, sf::Event& event, int point) {
    if (!size||Settings::getDelay())return;
    if (screens[point]->isWorkThrough()) {
        input(window,event,point - 1);
    }
    screens[point]->input(window,event);
}
void ScreenStack::update(float deltatime,int point) {
    if (!size)return;
    if (screens[point]->isUpdateThrough()) {
        update(deltatime,point - 1);
    }
    screens[point]->update(deltatime);
}
void ScreenStack::render(sf::RenderWindow& window, int point) {
    if (!size)return;
    if (screens[point]->isSeeThrough()) {
        render(window,point - 1);
    }
    screens[point]->render(window);
}
