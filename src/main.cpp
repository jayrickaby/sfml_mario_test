#include <cstdio>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main(){
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }
        window.clear();
        window.display();
    }
    return 0;
}
