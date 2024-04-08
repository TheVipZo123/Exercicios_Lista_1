#version 330 core
out vec4 FragColor;

uniform vec3 circleColor; // Uniform variable for circle color

void main()
{
    FragColor = vec4(circleColor, 8.0);
}
