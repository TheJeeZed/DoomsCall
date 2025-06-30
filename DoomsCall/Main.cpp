#include "Screens.h"

int main() {
    Settings s;
    Assets::loadTextures();
    sf::RenderWindow window(sf::VideoMode(800, 600), "DOOMSCALL");
    window.setFramerateLimit(Settings::getmaxFPS());
    sf::Clock clock;
    window.setIcon(32,32,s.geticon());
    ScreenStack::push_screen(new MainScreen);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || !ScreenStack::getsize())
                window.close();
        }
        float delta = clock.restart().asSeconds();
        window.clear(sf::Color::Black);
        ScreenStack::input(event, ScreenStack::getsize() - 1);
        ScreenStack::update(window,delta, ScreenStack::getsize() - 1);
        ScreenStack::render(window, ScreenStack::getsize() - 1);
        window.display();
        Settings::updateDelay();
        //std::cout << 1 / delta << "FPS" << std::endl;
    }
    return 0;
}