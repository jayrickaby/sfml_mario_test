//
// Created by jay on 10/01/2026.
//

#include "Animation.h"

void Animation::update(float deltaTime) {
    if (!isAnimated()) {
        frameTimer = 0.f;
        currentIndex = 0;
        return;
    }

    frameTimer += deltaTime;
    const Frame& currentFrame = frames[currentIndex];

    if (frameTimer >= currentFrame.duration * frameDurationScale) {
        frameTimer = 0.f;
        currentIndex++;
        if (currentIndex >= frames.size()) {
            currentIndex = 0;
        }
    }
}

bool Animation::isAnimated() const {
    return frames.size() > 1;
}

sf::IntRect Animation::getFrameRect() const {
    return frames[currentIndex].rect;
}

void Animation::addFrame(const Frame& newFrame) {
    frames.push_back(newFrame);
}

void Animation::addFrame(const sf::IntRect& intRect) {
    addFrame(intRect, 1.0);
}

void Animation::addFrame(const sf::IntRect& intRect, float duration) {
    Frame frame;
    frame.rect = intRect;
    frame.duration = duration;
    addFrame(frame);
}

void Animation::setFrameDurationScale(float scale) {
    frameDurationScale = scale;
}

void Animation::reset() {
    frameDurationScale = 1.f;
    frameTimer = 0.f;
    currentIndex = 0;
}
