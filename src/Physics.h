#pragma once

#include <vector>
#include "Ball.h"

class Physics
{
public:
    static void Step(float timeStep, int iterations, std::vector<Ball>& balls);
};