#include "DoomsCall.h"

int main() {
    Settings s;
    Assets assets;
    assets.loadTextures();
    sf::RenderWindow window(sf::VideoMode(800, 600), "DOOMSCALL");
    window.setFramerateLimit(s.getmaxFPS());
    sf::Clock clock;
    window.setIcon(32,32,s.geticon());

    Player player(0xFFFFFFFF, true); 
    player.setSize({ 50.f, 50.f });
    player.setPosition({ 375.f, 275.f }); 

    Map map;
    sf::View camera(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
    window.setView(camera);

    map.objects.emplace_back(0xFF888888, true, sf::Vector2f(0.f, 580.f), sf::Vector2f(2000.f, 20.f));  
    map.objects.emplace_back(0xFF888888, true, sf::Vector2f(0.f, 0.f), sf::Vector2f(2000.f, 20.f));    
    map.objects.emplace_back(0xFF888888, true, sf::Vector2f(0.f, 0.f), sf::Vector2f(20.f, 600.f));     
    map.objects.emplace_back(0xFF888888, true, sf::Vector2f(1980.f, 0.f), sf::Vector2f(20.f, 600.f));  

    map.objects.emplace_back(0xFFFF00FF, true, sf::Vector2f(100.f, 500.f), sf::Vector2f(100.f, 20.f));
    map.objects.emplace_back(0xFF00FFFF, true, sf::Vector2f(250.f, 450.f), sf::Vector2f(100.f, 20.f));
    map.objects.emplace_back(0xFF0000FF, true, sf::Vector2f(400.f, 400.f), sf::Vector2f(100.f, 20.f));
    map.objects.emplace_back(0xFFFFFFFF, true, sf::Vector2f(600.f, 350.f), sf::Vector2f(150.f, 20.f));
    map.objects.emplace_back(0xFFFF00FF, true, sf::Vector2f(850.f, 300.f), sf::Vector2f(100.f, 20.f));
    map.objects.emplace_back(0xFF00FFFF, true, sf::Vector2f(1050.f, 250.f), sf::Vector2f(150.f, 20.f));
    map.objects.emplace_back(0xFFFF99FF, true, sf::Vector2f(1300.f, 350.f), sf::Vector2f(100.f, 20.f));
    map.objects.emplace_back(0xFF99FF99, true, sf::Vector2f(1450.f, 300.f), sf::Vector2f(100.f, 20.f));
    map.objects.emplace_back(0xFF9999FF, true, sf::Vector2f(1600.f, 250.f), sf::Vector2f(100.f, 20.f));
    map.objects.emplace_back(0xFF444444, true, sf::Vector2f(1800.f, 200.f), sf::Vector2f(150.f, 20.f));



    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float delta = clock.restart().asSeconds();
        player.handleInput(map.objects,delta);
        player.simulateMovement(map.objects, delta);

        window.clear(sf::Color::Black);
        map.drawMap(window);
        camera.setCenter(player.getPosition());
        window.setView(camera);
        player.drawHUD(window, assets.getTexture(), camera.getCenter());
        player.draw(window);
        window.display();
    }
    return 0;
}