#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Globals.h"
#include "Managers/AnimationManager.h"
#include "Managers/LevelManager.h"
#include "Managers/ModelManager.h"
#include "Managers/TextureManager.h"
#include "Managers/TileManager.h"
#include "Types/Level.h"
#include "Types/Tile.h"

using json = nlohmann::json;

int main(){
    auto screenDimensions = Globals::getScreenDimensions();
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::View view(sf::Vector2f {screenDimensions.x/2,screenDimensions.y/2}, screenDimensions);

    AnimationManager::initialise();
    TextureManager::initialise();
    ModelManager::initialise();
    TileManager::initialise();
    LevelManager::initialise();
    ImGui::SFML::Init(window);

    Level level = LevelManager::getLevel("1-1.json");

    std::map<std::string, Tile> uiTiles = TileManager::getTiles();

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        Globals::updateDeltaTime();
        for (auto& tile : level.tiles | std::views::values) {
            tile.update();
        }

        ImGui::SFML::Update(window, sf::seconds(Globals::getDeltaTime()));

        ImGui::Begin("Hello, world!");
        for (auto& uiTile : uiTiles) {
            uiTile.second.update();
            sf::Sprite sprite = *uiTile.second.getModelFile()->getSprite();
            ImGui::ImageButton(uiTile.first.c_str(), sprite, sf::Vector2f{64,64});
        }

        ImGui::End();

        window.clear();
        window.setView(view);
        for (const auto& tile : level.tiles | std::views::values) {
            tile.draw(window);
        }

        window.setView(window.getDefaultView());
        ImGui::SFML::Render(window);
        window.display();
    }
}