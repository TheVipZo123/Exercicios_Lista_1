#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <cmath>

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

void buildPizzaSlice(float radius, int vCount, float sliceAngle)
{
    float angleIncrement = glm::radians(sliceAngle) / vCount; // Angle increment for the pizza slice
    std::vector<glm::vec3> temp;

    // Center of the pizza slice
    temp.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

    // Add vertices for the circular part of the slice
    for (int i = 0; i <= vCount; i++)
    {
        float currentAngle = angleIncrement * i;
        float x = radius * cos(currentAngle);
        float y = radius * sin(currentAngle);
        float z = 0.0f;
        temp.push_back(glm::vec3(x, y, z));
    }

    // Add vertices for the edges of the slice
    temp.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); // Center of the pizza slice
    temp.push_back(temp[1]); // First vertex on the circular part of the slice
    temp.push_back(temp[vCount]); // Last vertex on the circular part of the slice

    // Create triangles to form the slice
    for (size_t i = 0; i < temp.size() - 2; ++i)
    {
        vertices.push_back(temp[0]); // Center of the pizza slice
        vertices.push_back(temp[i + 1]); // Vertex forming the slice edge
        vertices.push_back(temp[i + 2]); // Next vertex forming the slice edge
    }
}


void buildPentagon(float radius)
{
    float angle = 360.0f / 5; // Divide 360 degrees by the number of sides (5 for a pentagon)
    std::vector<glm::vec3> temp;

    for (int i = 0; i < 5; i++) // Loop for the number of sides in the pentagon
    {
        float currentAngle = angle * i;
        float x = radius * cos(glm::radians(currentAngle));
        float y = radius * sin(glm::radians(currentAngle));
        float z = 0.0f;

        temp.push_back(glm::vec3(x, y, z));
    }

    // Create triangles connecting vertices to form the pentagon
    for (int i = 1; i <= 3; i++)
    {
        vertices.push_back(temp[0]);
        vertices.push_back(temp[i]);
        vertices.push_back(temp[i + 1]);
    }

    // Connect last vertex to first and second vertices to close the pentagon
    vertices.push_back(temp[0]);
    vertices.push_back(temp[4]);
    vertices.push_back(temp[1]);
}

int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Shapes", NULL, NULL);

    if (window == NULL)
    {
        cout << "Error. Unable to create a window!" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    Shader shader("../shaders/helloTriangle.vs", "../shaders/helloTriangle.fs");

    //buildPentagon(0.3f);
    buildPizzaSlice(0.5f, 64, 60.0f); // Radius of 0.5, 64 vertices, and a slice angle of 60 degrees


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

        // Draw pentagon
        shader.Use();
        glBindVertexArray(vertexArray);
        GLint colorLoc = glGetUniformLocation(shader.ID, "circleColor");
   //     glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f); // Red color
      //  glDrawArrays(GL_TRIANGLES, 0, 9);

        // Draw pizza slice
        glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f); // Yellow color
        glDrawArrays(GL_TRIANGLES, 9, vertices.size() - 9);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glfwTerminate();
    return 0;
}
