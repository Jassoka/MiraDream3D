Tessellation Shader: split n-quads en triangles
Geometry Shader: optionnel

### VAO pour créer une "config", qui dit au GPU où dans le Vertex struct se trouve quoi:
```
// "Prends les Positions et pousse-les dans le TUYAU 0"
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
glEnableVertexAttribArray(0);

// "Prends les Normales et pousse-les dans le TUYAU 1"
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));
glEnableVertexAttribArray(1);
```
-> positions slot 0, normales slot 1

Dans GLSL:
```
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
```