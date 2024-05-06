#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout (location = 2) in vec3 aNormal;


out vec3 FragPos;
out vec3 Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    FragPos = vec3(u_Model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;  
    
    gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 u_ViewPos;
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_Color;

void main()
{
    // ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * u_LightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = specularStrength * spec * u_LightColor;  


    vec3 result = (ambient + diffuse + specular) * u_Color;
    color = vec4(result, 1.0);
}