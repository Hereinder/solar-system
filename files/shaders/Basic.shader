#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
uniform mat4 transform;
void main()
{
    gl_Position =  position;
};

#shader fragment
#version 330 core

uniform vec4 u_Color;

layout(location = 0) out vec4 color;
void main()
{
    color = u_Color;
};