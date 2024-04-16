#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "Shader.h"
#include <vector>
#include <cmath>

using namespace std;

std::vector<glm::vec3> vertices;

unsigned int vertexBuffer;
unsigned int vertexArray;

void buildSpiral(float radius, int loops, int segments)
{
    float angleIncrement = 2.0f * 3.1415 / segments;
    float radiusIncrement = radius / segments;
    float angle = 0.0f;
    float currentRadius = 0.0f;

    for (int i = 0; i < loops * segments; ++i)
    {
        float x = currentRadius * cos(angle);
        float y = currentRadius * sin(angle);
        float z = static_cast<float>(i) / (loops * segments); // Varying z coordinate for depth effect
        vertices.push_back(glm::vec3(x, y, z));

        angle += angleIncrement;
        currentRadius += radiusIncrement;
    }
}

int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Spiral", NULL, NULL);

    if (window == NULL)
    {
        cout << "Error. I could not create a window at all!" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    buildSpiral(0.1f, 5, 100); // Radius, number of loops, number of segments per loop

    Shader shader("../shaders/helloTriangle.vs", "../shaders/helloTriangle.fs");

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        glBindVertexArray(vertexArray);

        // Set spiral color
        GLint colorLoc = glGetUniformLocation(shader.ID, "circleColor");
        glUniform3f(colorLoc, 1.0f, 0.0f, 1.0f); // Purple color (example)

        // Draw the spiral
        glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
