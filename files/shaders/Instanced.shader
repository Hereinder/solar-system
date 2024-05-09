#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 aInstanceMatrix;

uniform mat4 u_Model;

void main()
{
    gl_Position = u_Model * aInstanceMatrix * vec4(aPos, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
void main()
{
    color = vec4(1.0, 0.0, 0.0, 1.0);
};