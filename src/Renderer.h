#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

class Color
{
public:
    Color(uint32_t colorCode)
    {
        b = ((float)(colorCode & 0xff)) / 256.0f;
        colorCode >>= 8;
        g = ((float)(colorCode & 0xff)) / 256.0f;
        colorCode >>= 8;
        r = ((float)(colorCode & 0xff)) / 256.0f;
        a = 1.0f;
    }

    Color(float r, float g, float b, float a = 1.0f)
        : r(r), g(g), b(b), a(a)
    {

    }

    float r, g, b, a;
};

class Renderer
{
public:
    static Renderer Instance;

    void Init();

    void DrawBall(class Ball& ball, uint32_t textureId, Color color = Color(1.0f, 1.0f, 1.0f));
    void DrawRect(glm::vec2 pos, glm::vec2 size, uint32_t textureId);
    void DrawRectWithMatrix(const glm::mat4& matrix, uint32_t textureId);

    uint32_t CreateShader(const char* source, uint32_t type);
    uint32_t CreateShaderProgram(const char* vertexSource, const char* fragmentSource);
    uint32_t CreateTexture(const char* filename);

    struct GLFWwindow* window;
    float windowWidth = 1600.0f;
    float windowHeight = 900.0f;

    uint32_t shaderProgram;

    uint32_t rectVao, rectVbo;
    uint32_t circleVao, circleVbo;

private:
    Renderer() 
        : window(nullptr), rectVao(-1), rectVbo(-1), circleVao(-1), circleVbo(-1)
    {}

    void CreateRectVao();
    void CreateCircleVao();

    void BindViewAndProjectionMatrix();

    const int numOfCircleVertices = 35;
};