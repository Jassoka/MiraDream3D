#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

//uniform vect3 lightPosition;

void main(){
    vec4 extendedPos = vec4(position,1.0f);
    vec4 viewPos = vec4(0.0f);
    viewPos = viewMatrix * extendedPos;
    gl_Position = projMatrix * viewPos;
}
