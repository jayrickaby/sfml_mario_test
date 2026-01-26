//
// Created by jay on 09/01/2026.
//

#ifndef SFML_MARIO_TEST_PLAYER_H
#define SFML_MARIO_TEST_PLAYER_H
#include <map>
#include <SFML/Graphics.hpp>

#include "Animation.h"

class Player{
public:
    Player();
    void update(float deltaTime);
    void handleInput();
    void draw(sf::RenderTarget& target);

    // Getters
    sf::Vector2f getPosition() const { return position; };

    // Setters
    void setAnimation(const std::string& name);
    void setPosition(const sf::Vector2f& pos) {position = pos;}

private:
    // Appearance
    sf::Texture* texture;
    sf::Sprite sprite;
    std::map<std::string, Animation> animations;
    Animation* currentAnimation;
    std::string currentAnimationName;

    // Physics
    bool onGround;
    bool isJumping;
    bool isSkidding;
    signed int direction;
    const float walkSpeed;
    const float walkAcceleration;
    const float runSpeed;
    const float runAcceleration;
    const float dampening;
    const float gravity;
    const float jumpStrength;
    sf::Vector2f velocity;
    sf::Vector2f position;
    sf::FloatRect physicsBox;
};

#endif //SFML_MARIO_TEST_PLAYER_H