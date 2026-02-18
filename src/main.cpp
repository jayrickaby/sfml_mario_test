#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Globals.h"
#include "Managers/AnimationManager.h"
#include "Managers/ModelManager.h"
#include "Managers/TextureManager.h"
#include "spdlog/spdlog.h"

using json = nlohmann::json;

int main(){
    auto screenDimensions = Globals::getScreenDimensions();
    sf::RenderWindow window(sf::VideoMode({600,600}), "Super Mario Bros.");
    sf::View view(sf::Vector2f {screenDimensions.x/2,screenDimensions.y/2}, screenDimensions);
    ImGui::SFML::Init(window);

    AnimationManager::initialise();
    TextureManager::initialise();
    ModelManager::initialise();

    Model model = ModelManager::getModel("tiles/questionblock.json");

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        Globals::updateDeltaTime();
        model.update();
        ImGui::SFML::Update(window, sf::seconds(Globals::getDeltaTime()));

        ImGui::ShowDemoWindow();

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        window.setView(view);
        model.draw(window);
        window.setView(window.getDefaultView());
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}