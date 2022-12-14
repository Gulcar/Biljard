#include "Collision.h"
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <iostream>

void Collision::Check(Ball& ballA, Ball& ballB)
{
    float dist = glm::distance(ballA.position, ballB.position);

    if (dist < ballA.radius + ballB.radius)
    {
        glm::vec2 normal = glm::normalize(ballA.position - ballB.position);
        float depth = ballA.radius + ballB.radius - dist;

        ballA.position += normal * depth / 2.0f;
        ballB.position -= normal * depth / 2.0f;

        glm::vec2 tangent = glm::normalize(glm::vec2(- normal.y, normal.x));

        glm::vec2 relativeVelocity = ballA.velocity - ballB.velocity;

        float length = glm::dot(relativeVelocity, tangent);
        
        glm::vec2 velocityOnTangent = tangent * length;

        glm::vec2 velocityPerpendicularToTangent = relativeVelocity - velocityOnTangent;

        ballA.velocity.x -= velocityPerpendicularToTangent.x;
        ballA.velocity.y -= velocityPerpendicularToTangent.y;
        ballB.velocity.x += velocityPerpendicularToTangent.x;
        ballB.velocity.y += velocityPerpendicularToTangent.y;

        ballA.velocity *= 0.96f;
        ballB.velocity *= 0.96f;
    }
}

void Collision::CheckWalls(Ball& ball)
{
    CheckWall(ball, {13.75f, -5.5f}, {13.75f, 5.5f});
    CheckWall(ball, {-13.75f, 5.5f}, {-13.75f, -5.5f});

    CheckWall(ball, {12.5f, 6.9f}, {0.9f, 6.9f});
    CheckWall(ball, {-0.9f, 6.9f}, {-12.5f, 6.9f});

    CheckWall(ball, {0.9f, -6.9f}, {12.5f, -6.9f});
    CheckWall(ball, {-12.5f, -6.9f}, {-0.9f, -6.9f});
}

void Collision::CheckWall(Ball& ball, glm::vec2 wallA, glm::vec2 wallB)
{
    glm::vec2 wallDir = glm::normalize(wallA - wallB);
    glm::vec2 wallPerpendicularDir = glm::normalize(glm::vec2(-wallDir.y, wallDir.x));

    float c = glm::dot(ball.position, wallDir);
    float d = glm::dot(wallA, wallDir);
    float e = glm::dot(wallB, wallDir);
    if (c + ball.radius < std::min(d, e) || c - ball.radius > std::max(d, e))
        return;

    float a = glm::dot(wallA, wallPerpendicularDir);
    float b = glm::dot(ball.position, wallPerpendicularDir);

    float dist = glm::distance(a, b);

    if (dist < ball.radius)
    {
        float depth = ball.radius - dist;
        ball.position -= depth * wallPerpendicularDir;

        glm::vec2 tangent = wallDir;
        
        float l = glm::dot(ball.velocity, tangent);
        glm::vec2 velocityOnTangent = tangent * l;
        glm::vec2 velocityPerpendicualrToTangent = ball.velocity - velocityOnTangent;

        ball.velocity.x -= velocityPerpendicualrToTangent.x * 2.0f;
        ball.velocity.y -= velocityPerpendicualrToTangent.y * 2.0f;

        ball.velocity *= 0.96f;
    }
}

bool Collision::IsBallPotted(Ball& ball, glm::vec2* outPotPos)
{
    const glm::vec2 pots[] = {
        {13.9f, 7.0f},
        {-13.9f, 7.0f},
        {13.9f, -7.0f},
        {-13.9f, -7.0f},
        {0.0f, 7.6f},
        {0.0f, -7.6f}
    };

    for (int i = 0; i < sizeof(pots) / sizeof(glm::vec2); i++)
    {
        float dist = glm::distance(pots[i], ball.position);
        if (dist < 1.0f)
        {
            *outPotPos = pots[i];
            return true;
        }
    }

    *outPotPos = glm::vec2(0.0f, 0.0f);
    return false;
}