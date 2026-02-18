#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Globals.h"
#include "Managers/AnimationManager.h"
#include "Managers/ModelManager.h"
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
    ModelManager::initialise();
    Model model = ModelManager::getModel("tiles/brick.json");

    while (window.isOpen()){
        window.setView(view);

        Globals::updateDeltaTime();
        model.update();

        window.clear();

        model.draw(window);

        window.display();
    }

    return 0;
}
