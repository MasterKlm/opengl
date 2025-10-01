#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 textureCord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 fragPos;
out vec3 Normal;
out vec2 v_textureCord;

void main()
{   
    fragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model)))*aNormal;
    gl_Position = projection * view * vec4(fragPos, 1.0);
    v_textureCord = textureCord;
}

#shader fragment
#version 330 core

out vec4 FragColor;
uniform vec3 lightColor;


void main(){
    FragColor = vec4(lightColor, 1.0);
}