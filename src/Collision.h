#pragma once

#include "Ball.h"
#include <glm/vec2.hpp>


class Collision
{
public:
    static void Check(Ball& ballA, Ball& ballB);
    static void CheckWalls(Ball& ball);

    static bool IsBallPotted(Ball& ball, glm::vec2* outPotPos);

private:
    static void CheckWall(Ball& ball, glm::vec2 wallA, glm::vec2 wallB); 
};
