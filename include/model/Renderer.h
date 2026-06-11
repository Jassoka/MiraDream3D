//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_RENDERER_H
#define MIRADREAM3D_RENDERER_H
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "Scene.h"

class Scene;
class Camera;
class ShaderManager;
class QOpenGLFunctions;

enum class ViewportMode;

class Renderer
{
public:
    Renderer() = default;

    void setScene(const Scene* scene)
    {
        mScene = scene;
    }

    Camera *getEngineCamera() const
    {
        return mEngineCamera;
    }

    void initialize(QOpenGLFunctions* glFuncs);
    void resize(int width, int height) const;

    /**
     * @brief Draw function for cases when vertices or faces are added/ deleted
     */
    void geometryRedraw(ViewportMode mode);

    /**
     * @brief Draw function for cases when vertices are moved
     */
    //void topologyRedraw();
    /**
     * @brief Draw function without calculating new topology
     */
    void draw(ViewportMode mode);

    void initShaders();

#ifdef TEST_HALFEDGES
    void addTestHalfEdge(const int32_t i) {
        const Mesh &currMesh = mScene->getMeshes()[mTestMesh];
        if (i>0) mTestHalfEdge = currMesh.getNextHalfEdge(mTestHalfEdge);
        if (i<0) mTestHalfEdge = currMesh.getPrevHalfEdge(mTestHalfEdge);
        if (i==0)
            if (const int32_t twin = currMesh.getTwinHalfEdge(mTestHalfEdge); twin > -1)
                mTestHalfEdge = twin;
    };

    void nextTestMesh() {
        const uint32_t nMeshes = mScene->getMeshes().size();
        if (nMeshes == 0) return;
        mTestMesh = (mTestMesh+1)%nMeshes;
        mTestHalfEdge = 0;
    };


    void nextTestComponent() {
        const auto components = mScene->getMeshes()[mTestMesh].getComponents();
        const uint32_t nComponents = components.size();
        mTestHalfEdge = components[(++mTestComponent)%nComponents];
    };
#endif

private:
    static Camera *initEngineCamera();

    template <ViewportMode m>
    void drawTemplate();

    template <ViewportMode m>
    void geometryRedrawTemplate();

    Camera *mEngineCamera = initEngineCamera();
    ShaderManager *mShaderManager;
    const Scene *mScene = nullptr;
    QOpenGLFunctions *mGlFuncs = nullptr;
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVBO;
    QOpenGLBuffer mEBO{QOpenGLBuffer::IndexBuffer};
    uint32_t nIndices = 0;
#ifdef TEST_HALFEDGES
    uint32_t mTestHalfEdge=0;
    uint32_t mTestMesh=0;
    uint32_t mTestComponent=0;
#endif
};
#endif //MIRADREAM3D_RENDERER_H
