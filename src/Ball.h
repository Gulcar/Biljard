#pragma once

#include <glm/vec2.hpp>

class Ball
{
public:
    Ball(glm::vec2 pos = {0.0f, 0.0f});

    void Update(float deltaTime);
    void AddForce(glm::vec2 force);

    glm::vec2 position = {0.0f, 0.0f};
    glm::vec2 velocity = {0.0f, 0.0f};
    glm::vec2 force = {0.0f, 0.0f};
    float radius = 0.5f;
    bool isPotted = false;
};