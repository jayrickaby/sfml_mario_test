//
// Created by jay on 09/01/2026.
//

#ifndef SFML_MARIO_TEST_PLAYER_H
#define SFML_MARIO_TEST_PLAYER_H
#include <map>
#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "AnimationSubManager.h"
#include "CollisionSide.h"

class Tile;

class Player{
public:
    Player();
    void update(float deltaTime);
    void updateAnimation(float deltaTime);
    void move(float deltaTime);
    void moveX(float deltaTime);
    void moveY(float deltaTime);
    void collide(CollisionSide side, sf::FloatRect overlap);
    void collideX(CollisionSide side, sf::FloatRect overlap);
    void collideY(CollisionSide side, sf::FloatRect overlap);
    void handleInput();
    void draw(sf::RenderTarget& target) const;
    void initialisePlayer();

    // Getters
    sf::FloatRect getBoundingBox() const;
    sf::Vector2f getPosition() const;

    // Setters
    void setPosition(const sf::Vector2f& pos) {position = pos;}

private:
    // Appearance
    std::shared_ptr<sf::Texture> texture;
    std::unique_ptr<sf::Sprite> sprite;
    AnimationSubManager animationSubManager;

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