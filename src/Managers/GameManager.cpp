//
// Created by jay on 26/02/2026.
//

#include "GameManager.h"

#include "AnimationManager.h"
#include "EditorManager.h"
#include "FontManager.h"
#include "LevelManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "TileManager.h"
#include "../Globals.h"
#include "spdlog/spdlog.h"

bool GameManager::initialised = false;

Level GameManager::level;
sf::RenderWindow* GameManager::window = nullptr;
sf::View* GameManager::view = nullptr;

void GameManager::initialise(sf::RenderWindow* targetWindow, sf::View* targetView) {
    window = targetWindow;
    view = targetView;

    AnimationManager::initialise();
    TextureManager::initialise();
    ModelManager::initialise();
    TileManager::initialise();
    LevelManager::initialise();
    FontManager::initialise();
    EditorManager::initialise(window, view);

    level = LevelManager::getLevel("1-1.json");
    initialised = true;
}


void GameManager::update() {
    if (!isInitialised()) {
        spdlog::error("Tried to update GameManager, but it is not initialised!");
        throw std::runtime_error("GameManager is not initialised!");
    }
    while (const auto event = window->pollEvent()) {
        EditorManager::processEvents(event);
        if (event->is<sf::Event::Closed>()) {
            window->close();
        }
    }

    Globals::update(window);
    EditorManager::update();
    for (auto& tile : level.tiles | std::views::values) {
        tile.update();
    }
}

void GameManager::draw() {
    if (!isInitialised()) {
        spdlog::error("Tried to draw from GameManager, but it is not initialised!");
        throw std::runtime_error("GameManager is not initialised!");
    }
    window->setView(*view);

    for (const auto& tile : level.tiles | std::views::values) {
        tile.draw(*window);
    }
    EditorManager::draw();
}

bool GameManager::isInitialised() {
    return initialised;
}

Level* GameManager::getLevel() {
    return &level;
}