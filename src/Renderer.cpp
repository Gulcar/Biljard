#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "Ball.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

Renderer Renderer::Instance;

const char* vertexSource = R"(
#version 430 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
    TexCoords = aTexCoords;
}
)";

const char* fragmentSource = R"(
#version 430 core

out vec4 FragColor;
in vec2 TexCoords;

uniform vec4 colorTint;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoords) * colorTint;
}
)";

void GlfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Renderer::Instance.windowWidth = (float)width;
    Renderer::Instance.windowHeight = (float)height;
}

void Renderer::Init()
{
    if (!glfwInit()) std::cout << "ERROR: failed to init glfw!\n";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1600, 900, "Biljard", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!window) std::cout << "ERROR: failed to create a window!\n";

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetFramebufferSizeCallback(window, GlfwFramebufferSizeCallback);
    glViewport(0, 0, 1600, 900);

    shaderProgram = CreateShaderProgram(vertexSource, fragmentSource);

    CreateRectVao();
    CreateCircleVao();

    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
}

void Renderer::DrawBall(Ball& ball, uint32_t textureId, Color color)
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, {ball.position.x, ball.position.y, 0.0f});
    model = glm::scale(model, {ball.radius, ball.radius, 1.0f});
    
    BindViewAndProjectionMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    glUniform4fv(glGetUniformLocation(shaderProgram, "colorTint"), 1, &color.r);

    glBindVertexArray(circleVao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numOfCircleVertices);
}

void Renderer::DrawRect(glm::vec2 pos, glm::vec2 size, uint32_t textureId)
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, {pos.x, pos.y, 0.0f});
    model = glm::scale(model, {size.x, size.y, 1.0f});

    BindViewAndProjectionMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    glUniform4f(glGetUniformLocation(shaderProgram, "colorTint"), 1.0f, 1.0f, 1.0f, 1.0f);

    glBindVertexArray(rectVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawRectWithMatrix(const glm::mat4& matrix, uint32_t textureId)
{
    BindViewAndProjectionMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &matrix[0][0]);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    glUniform4f(glGetUniformLocation(shaderProgram, "colorTint"), 1.0f, 1.0f, 1.0f, 1.0f);

    glBindVertexArray(rectVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

uint32_t Renderer::CreateShader(const char* source, uint32_t type)
{
    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char* infoLog = (char*)_malloca(logLength * sizeof(char));
        glGetShaderInfoLog(shader, logLength, 0, infoLog);
        std::cout << "ERROR COMPILING SHADER:\n" << infoLog << "\n";
    }

    return shader;
}

uint32_t Renderer::CreateShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    uint32_t vertexShader = CreateShader(vertexSource, GL_VERTEX_SHADER);
    uint32_t fragmentShader = CreateShader(fragmentSource, GL_FRAGMENT_SHADER);

    uint32_t program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetShaderiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        int logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        char* infoLog = (char*)_malloca(logLength * sizeof(char));
        glGetProgramInfoLog(program, logLength, 0, infoLog);
        std::cout << "ERROR LINKING SHADER PROGRAM:\n" << infoLog << "\n";
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

uint32_t Renderer::CreateTexture(const char* filename)
{
    std::string relativeFilename = std::string("../../../") + filename;
    std::string differentRelative = std::string("../") + filename;

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, numOfChannels;
    uint8_t* data = stbi_load(filename, &width, &height, &numOfChannels, 0);
    if (!data) data = stbi_load(relativeFilename.c_str(), &width, &height, &numOfChannels, 0);
    if (!data) data = stbi_load(differentRelative.c_str(), &width, &height, &numOfChannels, 0);

    if (data)
    {
        GLenum format;
        switch (numOfChannels)
        {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: std::cout << "ERROR: weird image format!\n";
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "ERROR: failed to load image: " << filename << "\n";
    }

    stbi_image_free(data);

    return texture;
}

void Renderer::CreateRectVao()
{
    float rectVertices[] = {
        -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,   1.0f, 1.0f,

         0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f,   0.0f, 0.0f
    };
    glGenVertexArrays(1, &rectVao);
    glBindVertexArray(rectVao);
    glGenBuffers(1, &rectVbo);
    glBindBuffer(GL_ARRAY_BUFFER, rectVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::CreateCircleVao()
{
    float* vertices = new float[numOfCircleVertices * 4];

    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.5f;
    vertices[3] = 0.5f;

    for (int i = 4; i < numOfCircleVertices * 4; i += 4)
    {
        float angle = (float)(i - 4) / (numOfCircleVertices * 4 - 8) * 360.0f;

        vertices[i] = cosf(angle * M_PI / 180.0f);
        vertices[i + 1] = sinf(angle * M_PI / 180.0f);
        vertices[i + 2] = vertices[i] / 2.0f + 0.5f;
        vertices[i + 3] = vertices[i + 1] / 2.0f + 0.5f;
    }

    glGenVertexArrays(1, &circleVao);
    glBindVertexArray(circleVao);
    glGenBuffers(1, &circleVbo);
    glBindBuffer(GL_ARRAY_BUFFER, circleVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfCircleVertices * 4, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete[] vertices;
}


void Renderer::BindViewAndProjectionMatrix()
{
    glm::mat4 view(1.0f);

    float windowRatio = Renderer::Instance.windowWidth / Renderer::Instance.windowHeight;
    glm::mat4 projection = glm::ortho(-10.0f * windowRatio, 10.0f * windowRatio, -10.0f, 10.0f);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
}