#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform vec3 halfEdgeOrigin;
uniform vec3 halfEdgeEnd;

out vec3 color;
void main(){
    vec4 viewPos = viewMatrix * vec4(position, 1.0f);
    gl_Position = projMatrix * viewPos;
    if (halfEdgeEnd==position){
        color=vec3(0.0,0.0,1.0);
    }
    else if (halfEdgeOrigin==position){
        color=vec3(1.0,0.0,0.0);
    }
    else color=vec3(1.0,1.0,1.0)/5;
}
