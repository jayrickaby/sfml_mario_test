//
// Created by jay on 09/02/2026.
//

#include "Globals.h"

float Globals::deltaTime = 0.0f;
sf::Vector2i Globals::mousePosition = {0,0};

sf::Clock Globals::clock;

float Globals::getDeltaTime(){
    return deltaTime;
}

void Globals::update(const sf::RenderWindow* window){
    deltaTime = clock.restart().asSeconds();
    mousePosition = sf::Mouse::getPosition(*window);
}
