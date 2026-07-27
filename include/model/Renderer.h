//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_RENDERER_H
#define MIRADREAM3D_RENDERER_H
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <qopenglframebufferobject.h>

#include "Scene.h"
#include "types.h"

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

    void setSelectionMode(const SelectionMode mode)
    {
        mCurrentSelectionMode = mode;
    }

    /**
     * @getter{\ref mEngineCamera}
     */
    Camera *getEngineCamera() const { return mEngineCamera; }

    /** @brief Initialization for OpenGL */
    void initialize(QOpenGLFunctions* glFuncs);
    /** @brief Resizes the engine camera aspect ratio */
    void resize(int width, int height);

    /** @brief Draw function for cases when vertices or faces are added/ deleted */
    void geometryRedraw(ViewportMode mode);

    /** @brief Draw function for cases when vertices are moved */
    //void topologyRedraw();
    /** @brief Draw function without calculating new topology */
    void draw(ViewportMode mode);

    /** @brief Initializes all shaders in GPU memory */
    static void initShaders();

    /** @brief Reads the picking buffer at pixel (x, y) (top-left origin) */
    int32_t readPickingBuffer(int x, int y);

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

    /** @brief Draws the viewport vertices as squares */
    void drawPoints();


    /** @brief Draws the viewport edges as lines */
    void drawLines();

    /**
     * @brief Sets all the input arguments for the shader based of viewport mode
     */
    template <ViewportMode m>
    void setShaderArguments(GLuint programID);

    template <ViewportMode m>
    void drawForMode(GLuint programID);

    /**
     * @brief Binds all triangle indices to the render VBO
     */
    void buildFaceBuffer(const std::vector<Mesh> &meshes);
    /**
     * @brief Binds all edge indices to the geometric VBO
     */
    void buildEdgeBuffer(const std::vector<Mesh> &meshes);

    void initPickingBuffer();

    /**
     * @brief Builds the picking buffer, assumes the VAO and vertex buffers are up to date
     */
    void updatePickingBuffer();


    void resizePickingBuffer();

    /**
     * @brief Camera used to render the software's viewport
     */
    Camera *mEngineCamera = initEngineCamera();
    ShaderManager *mShaderManager;
    TextureManager *mTextureManager;
    const Scene *mScene = nullptr;
    QOpenGLFunctions *mGlFuncs = nullptr;
    /**
     * @brief OpenGL Vertex Array Object (for render)
     * For storing how to read vertex data from a CPU vertex objet
     */
    QOpenGLVertexArrayObject mRenderVAO;
    /**
     * @brief OpenGL Vertex Buffer Object (for render)
     * For storing all vertices as a contiguous memory block
     */
    QOpenGLBuffer mRenderVBO;
    /**
     * @brief OpenGL Element Buffer Object (for render)
     * For storing elements indices such as faces or edges
     */
    QOpenGLBuffer mRenderEBO{QOpenGLBuffer::IndexBuffer};
    /** @brief OpenGL Vertex Array Object (for topology) */
    QOpenGLVertexArrayObject mGeometricVAO;
    /** @brief OpenGL Vertex Buffer Object (for topology) */
    QOpenGLBuffer mGeometricVBO;
    /** @brief OpenGL Element Buffer Object  (for topology) */
    QOpenGLBuffer mGeometricEBO{QOpenGLBuffer::IndexBuffer};

    /** @brief OpenGL Vertex Buffer Object for selection highlight */
    QOpenGLBuffer mSelectionVBO;

    /** @brief OpenGL Vertex Array Object for the 2D grid */
    QOpenGLVertexArrayObject mGridVAO;

    /** @brief Frame buffer for picking */
    GLuint mPickingFBO = 0;

    /** @brief Texture slot bound to the picking FBO */
    GLuint mPickingTexture = 0;
    GLuint mDepthBuffer = 0;

    /**
     * @brief Selected object (can be vertex, edge, face, or mesh)
     */
    int32_t mSelection = 0;
    SelectionMode mCurrentSelectionMode = SelectionMode::NONE;
    /**
     * @brief Number of edge indices
     */
    uint32_t mNbEdgeIndices = 0;

    /**
     * @brief Number of face indices
     */
    uint32_t mNbFaceIndices = 0;
    /** @brief Number of existing geometric vertices */
    uint32_t nVertices = 0;
    int mWidth = 1;
    int mHeight = 1;
#ifdef TEST_HALFEDGES
    uint32_t mTestHalfEdge=0;
    uint32_t mTestMesh=0;
    uint32_t mTestComponent=0;
#endif
};
#endif //MIRADREAM3D_RENDERER_H
