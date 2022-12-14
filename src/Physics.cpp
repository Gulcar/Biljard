#include "Physics.h"

#include "Collision.h"

void Physics::Step(float timeStep, int iterations, std::vector<Ball>& balls)
{
    for (int it = 0; it < iterations; it++)
    {
        for (Ball& b : balls)
        {
            b.Update(timeStep / iterations);

            if (Collision::IsBallPotted(b))
            {
                b.position = {10000.0f, 10000.0f};
                b.velocity = {0.0f, 0.0f};
                b.force = {0.0f, 0.0f};
                b.isPotted = true;
            }
        }

        for (int i = 0; i < balls.size() - 1; i++)
        {
            for (int j = i + 1; j < balls.size(); j++)
            {
                Collision::Check(balls[i], balls[j]);
            }
        }

        for (Ball& b : balls)
        {
            Collision::CheckWalls(b);
        }
    }
}