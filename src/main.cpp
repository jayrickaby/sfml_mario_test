#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Globals.h"
#include "Managers/AnimationManager.h"
#include "Types/AnimationSet.h"

using json = nlohmann::json;

int main(){
    auto screenDimensions = Globals::getScreenDimensions();
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::View view(sf::Vector2f {screenDimensions.x/2,screenDimensions.y/2}, screenDimensions);

    AnimationManager::initialise();

    sf::Texture texture("assets/textures/tiles/questionblock.png");
    sf::Sprite sprite(texture);

    AnimationSet qblock = AnimationManager::getAnimationSet("tiles/questionblock.json");
    qblock.playAnimation("idle");

    while (window.isOpen()){
        window.setView(view);
        Globals::updateDeltaTime();
        qblock.updateAnimation();
        sprite.setTextureRect(qblock.getCurrentFrame());
        window.draw(sprite);
        window.display();
    }

    return 0;
}
