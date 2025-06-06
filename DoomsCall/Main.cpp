#include "DoomsCall.h"

int main() {
    Settings s;
    Assets::loadTextures();
    HUD hud;
    sf::RenderWindow window(sf::VideoMode(800, 600), "DOOMSCALL");
    window.setFramerateLimit(s.getmaxFPS());
    sf::Clock clock;
    window.setIcon(32,32,s.geticon());

    Player player(0xFFFFFFFF, true); 
    player.setSize({ 50.f, 50.f });
    player.setPosition({ 375.f, -275.f }); 
    
    Game game(1024,1024);
    player.focus(window);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float delta = clock.restart().asSeconds();
        player.handleInput();
        player.simulateMovement(game.map, delta);

        std::cout << static_cast<int>(1 / delta) << "FPS \n";
        player.setCameraPosition();
        player.focus(window);
        window.clear(sf::Color::Black);
        game.draw(window,player.getCamera());
        hud.draw(window,player);
        player.draw(window);
        window.display();
    }
    return 0;
}