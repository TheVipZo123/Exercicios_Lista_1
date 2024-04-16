#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#define PI 3.14159265358979323846

void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINES);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * PI * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void drawStickFigure() {
    // Head
    drawCircle(0.0f, 0.6f, 0.2f, 20);

    // Body
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.4f);
    glVertex2f(0.0f, -0.5f);
    glEnd();

    // Arms
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.2f);
    glVertex2f(0.3f, 0.0f);
    glVertex2f(0.0f, 0.2f);
    glVertex2f(-0.3f, 0.0f);
    glEnd();

    // Legs
    glBegin(GL_LINES);
    glVertex2f(0.0f, -0.5f);
    glVertex2f(0.2f, -1.0f);
    glVertex2f(0.0f, -0.5f);
    glVertex2f(-0.2f, -1.0f);
    glEnd();
}

int main() {
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(800, 600, "Stick Figure", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the stick figure
        drawStickFigure();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
