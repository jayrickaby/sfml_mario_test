#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main(){
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");

    // Logic
    sf::Clock clock;
    float deltaTime;

    // Player
    sf::RectangleShape rect({16,16});
    rect.setFillColor(sf::Color::Red);
    signed int direction{0};
    float speed{500.f};
    sf::Vector2f position{0.f,0.f};

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }
        deltaTime = clock.restart().asSeconds();

        window.clear();

        direction = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) - (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A));
        std::printf("%i\n", position.x);

        position.x += direction * speed;
        rect.setPosition(position * deltaTime);

        window.draw(rect);
        window.display();
    }
    return 0;
}
