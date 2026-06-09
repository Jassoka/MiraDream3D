//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_RENDERER_H
#define MIRADREAM3D_RENDERER_H

#include <QOpenGLFunctions>
#include "Camera.h"
#include "Scene.h"
#include "ShaderManager.h"

enum ViewportMode
{
    SOLID,
    WIREFRAME
};

class Renderer
{

public:
    Renderer() = default;

    void initialize(QOpenGLFunctions* glFuncs);

    void resize(int width, int height);

    void setScene(const Scene* scene)
    {
        mScene = scene;
    }

    Camera &getEngineCamera()
    {
        return mEngineCamera;
    }

    /**
     * @brief Draw function for cases when vertices or faces are added/ deleted
     */
    void geometryRedraw();


    /**
     * @brief Draw function for cases when vertices are moved
     */
    //void topologyRedraw();
    /**
     * @brief Draw function without calculating new topology
     */
    void draw();


    void changeMode(ViewportMode mode) {
        this->mCurrViewportMode=mode;
        hasGeometryChange=true;
    }

    void initShaders();

    bool hasGeometryChange = true;
    bool hasTopologyChange = false; //TODO faire des vraies méthodes pour changer les flags
#ifdef TEST_HALFEDGES
    void addTestHalfEdge(int32_t i) {
        mTestHalfEdge+=i;
    };
#endif


private:
    static constexpr glm::vec3 worldOrigin {0.0f, 0.0f, 0.0f};
    static constexpr glm::vec3 worldUp {0.0f, 0.0f, 1.0f};
    static constexpr glm::vec3 defaultEngineCameraPosition {4.0f, 4.0f, 4.0f};
    static constexpr float defaultEngineCameraFOV = glm::radians(45.0f);
    static constexpr float defaultEngineCameraNearPlane = 0.1f;
    static constexpr float defaultEngineCameraFarPlane = 100.0f;

    static Camera initEngineCamera()
    {
        const glm::vec3 lookAt = glm::normalize(worldOrigin - defaultEngineCameraPosition);
        const glm::vec3 right = glm::normalize(glm::cross(lookAt, worldUp));
        const glm::vec3 up = glm::normalize(glm::cross(right, lookAt));
        return Camera (
            up,
            right,
            lookAt,
            defaultEngineCameraPosition,
            defaultEngineCameraFOV,
            defaultEngineCameraNearPlane,
            defaultEngineCameraFarPlane,
            1.0
        );
    }
    template <ViewportMode m>
    void drawTemplate();

    template <ViewportMode m>
    void geometryRedrawTemplate();

    Camera mEngineCamera = initEngineCamera();
    ShaderManager mShaderManager;
    const Scene *mScene = nullptr;
    QOpenGLFunctions *mGlFuncs = nullptr;
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVBO;
    QOpenGLBuffer mEBO{QOpenGLBuffer::IndexBuffer};
    uint32_t numTriangles = 0;
    uint32_t numEdges = 0;
    ViewportMode mCurrViewportMode = ViewportMode::SOLID;
#ifdef TEST_HALFEDGES
    uint32_t mTestHalfEdge=0;
#endif
};
#endif //MIRADREAM3D_RENDERER_H
