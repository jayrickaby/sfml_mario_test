//
// Created by jay on 20/02/2026.
//

#ifndef SFML_MARIO_TEST_EDITORMANAGER_H
#define SFML_MARIO_TEST_EDITORMANAGER_H
#include "TileManager.h"
#include "SFML/Graphics.hpp"

struct Level;

class EditorManager {
public:
    EditorManager() = default;
    ~EditorManager() = default;

    static void initialise(sf::RenderWindow* targetWindow, sf::View* targetView);
    static void processEvents(const std::optional<sf::Event>& event);
    static void update();
    static void draw();

    static void enable();
    static void disable();

    // Validators
    static bool isEnabled();


private:
    static void loadTiles();
    static void createEditor();

    static std::string selectedObject;
    static std::optional<sf::Sprite> selectedObjectSprite;

    static sf::Vector2i mouseGridPosition;
    static sf::Text mousePositionText;

    static std::map<std::string, Tile> tiles;
    static bool initialised;
    static bool enabled;

    static sf::RenderWindow* window;
    static sf::View* view;
    static Level* level;
};


#endif //SFML_MARIO_TEST_EDITORMANAGER_H
