//
// Created by jay on 20/02/2026.
//

#include "EditorManager.h"

#include <iostream>
#include <string>
#include "imgui.h"
#include "imgui-SFML.h"
#include "../Globals.h"
#include "spdlog/spdlog.h"
#include "../Types/Tile.h"

sf::RenderWindow* EditorManager::window = nullptr;
sf::View* EditorManager::view = nullptr;

std::string EditorManager::selectedObject;
std::optional<sf::Sprite> EditorManager::selectedObjectSprite;

sf::Vector2i EditorManager::mouseGridPosition;
sf::Text EditorManager::mousePositionText("");

std::map<std::string, Tile> EditorManager::tiles;
bool EditorManager::enabled = true;
bool EditorManager::initialised = false;

void EditorManager::initialise(sf::RenderWindow* targetWindow, sf::View* targetView) {
    spdlog::info("Initialising Editor Manager...");
    window = targetWindow;
    view = targetView;
    if (ImGui::SFML::Init(*window)) {
        spdlog::info("ImGui Initialised!");
    };
    loadTiles();
    initialised = true;
    spdlog::info("Initialised Editor Manager!");
}

void EditorManager::processEvents(const std::optional<sf::Event>& event) {
    if (event.has_value()) {
        ImGui::SFML::ProcessEvent(*window, *event);
    }
}

void EditorManager::update() {
    if (!initialised) {
        spdlog::critical("Tried to update Editor Manager, but it isn't initialised!");
        throw std::runtime_error("Editor Manager is uninitialised!");
    }
    if (window == nullptr) {
        spdlog::critical("Tried to update Editor Manager, but no window has been attached!");
        throw std::runtime_error("Editor Manager has no attached window!");
    }
    if (enabled) {
        ImGui::SFML::Update(*window, sf::seconds(Globals::getDeltaTime()));
        createEditor();
        if (!selectedObject.empty()) {
            selectedObjectSprite = *tiles[selectedObject].getModelFile()->getSprite();
            selectedObjectSprite->setTextureRect(tiles[selectedObject].getModelFile()->getIntRect());
            selectedObjectSprite->setPosition(window->mapPixelToCoords(Globals::getMousePosition(), *view));
        }
    }
}

void EditorManager::draw() {
    if (!initialised) {
        spdlog::critical("Tried to draw Editor Manager, but it isn't initialised!");
        throw std::runtime_error("Editor Manager is uninitialised!");
    }
    if (window == nullptr) {
        spdlog::critical("Tried to draw Editor Manager, but no window has been attached!");
        throw std::runtime_error("Editor Manager has no attached window!");
    }
    if (enabled) {
        if (selectedObjectSprite.has_value()) {
            window->draw(*selectedObjectSprite);
        }
        ImGui::SFML::Render(*window);
    }
}

void EditorManager::enable() {
    enabled = true;
}

void EditorManager::disable() {
    enabled = false;
}

bool EditorManager::isEnabled() {
    return enabled;
}

void EditorManager::loadTiles() {
    tiles = TileManager::getTiles();
}

void EditorManager::createEditor() {
    if (!initialised) {
        spdlog::critical("Tried to create Editor Manager, but it isn't initialised!");
        throw std::runtime_error("Editor Manager is uninitialised!");
    }
    if (window == nullptr) {
        spdlog::critical("Tried to create Editor Manager, but no window has been attached!");
        throw std::runtime_error("Editor Manager has no attached window!");
    }
    sf::Vector2f mousePosition({window->mapPixelToCoords(Globals::getMousePosition(), *view)});
    mouseGridPosition = ({std::floor(mousePosition.x), std::floor(mousePosition.y)});



    ImGui::Begin("Editor");

    for (auto& tile : tiles) {
        tile.second.update();
        sf::Sprite sprite = *tile.second.getModelFile()->getSprite();
        if (ImGui::ImageButton(tile.first.c_str(), sprite, sf::Vector2f{32,32})){
            selectedObject = tile.first;
        }
    }

    ImGui::End();
}
