//
// Created by jay on 09/02/2026.
//

#include "Globals.h"

float Globals::deltaTime = 0.0f;
sf::Clock Globals::clock;

float Globals::getDeltaTime(){
    return deltaTime;
}

void Globals::updateDeltaTime(){
    deltaTime = clock.restart().asSeconds();
}
