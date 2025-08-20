#version 330 core
layout (location = 0) in vec3 aPos;   // позиция
layout (location = 1) in vec3 aColor; // цвет

out vec3 Color;

uniform mat4 camMatrix; // твоя view * projection матрица

void main()
{
    gl_Position = camMatrix * vec4(aPos, 1.0);
    Color = aColor;
}
