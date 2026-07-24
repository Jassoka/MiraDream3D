#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// 0: wireframe, 1: solid, 2: material, 3: test_half_edges

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float scale;

#if RENDER_MODE == 1 || RENDER_MODE == 2
out vec3 n, v, l;

uniform vec3 lightPos;
uniform vec3 cameraPos;
#if RENDER_MODE == 2
out vec2 uvTexCoords;
#endif
#endif

#if RENDER_MODE == 3
uniform vec3 halfEdgeOrigin;
uniform vec3 halfEdgeEnd;

out vec3 color;
#endif

#if RENDER_MODE == 1 || RENDER_MODE == 2
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
    #if RENDER_MODE == 1 || RENDER_MODE == 2
    n = computeN();
    v = computeV(viewPos);
    l = computeL(viewPos);
    #endif

    #if RENDER_MODE == 2
    uvTexCoords = texCoords;
    #endif

    #if RENDER_MODE == 3
    if (halfEdgeEnd==position){
        color=vec3(0.0,0.0,1.0);
    }
    else if (halfEdgeOrigin==position){
        color=vec3(1.0,0.0,0.0);
    }
    else color=vec3(1.0,1.0,1.0)/5;
    #endif

    gl_Position = projMatrix * viewPos;
}