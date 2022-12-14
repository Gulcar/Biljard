#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
#include "Ball.h"
#include <vector>
#include "Collision.h"
#include "Cue.h"
#include "Physics.h"
#include <ctime>
#include <cstdlib>

int main(int argc, char* argv[])
{
    srand(time(nullptr));

    Renderer::Instance.Init();

    uint32_t whiteTexture = Renderer::Instance.CreateTexture("resources/whitepixel.jpg");

    uint32_t tableTexture;
    if (rand() % 10 == 0)
        tableTexture = Renderer::Instance.CreateTexture("resources/table1.png");
    else
        tableTexture = Renderer::Instance.CreateTexture("resources/table.png");

    std::vector<Ball> balls;

    const glm::vec2 whiteBallStartPos = { -6.9f, 0.0f };
    balls.emplace_back(whiteBallStartPos);

    const glm::vec2 firstBallPos = {6.9f - 2 * 1.1f, 0.0f};
    for (int i = 1; i <= 5; i++)
    {
        for (int j = 0; j < i; j++)
        {
            //if (i == 3) break;
            balls.emplace_back(firstBallPos + glm::vec2((i-1) * 1.1f, ((i-1) / 2.0f - j) * 1.1f));
        }
    }

    Cue cue;

    Color ballColors[] = {
        0xffffff,
        0xff0000,
        0x800000,
        0xffff00,
        0x00ff00,
        0x000000,
        0x00ffff,
        0xff00ff,
        0x0000ff,
        0xf00080,
        0x808000,
        0x00c0ff,
        0x008080,
        0xff9000,
        0x00ff90,
        0x800080,
    };

    while (!glfwWindowShouldClose(Renderer::Instance.window))
    {
        static float prevFrameTime = 0.0f;
        float deltaTime = glfwGetTime() - prevFrameTime;
        prevFrameTime = glfwGetTime();

        glfwPollEvents();

        if (glfwGetKey(Renderer::Instance.window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(Renderer::Instance.window, true);

        const float ballSpeed = 50.0f;
        if (glfwGetKey(Renderer::Instance.window, GLFW_KEY_W))
            balls[0].AddForce({0.0f, ballSpeed});
        if (glfwGetKey(Renderer::Instance.window, GLFW_KEY_S))
            balls[0].AddForce({0.0f, -ballSpeed});
        if (glfwGetKey(Renderer::Instance.window, GLFW_KEY_A))
            balls[0].AddForce({-ballSpeed, 0.0f});
        if (glfwGetKey(Renderer::Instance.window, GLFW_KEY_D))
            balls[0].AddForce({ballSpeed, 0.0f});


        Physics::Step(deltaTime, 1, balls);


        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Renderer::Instance.DrawRect({0.0f, 0.0f}, {17.0f * 1579.0f / 873.0f, 17.0f}, tableTexture);

        //Renderer::Instance.DrawRect({0.0f, 5.0f}, {1.0f, 1.0f}, whiteTexture);
        //Renderer::Instance.DrawRect({6.75f, 7.0f}, {12.0f, 1.0f}, whiteTexture);

        Ball testb;
        testb.position = {13.9f, 7.0f};
        testb.radius = 1.0f;
        //Renderer::Instance.DrawBall(testb, whiteTexture);

        for (int i = 0; i < balls.size(); i++)
        {
            if (balls[i].isPotted == false)
                Renderer::Instance.DrawBall(balls[i], whiteTexture, ballColors[i]);
        }

        bool ballsMoving = false;
        for (Ball& b : balls)
        {
            if (glm::abs(b.velocity.x) > 0.03f || glm::abs(b.velocity.y) > 0.03f || glm::abs(b.force.x) > 0.0f || glm::abs(b.force.y) > 0.0f)
            {
                ballsMoving = true;
                cue.Reset();
                break;
            }
        }
        if (ballsMoving == false)
            cue.UpdateAndDraw(balls[0].position, deltaTime);
        if (cue.DidShoot())
        {
            balls[0].AddForce(cue.ShootForce());
        }

        if (ballsMoving == false && balls[0].isPotted)
        {
            balls[0].isPotted = false;
            balls[0].position = whiteBallStartPos;
        }

        glfwSwapBuffers(Renderer::Instance.window);
    }

    glfwTerminate();
}

