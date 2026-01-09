#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "TextureManager.h"

int main(){
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");

    TextureManager::initialiseTextures("assets/textures/");
    sf::Sprite sprite(TextureManager::loadTexture("assets/textures/mariosheet.png"));

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}
