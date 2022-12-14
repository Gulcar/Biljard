#include "Ball.h"

#include <iostream>

Ball::Ball(glm::vec2 pos)
    : position(pos)
{

}

void Ball::Update(float deltaTime)
{
    velocity += force * deltaTime;

    position += velocity * deltaTime;
    
    force = {0.0f, 0.0f};

    // friction
    velocity -= 0.03f * velocity * deltaTime * 30.0f;
}

void Ball::AddForce(glm::vec2 force)
{
    this->force += force;
}