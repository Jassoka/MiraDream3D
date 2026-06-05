//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_RENDERER_H
#define MIRADREAM3D_RENDERER_H

#include <QOpenGLFunctions>
#include "Camera.h"
#include "Scene.h"
#include "ShaderManager.h"

static constexpr float SQRT3_INV = 0.57735026919f;
static constexpr float SQRT2_INV = 0.70710678118f;
static constexpr float SQRT6_INV = 0.40824829046f;

class Renderer
{
public:
    Renderer();

    void initialize(QOpenGLFunctions* glFuncs);

    void resize(int width, int height);

    ~Renderer();

    void setScene(const Scene* scene)
    {
        mScene = scene;
    }

    void render();

    void initShaders();


private:
    static constexpr glm::vec3 defaultEngineCameraLookAt {-SQRT3_INV, -SQRT3_INV, -SQRT3_INV};
    static constexpr glm::vec3 defaultEngineCameraRight  { SQRT2_INV,  0.0f,      -SQRT2_INV};
    static constexpr glm::vec3 defaultEngineCameraUp     {-SQRT6_INV,  2.0f*SQRT6_INV, -SQRT6_INV};
    static constexpr glm::vec3 defaultEngineCameraPosition {5.0f, 5.0f, 5.0f};
    static constexpr float defaultEngineCameraFOV = glm::radians(45.0f);
    static constexpr float defaultEngineCameraNearPlane = 0.1f;
    static constexpr float defaultEngineCameraFarPlane = 100.0f;

    Camera mEngineCamera;
    ShaderManager mShaderManager;
    const Scene *mScene = nullptr;
    QOpenGLFunctions *mGlFuncs = nullptr;
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVBO;
    QOpenGLBuffer mEBO{QOpenGLBuffer::IndexBuffer};

    void VertexShader();
};
#endif //MIRADREAM3D_RENDERER_H
