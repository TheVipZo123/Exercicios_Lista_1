#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "Shader.h"
#include <vector>

using namespace std;

std::vector<glm::vec3> vertices;

unsigned int vertexBuffer;
unsigned int vertexArray;

void buildStar(float size)
{
    // Define os vértices manualmente para formar uma estrela
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); // Centro da estrela

    float outerRadius = size;
    float innerRadius = size * 0.4f;

    for (int i = 0; i < 5; ++i)
    {
        float angle = 4.0f * static_cast<float>(i)* static_cast<float>(3.1415) / 5.0f;
        float x = cos(angle);
        float y = sin(angle);
        vertices.push_back(glm::vec3(x * outerRadius, y * outerRadius, 0.0f));
        angle += static_cast<float>(3.1415) / 5.0f;
        x = cos(angle);
        y = sin(angle);
        vertices.push_back(glm::vec3(x * innerRadius, y * innerRadius, 0.0f));
    }
}

int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Star", NULL, NULL);

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

    buildStar(0.5f); // Tamanho da estrela

    Shader shader("../shaders/helloTriangle.vs", "../shaders/helloTriangle.fs");

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Loop de renderização
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        glBindVertexArray(vertexArray);

        // Define a cor da estrela
        GLint colorLoc = glGetUniformLocation(shader.ID, "circleColor");
        glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f); // Cor amarela (exemplo)

        // Desenha a estrela
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
