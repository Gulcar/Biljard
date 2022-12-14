#pragma once

#include <cstdint>
#include <glm/vec2.hpp>

class Cue
{
public:
    Cue();
    void UpdateAndDraw(glm::vec2 ballPos, float deltaTime);
    bool DidShoot();
    glm::vec2 ShootForce();
    void Reset();

private:
    uint32_t m_cueTexture;
    float m_distFromBall = 0.0f;

    bool m_didShoot = false;
    glm::vec2 m_shootForce = {0.0f, 0.0f};
};