//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_RENDERER_H
#define MIRADREAM3D_RENDERER_H
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "Scene.h"

class TextureManager;
class Scene;
class Camera;
class ShaderManager;
class QOpenGLFunctions;

enum class ViewportMode;

/**
 * @brief Class for 3D rendering
 */
class Renderer
{
public:
    Renderer() = default;

    void setScene(const Scene* scene) { mScene = scene; }

    /**
     * @getter{\ref mEngineCamera}
     */
    Camera *getEngineCamera() const { return mEngineCamera; }

    /** @brief Initialization for OpenGL */
    void initialize(QOpenGLFunctions* glFuncs);
    /** @brief Resizes the engine camera aspect ratio */
    void resize(int width, int height) const;

    /** @brief Draw function for cases when vertices or faces are added/ deleted */
    void geometryRedraw(ViewportMode mode);

    /** @brief Draw function for cases when vertices are moved */
    //void topologyRedraw();
    /** @brief Draw function without calculating new topology */
    void draw(ViewportMode mode);

    /** @brief Initializes all shaders in GPU memory */
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
    /** @brief Returns an engine camera pointer with default settings */
    static Camera *initEngineCamera();

    template <ViewportMode m>
    void drawTemplate();

    template <ViewportMode m>
    void geometryRedrawTemplate();

    /** @brief Draws the viewport grid 2d */
    void drawGrid();

    /**
     * @brief Camera used to render the software's viewport
     */
    Camera *mEngineCamera = initEngineCamera();
    ShaderManager *mShaderManager;
    TextureManager *mTextureManager;
    const Scene *mScene = nullptr;
    QOpenGLFunctions *mGlFuncs = nullptr;
    /**
     * @brief OpenGL Vertex Array Object
     * For storing how to read vertex data from a CPU vertex objet
     */
    QOpenGLVertexArrayObject mVAO;
    /**
     * @brief OpenGL Vertex Buffer Object
     * For storing all vertices as a contiguous memory block
     */
    QOpenGLBuffer mVBO;
    /**
     * @brief OpenGL Element Buffer Object
     * For storing elements indices such as faces or edges
     */
    QOpenGLBuffer mEBO{QOpenGLBuffer::IndexBuffer};

    /** @brief OpenGL Vertex Array Object for the 2D grid */
    QOpenGLVertexArrayObject mGridVAO;

    /**
     * @brief Number of indices (face or edges depending on viewport mode)
     */
    uint32_t nIndices = 0;
#ifdef TEST_HALFEDGES
    uint32_t mTestHalfEdge=0;
    uint32_t mTestMesh=0;
    uint32_t mTestComponent=0;
#endif
};
#endif //MIRADREAM3D_RENDERER_H
