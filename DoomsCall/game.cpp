//#include <SFML/Graphics.hpp>
//#include <vector>
//
//int main() {
//    // Create window
//    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Painting App");
//    window.setFramerateLimit(60);
//
//    // Create canvas (white rectangle)
//    sf::RectangleShape canvas(sf::Vector2f(800.0f, 600.0f));
//    canvas.setFillColor(sf::Color::White);
//    canvas.setPosition(0.0f, 0.0f);
//
//    // Create button for color change (50x50 square in top-left)
//    sf::RectangleShape colorButton(sf::Vector2f(50.0f, 50.0f));
//    colorButton.setPosition(10.0f, 10.0f);
//    colorButton.setFillColor(sf::Color::Red); // Initial color
//
//    // Brush properties
//    std::vector<sf::CircleShape> brushStrokes;
//    std::vector<sf::Color> colors = { sf::Color::Red, sf::Color::Green, sf::Color::Blue };
//    int currentColorIndex = 0;
//    const float brushSize = 5.0f;
//
//    // Button interaction state
//    bool buttonClicked = false;
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//
//            // Handle button click to change color
//            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
//                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
//                if (colorButton.getGlobalBounds().contains(mousePos)) {
//                    buttonClicked = true;
//                    currentColorIndex = (currentColorIndex + 1) % colors.size();
//                    colorButton.setFillColor(colors[currentColorIndex]);
//                }
//            }
//            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
//                buttonClicked = false;
//            }
//        }
//
//        // Drawing mechanics
//        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !buttonClicked) {
//            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
//            // Only draw if within canvas bounds
//            if (mousePos.x >= 0 && mousePos.x <= 800 && mousePos.y >= 0 && mousePos.y <= 600) {
//                sf::CircleShape stroke(brushSize);
//                stroke.setFillColor(colors[currentColorIndex]);
//                stroke.setPosition(mousePos.x - brushSize, mousePos.y - brushSize);
//                brushStrokes.push_back(stroke);
//            }
//        }
//
//        // Render
//        window.clear(sf::Color::Black);
//        window.draw(canvas);
//        for (const auto& stroke : brushStrokes) {
//            window.draw(stroke);
//        }
//        window.draw(colorButton);
//        window.display();
//    }
//
//    return 0;
//}