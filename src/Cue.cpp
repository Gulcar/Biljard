#include "Cue.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#define PI 3.14159265358979323846264338327950288419716939f
#include <cmath>

Cue::Cue()
{
    m_cueTexture = Renderer::Instance.CreateTexture("resources/cue.png");
}

void Cue::UpdateAndDraw(glm::vec2 ballPos, float deltaTime)
{
    double mouseX, mouseY;
    glfwGetCursorPos(Renderer::Instance.window, &mouseX, &mouseY);

    glm::vec2 windowSize = {Renderer::Instance.windowWidth, Renderer::Instance.windowHeight};

    glm::vec2 mouseWorldPos = {mouseX, mouseY};
    mouseWorldPos -= windowSize / 2.0f;
    mouseWorldPos.y = -mouseWorldPos.y;
    mouseWorldPos.y = mouseWorldPos.y / (windowSize.y / 2.0f) * 10.0f;
    mouseWorldPos.x = mouseWorldPos.x / (windowSize.x / 2.0f) * 10.0f * (windowSize.x / windowSize.y);

    //std::cout << mouseWorldPos.x << ", " << mouseWorldPos.y << "\n";

    const glm::vec2 cueSize = {20.0f * (200.0f / 2211.0f), 20.0f};

    glm::vec2 dir = ballPos - mouseWorldPos;
    float angle = std::atan2(dir.y, dir.x);
    angle += PI / 2.0f; // 90 degrees

    if (glfwGetKey(Renderer::Instance.window, GLFW_KEY_SPACE))
    {
        m_distFromBall += 4.0f * deltaTime;
        if (m_distFromBall > 6.0f) m_distFromBall = 6.0f;
    }
    else if (m_distFromBall > 0.1f)
    {
        // TODO: mora samo enkrat dat m_didShoot na true
        if (m_didShoot == false) 
        {
            m_didShoot = true;
            m_shootForce = glm::normalize(-dir) * m_distFromBall * 500.0f;
        }

        m_distFromBall -= 300.0f * deltaTime;
        if (m_distFromBall < 0.0f) m_distFromBall = 0.0f;
    }

    glm::mat4 model(1.0f);
    model = glm::translate(model, {ballPos.x, ballPos.y, 0.0f});
    model = glm::rotate(model, angle, {0.0f, 0.0f, 1.0f});
    model = glm::translate(model, {0.0f, -10.25f - m_distFromBall, 0.0f});
    model = glm::scale(model, {cueSize.x, cueSize.y, 1.0f});

    //Renderer::Instance.DrawRect(ballPos, {20.0f * 200.0f / 2211.0f, 20.0f}, m_cueTexture);
    Renderer::Instance.DrawRectWithMatrix(model, m_cueTexture);
}

bool Cue::DidShoot()
{
    if (m_didShoot == true)
    {
        m_didShoot = false;
        return true;
    }
    return false;
}

glm::vec2 Cue::ShootForce()
{
    return m_shootForce;
}

void Cue::Reset()
{
    m_didShoot = false;
    m_distFromBall = 0.0f;
    m_shootForce = {0.0f, 0.0f};
}