#version 330 core
layout(location = 0) out uint fragID;


flat in int v_VertexID;

void main() {
    fragID = uint(v_VertexID + 1);
}