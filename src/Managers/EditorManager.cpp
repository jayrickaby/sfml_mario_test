//
// Created by jay on 20/02/2026.
//

#include "EditorManager.h"

#include <iostream>
#include <string>

#include "FontManager.h"
#include "GameManager.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "../Globals.h"
#include "spdlog/spdlog.h"
#include "../Types/Tile.h"

sf::RenderWindow* EditorManager::window = nullptr;
sf::View* EditorManager::view = nullptr;
Level* EditorManager::level = nullptr;

std::string EditorManager::selectedObject;
std::optional<sf::Sprite> EditorManager::selectedObjectSprite;

sf::Vector2f EditorManager::mouseGridPosition;
std::optional<sf::Text> EditorManager::mousePositionText;

std::map<std::string, Tile> EditorManager::tiles;
bool EditorManager::enabled = true;
bool EditorManager::initialised = false;

void EditorManager::initialise(sf::RenderWindow* targetWindow, sf::View* targetView) {
    spdlog::info("Initialising Editor Manager...");
    window = targetWindow;
    view = targetView;
    level = GameManager::getLevel();
    mousePositionText = *FontManager::getFont("arial");

    if (ImGui::SFML::Init(*window)) {
        spdlog::info("ImGui Initialised!");
    };
    loadTiles();
    mousePositionText->setScale({0.25f, 0.25f});
    mousePositionText->setOutlineColor(sf::Color::Black);
    mousePositionText->setOutlineThickness(1);
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
        handleInput();
        createEditor();
        const sf::Vector2f mouseCoordPosition = window->mapPixelToCoords(Globals::getMousePosition(), *view);
        mouseGridPosition = {std::floor(mouseCoordPosition.x / 16), std::floor(mouseCoordPosition.y / 16)};
        mousePositionText->setPosition({mouseCoordPosition.x, mouseCoordPosition.y - 8});

        if (!selectedObject.empty()) {
            selectedObjectSprite = *tiles[selectedObject].getModelFile()->getSprite();
            selectedObjectSprite->setTextureRect(tiles[selectedObject].getModelFile()->getIntRect());
            // Multiply by 16 after dividing and floored so that it 'snaps' to grid
            selectedObjectSprite->setPosition({mouseGridPosition.x * 16, mouseGridPosition.y * 16});
        }

        const std::string text{"(" + std::to_string(mouseGridPosition.x) + ", " + std::to_string(mouseGridPosition.y) + ")"};
        mousePositionText->setString(text);
    }
}

void EditorManager::handleInput() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !selectedObject.empty()) {
        // Multiply by 16 after dividing and floored so that it 'snaps' to grid
        const std::pair coords{static_cast<int>(mouseGridPosition.x * 16),static_cast<int>(mouseGridPosition.y * 16)};
        //@TODO Fix QBlock animation Offset -> GameManager has global animation timer?? Or all animations reset when a animated tile is created
        level->tiles[coords] = TileManager::getTile(selectedObject);
        level->tiles[coords].setPosition(sf::Vector2i{coords.first, coords.second});
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
        if (mousePositionText.has_value()) {
            window->draw(*mousePositionText);
        }
    }
}

void EditorManager::enable() {
    enabled = true;
    window->setMouseCursorVisible(true);
}

void EditorManager::disable() {
    enabled = false;
    window->setMouseCursorVisible(false);
}

bool EditorManager::isEnabled() {
    return enabled;
}

void EditorManager::loadTiles() {
    tiles = TileManager::getTiles();
}

void EditorManager::createEditor() {
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
