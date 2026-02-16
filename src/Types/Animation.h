//
// Created by jay on 10/01/2026.
//

#ifndef SFML_MARIO_TEST_ANIMATION_H
#define SFML_MARIO_TEST_ANIMATION_H
#include <string>
#include <vector>


struct Frame;
struct FrameJson;

struct AnimationJson {
    std::vector<FrameJson> frames;
    std::string name;
    float speed = 1.f;
};

struct Animation {
    std::vector<Frame> frames;
    unsigned int frameIndex = 0;
    float frameTimer = 0.f;
    float defaultSpeed;
    float speed;
};


#endif //SFML_MARIO_TEST_ANIMATION_H