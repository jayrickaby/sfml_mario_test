#include <iostream>
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>


#include "AnimationManager.h"
#include "constants.h"
#include "Player.h"
#include "TextureManager.h"

using json = nlohmann::json;

int main(){
    //@TODO FIX NON-DEBUG CRASH
    // Might be fixed??
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::View view(sf::Vector2f {64,64}, sf::Vector2f{SCREEN_WIDTH,SCREEN_HEIGHT});

    AnimationManager::initialiseAnimations("assets/animations/");
    TextureManager::initialiseTextures("assets/textures/");
    sf::Texture* grnd_tilesheet = &TextureManager::loadTexture("assets/textures/grnd_tilesheet.png");



    std::vector<sf::Sprite> groundBlocks;
    for (int i = 0; i < 16; i++){
        sf::Sprite groundBlock(*grnd_tilesheet);
        groundBlock.setTextureRect(sf::IntRect({0,0}, {16,16}));
        groundBlock.setPosition({i * 16.f, SCREEN_HEIGHT - 16.f});
        groundBlocks.push_back(groundBlock);
    }

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

        for (const auto& block : groundBlocks){
            window.draw(block);
        }

        window.display();
    }
    return 0;
}
