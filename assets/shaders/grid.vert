#version 330



// Triangle plein écran généré sans aucun VBO
vec2 positions[3] = vec2[3](
vec2(-1.0, -1.0),
vec2( 3.0, -1.0),
vec2(-1.0,  3.0)
);
out vec3 nearUnProj;
out vec3 farUnProj;



uniform mat4 viewMatrix;
uniform mat4 projMatrix;



void main() {
    vec2 pos2D = positions[gl_VertexID];

    mat4 viewInv = inverse(viewMatrix);
    mat4 projInv = inverse(projMatrix);

    vec4  farUnProj4D = viewInv * projInv * vec4(pos2D,1.0,1.0);
    farUnProj=farUnProj4D.xyz/farUnProj4D.w;
    vec4  nearUnProj4D = viewInv * projInv * vec4(pos2D,0.0,1.0);
    nearUnProj=nearUnProj4D.xyz/nearUnProj4D.w;


    gl_Position = vec4(pos2D,0.0,1.0);
}