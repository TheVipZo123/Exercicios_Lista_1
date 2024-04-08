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

void buildCircle(float radius, int vCount, float aspectRatio)
{
    float angle = 360.0f / vCount;
    int triangleCount = vCount - 2;
    std::vector<glm::vec3> temp;

    for (int i = 0; i < vCount; i++)
    {
        float currentAngle = angle * i;
        float x = radius * cos(glm::radians(currentAngle));
        float y = radius * aspectRatio * sin(glm::radians(currentAngle)); // Adjust y coordinate based on aspect ratio
        float z = 0.0f;

        temp.push_back(glm::vec3(x, y, z));
    }

    for (int i = 0; i < triangleCount; i++)
    {
        vertices.push_back(temp[0]);
        vertices.push_back(temp[i + 1]);
        vertices.push_back(temp[i + 2]);
    }
}

int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Circle", NULL, NULL);

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
    buildCircle(0.1, 256, static_cast<float>(800) / static_cast<float>(600));

    Shader shader("../shaders/helloTriangle.vs", "../shaders/helloTriangle.fs");

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        glBindVertexArray(vertexArray);

        // Set circle color
        GLint colorLoc = glGetUniformLocation(shader.ID, "circleColor");
        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f); // Red color (example)

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
