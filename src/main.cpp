#include <math.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "constants.h"
#include "Player.h"
#include "TextureManager.h"

int main(){
    //@TODO FIX NON-DEBUG CRASH
    // Might be fixed??
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::View view(sf::Vector2f {64,64}, sf::Vector2f{SCREEN_WIDTH,SCREEN_HEIGHT});
    TextureManager::initialiseTextures("assets/textures/");

    Player player;
    sf::Clock clock;

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        window.clear();

        window.setView(view);
        player.update(deltaTime);

        player.draw(window);
        window.display();
    }
    return 0;
}
