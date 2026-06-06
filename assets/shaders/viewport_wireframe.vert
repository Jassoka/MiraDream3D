#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main(){
    vec4 viewPos = viewMatrix * vec4(position, 1.0f);
    gl_Position = projMatrix * viewPos;

}
