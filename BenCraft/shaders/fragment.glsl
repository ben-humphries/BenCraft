#version 330 core
out vec4 FragColor;

in float color;

void main()
{
    FragColor = vec4(color / 255 / 3, color / 255, color / 255 / 5, 1.0);
} 