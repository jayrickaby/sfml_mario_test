//
// Created by jay on 10/01/2026.
//

#ifndef SFML_MARIO_TEST_ANIMATION_H
#define SFML_MARIO_TEST_ANIMATION_H
#include <SFML/Graphics.hpp>

struct Frame {
    sf::IntRect rect;
    float duration = 1;
};

class Animation {
public:
    Animation(const std::string& givenName);
    void update();

    // Verifiers
    bool isAnimated() const;

    // Getters
    std::string getName() const;
    sf::IntRect getFrameRect() const;

    // Setters
    void addFrame(const Frame& newFrame);
    void addFrame(const sf::IntRect& intRect);
    void addFrame(const sf::IntRect& intRect, float duration);
    void setFrameDurationScale(float scale);
    void reset();

private:
    std::string name;
    std::vector<Frame> frames;
    unsigned int currentIndex = 0;
    float frameTimer = 0;
    float frameDurationScale = 1;
};


#endif //SFML_MARIO_TEST_ANIMATION_H