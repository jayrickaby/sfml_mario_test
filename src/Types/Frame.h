//
// Created by jay on 15/02/2026.
//

#ifndef SFML_MARIO_TEST_FRAME_H
#define SFML_MARIO_TEST_FRAME_H
#include "SFML/Graphics/Rect.hpp"


struct FrameJson {
    int size[2]{16,16};
    int pos[2]{0,0};
    float duration{1.f};
};

struct Frame {
    sf::IntRect rect;
    float duration;
};


#endif //SFML_MARIO_TEST_FRAME_H