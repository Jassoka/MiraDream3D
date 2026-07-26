#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// 0: points, 1: lines, 2: solid, 3: material

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float scale;

#if RENDER_MODE == 0
uniform int selectedVertex;
out vec3 color;
#endif
#if RENDER_MODE == 2 || RENDER_MODE == 3
out vec3 n, v, l;

uniform vec3 lightPos;
uniform vec3 cameraPos;
#if RENDER_MODE == 3
out vec2 uvTexCoords;
#endif
#endif

/*
#if RENDER_MODE == 1
uniform vec3 edgeOrigin;
uniform vec3 edgeEnd;

out vec3 color;
#endif
*/

#if RENDER_MODE == 2 || RENDER_MODE == 3
vec3 computeN() {
    mat3 normalMatrix = transpose(mat3(inverse(viewMatrix)));
    return normalMatrix * normal;
}

vec3 computeV(vec4 viewPos) {
    return -viewPos.xyz;
}

vec3 computeL(vec4 viewPos) {
    vec3 lightPosViewSpace = vec3(viewMatrix * vec4(lightPos, 1.0f));
    return lightPosViewSpace - viewPos.xyz;
}
#endif

void main() {
    vec4 viewPos = viewMatrix * vec4(position, 1.0f);
    #if RENDER_MODE == 2 || RENDER_MODE == 3
    n = computeN();
    v = computeV(viewPos);
    l = computeL(viewPos);
    #endif

    #if RENDER_MODE == 3
    uvTexCoords = texCoords;
    #endif

    #if RENDER_MODE == 0
    if (gl_VertexID == selectedVertex) {
        color = vec3(1.0, 0.0, 0.0); // Red if selected
    }
    else {
        color = vec3(0.0, 0.0, 0.0); // White normally
    }

    gl_PointSize = 10.0;
    #endif

    /*
    #if RENDER_MODE ==
    if (halfEdgeEnd==position){
        color=vec3(0.0,0.0,1.0);
    }
    else if (halfEdgeOrigin==position){
        color=vec3(1.0,0.0,0.0);
    }
    else color=vec3(0.0,0.0,0.0);
    #endif
*/

    gl_Position = projMatrix * viewPos;
}