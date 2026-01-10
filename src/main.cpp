#include <math.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "TextureManager.h"

int main(){
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::Clock clock;
    float deltaTime;
    float elapsedTime;

    TextureManager::initialiseTextures("assets/textures/");
    sf::Sprite sprite(TextureManager::loadTexture("assets/textures/mariosheet.png"));
    sprite.setScale({4.f, 4.f});

    std::map<std::string, Animation> animations;

    Animation idleAnimation;
    idleAnimation.addFrame(sf::IntRect({0,0}, {16,16}));

    Animation walkAnimation;
    walkAnimation.addFrame(sf::IntRect({16,0}, {16,16}), 0.1f);
    walkAnimation.addFrame(sf::IntRect({32,0}, {16,16}), 0.1f);
    walkAnimation.addFrame(sf::IntRect({48,0}, {16,16}), 0.1f);

    animations["idle"] = idleAnimation;
    animations["walk"] = walkAnimation;

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }

        deltaTime = clock.restart().asSeconds();
        elapsedTime += deltaTime;
        animations.at("walk").setFrameDurationScale(std::fabs(sin(elapsedTime)));

        window.clear();
        animations.at("walk").update(deltaTime);
        sprite.setTextureRect(animations.at("walk").getFrameRect());

        window.draw(sprite);
        window.display();
    }
    return 0;
}
