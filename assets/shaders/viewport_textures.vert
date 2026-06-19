#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

//out vec3 n, v, l;

uniform vec3 lightPos;
uniform vec3 cameraPos;

void main(){
    vec4 viewPos = viewMatrix * vec4(position, 1.0f);

    mat3 normalMatrix = transpose(mat3(inverse(viewMatrix)));
    //n = normalMatrix * normal;
    //v = -viewPos.xyz;
    vec3 lightPosViewSpace = vec3(viewMatrix * vec4(lightPos, 1.0f));
    //l = lightPosViewSpace - viewPos.xyz;

    gl_Position = projMatrix * viewPos;

}
