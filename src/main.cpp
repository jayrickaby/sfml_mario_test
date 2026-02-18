#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Globals.h"
#include "Managers/AnimationManager.h"
#include "Managers/TextureManager.h"
#include "Types/AnimationSet.h"
#include "Types/Texture.h"

using json = nlohmann::json;

int main(){
    auto screenDimensions = Globals::getScreenDimensions();
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::View view(sf::Vector2f {screenDimensions.x/2,screenDimensions.y/2}, screenDimensions);

    AnimationManager::initialise();
    TextureManager::initialise();
    Texture* texture = TextureManager::getTexture("tiles/questionblock.png");
    AnimationSet anims = AnimationManager::getAnimationSet("tiles/questionblock.json");
    sf::Sprite sprite(*texture->atlas);

    while (window.isOpen()){
        window.setView(view);

        Globals::updateDeltaTime();
        anims.update();

        window.clear();

        sf::IntRect rect(anims.getCurrentFrame());
        rect.position += texture->rect.position;
        sprite.setTextureRect(rect);

        window.draw(sprite);
        window.display();
    }

    return 0;
}
