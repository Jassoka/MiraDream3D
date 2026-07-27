#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

flat out int v_VertexID;

void main() {
    gl_PointSize = 30.0;
    v_VertexID = gl_VertexID;
    vec4 viewPos = viewMatrix * vec4(position, 1.0f);
    gl_Position = projMatrix * viewPos;
}