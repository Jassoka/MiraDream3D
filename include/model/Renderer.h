//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_RENDERER_H
#define MIRADREAM3D_RENDERER_H
#include "Camera.h"
#include "Scene.h"


class Renderer
{
private:
    Camera EngineCamera;
    char *VertexShader = nullptr;
    char *FragmentShader = nullptr;
    Scene *scene = nullptr;
};
#endif //MIRADREAM3D_RENDERER_H
