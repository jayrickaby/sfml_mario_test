//
// Created by jay on 18/02/2026.
//

#ifndef SFML_MARIO_TEST_TEXTURE_H
#define SFML_MARIO_TEST_TEXTURE_H
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"

struct Texture {
    std::filesystem::path path;
    sf::IntRect rect;
    sf::Texture* atlas;
};

#endif //SFML_MARIO_TEST_TEXTURE_H