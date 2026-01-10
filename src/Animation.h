//
// Created by jay on 10/01/2026.
//

#ifndef SFML_MARIO_TEST_ANIMATIONMANAGER_H
#define SFML_MARIO_TEST_ANIMATIONMANAGER_H
#include <SFML/Graphics.hpp>

struct Frame {
    sf::IntRect rect;
    float duration = 1;
};

class Animation {
public:
    Animation() = default;
    void update(float deltaTime);

    // Verifiers
    bool isAnimated() const;

    // Getters
    sf::IntRect getFrameRect() const;

    // Setters
    void addFrame(const Frame& newFrame);
    void addFrame(const sf::IntRect& intRect);
    void addFrame(const sf::IntRect& intRect, float duration);
    void setFrameDurationScale(float scale);

private:
    std::vector<Frame> frames;
    unsigned int currentIndex = 0;
    float frameTimer = 0;
    float frameDurationScale = 1;
};


#endif //SFML_MARIO_TEST_ANIMATIONMANAGER_H