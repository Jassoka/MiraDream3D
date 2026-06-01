//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_RENDERER_H
#define MIRADREAM3D_RENDERER_H

#include <QOpenGLFunctions>
#include "Camera.h"
#include "Scene.h"
#include "ShaderManager.h"


class Renderer
{
private:
    static constexpr glm::vec3 defaultEngineCameraUp {1, 1, 1}; //TODO: mettre des vraies valeurs
    static constexpr glm::vec3 defaultEngineCameraRight {1, 1, 1};
    static constexpr glm::vec3 defaultEngineCameraLookAt {1, 1, 1};
    static constexpr glm::vec3 defaultEngineCameraPosition {1, 1, 1};
    static constexpr float defaultEngineCameraFOV = glm::radians(45.0f);
    static constexpr float defaultEngineCameraNearPlane = 0.1f;
    static constexpr float defaultEngineCameraFarPlane = 100.0f;

    Camera mEngineCamera;
    ShaderManager mShaderManager;
    Scene *mScene = nullptr;
    QOpenGLFunctions *mGlFuncs = nullptr;
public:
    Renderer(float initialAspectRatio, QOpenGLFunctions *glFuncs);
    ~Renderer();

    void initShaders();
};
#endif //MIRADREAM3D_RENDERER_H
