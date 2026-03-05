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
#include "TextureManager.h"
#include "../Globals.h"
#include "spdlog/spdlog.h"
#include "../Types/Tile.h"

sf::RenderWindow* EditorManager::window = nullptr;
sf::View* EditorManager::view = nullptr;
Level* EditorManager::level = nullptr;

std::string EditorManager::selectedObject;
std::optional<sf::Sprite> EditorManager::selectedObjectSprite;
sf::RectangleShape EditorManager::tileHighlight;

EditorTool EditorManager::currentTool = EditorTool::Pencil;

sf::Vector2f EditorManager::mouseGridPosition;
std::optional<sf::Text> EditorManager::mousePositionText;

std::map<std::string, Tile> EditorManager::tiles;
bool EditorManager::enabled = false;
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
    tileHighlight.setSize({14.f,14.f});
    tileHighlight.setOutlineThickness(1);

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

        tileHighlight.setFillColor({255,255,255,128});
        tileHighlight.setOutlineColor({255,255,255,255});

        if (currentTool == EditorTool::Pencil) {
            if (!selectedObject.empty() && selectedObjectSprite.has_value()) {
                selectedObjectSprite->setTextureRect(tiles[selectedObject].getModelFile()->getIntRect());
                // Multiply by 16 after dividing and floored so that it 'snaps' to grid
                selectedObjectSprite->setPosition({mouseGridPosition.x * 16, mouseGridPosition.y * 16});
                selectedObjectSprite->setColor({255,255,255,192});
            }
            else if (!selectedObject.empty() && !selectedObjectSprite.has_value()) {
                selectedObjectSprite = *tiles[selectedObject].getModelFile()->getSprite();
            }
        }
        else if (currentTool == EditorTool::Eraser) {
            tileHighlight.setFillColor({255,0,0, 128});
            tileHighlight.setOutlineColor({255,0,0, 255});
        }
        tileHighlight.setPosition({mouseGridPosition.x*16 + 1, mouseGridPosition.y*16 + 1});

        const std::string text{"(" + std::to_string(mouseGridPosition.x) + ", " + std::to_string(mouseGridPosition.y) + ")"};
        mousePositionText->setString(text);
    }
}

void EditorManager::handleInput() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        const std::pair grid {static_cast<int>(mouseGridPosition.x), static_cast<int>(mouseGridPosition.y)};
        const std::pair coords{static_cast<int>(mouseGridPosition.x * 16),static_cast<int>(mouseGridPosition.y * 16)};
        if (currentTool == EditorTool::Pencil && !selectedObject.empty()) {
            // Multiply by 16 after dividing and floored so that it 'snaps' to grid
            level->tiles[grid] = TileManager::getTile(selectedObject);
            level->tiles[grid].update();
            level->tiles[grid].reset();
            level->tiles[grid].setPosition(sf::Vector2i{coords.first, coords.second});
        }
        else if (currentTool == EditorTool::Eraser) {
            level->tiles.erase(grid);
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
        if (selectedObjectSprite.has_value() && currentTool == EditorTool::Pencil) {
            window->draw(*selectedObjectSprite);
        }
        window->draw(tileHighlight);
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

    sf::Sprite pencilSprite(*TextureManager::getAtlas());
    pencilSprite.setTextureRect(TextureManager::getTexture("editor/pencil.png")->rect);

    if (ImGui::ImageButton("Pencil", pencilSprite, sf::Vector2f({32.f,32.f}))) {
        currentTool = EditorTool::Pencil;
    };

    ImGui::SameLine();

    sf::Sprite eraserSprite(*TextureManager::getAtlas());
    eraserSprite.setTextureRect(TextureManager::getTexture("editor/eraser.png")->rect);

    //@TODO Add InputManager in order to add keybinds for Pencil & Eraser
    if (ImGui::ImageButton("Eraser", eraserSprite, sf::Vector2f({32.f,32.f}))) {
        currentTool = EditorTool::Eraser;
    };
    if (ImGui::CollapsingHeader("Tiles")) {
        for (auto& tile : tiles) {
            tile.second.update();
            tile.second.reset();
            sf::Sprite sprite = *tile.second.getModelFile()->getSprite();
            if (ImGui::ImageButton(tile.first.c_str(), sprite, sf::Vector2f{32,32})){
                selectedObject = tile.first;
                currentTool = EditorTool::Pencil;
            }
            ImGui::SameLine();
        }
    }

    ImGui::NewLine();

    float backgroundColour[3] = {
        level->properties.backgroundColour.r / 255.0f,
        level->properties.backgroundColour.g / 255.0f,
        level->properties.backgroundColour.b / 255.0f
    };

    if (ImGui::CollapsingHeader("Level Properties")) {
        if (ImGui::ColorEdit3("Background Colour", backgroundColour, ImGuiColorEditFlags_NoInputs)) {
            level->properties.backgroundColour.r = static_cast<uint8_t>(backgroundColour[0] * 255.0f);
            level->properties.backgroundColour.g = static_cast<uint8_t>(backgroundColour[1] * 255.0f);
            level->properties.backgroundColour.b = static_cast<uint8_t>(backgroundColour[2] * 255.0f);
        }
    }

    ImGui::End();
}
