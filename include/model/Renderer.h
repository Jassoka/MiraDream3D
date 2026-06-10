//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_RENDERER_H
#define MIRADREAM3D_RENDERER_H
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

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
    void addTestHalfEdge(int32_t i) {
        mTestHalfEdge+=i;
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
#endif
};
#endif //MIRADREAM3D_RENDERER_H
