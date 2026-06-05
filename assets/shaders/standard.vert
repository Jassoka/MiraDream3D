#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;


out vec3 fragNormal;
out vec4 viewPos;

uniform vec3 lightPos;
uniform vec3 cameraPos;

void main(){
    fragNormal  =  normal;




    vec4 extendedPos = vec4(position,1.0f);
    viewPos = vec4(0.0f);
    viewPos = viewMatrix * extendedPos;
    gl_Position = projMatrix * viewPos;
}
