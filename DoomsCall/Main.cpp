#include "Renderer.h"

int main() {
    Settings s;
    Assets::loadTextures();
    sf::RenderWindow window(sf::VideoMode(800, 600), "DOOMSCALL");
    window.setFramerateLimit(s.getmaxFPS());
    sf::Clock clock;
    window.setIcon(32,32,s.geticon());
    GameScreen screen(2048,2048);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float delta = clock.restart().asSeconds();
        screen.input();
        screen.update(delta);
        screen.render(window);
        std::cout << 1 / delta << "FPS" << std::endl;
    }
    return 0;
}