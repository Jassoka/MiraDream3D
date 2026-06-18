//
// Created by jassoka on 6/1/26.
//

#include "model/Renderer.h"
#include "types.h"
#include "defs.h"
#include "glm/trigonometric.hpp"

#include "model/Mesh.h"
#include "model/Camera.h"
#include "model/Scene.h"
#include "model/ShaderManager.h"

#include <iostream>

const std::string VIEWPORT_SOLID = "viewport_solid";
const std::string VIEWPORT_WIREFRAME = "viewport_wireframe";
const std::string GRID = "grid";

static constexpr glm::vec3 worldOrigin {0.0f, 0.0f, 0.0f};
static constexpr glm::vec3 worldUp {0.0f, 0.0f, 1.0f};
static constexpr glm::vec3 defaultEngineCameraPosition {4.0f, 4.0f, 4.0f};
static constexpr float defaultEngineCameraFOV = glm::radians(45.0f);
static constexpr float defaultEngineCameraNearPlane = 0.1f;
static constexpr float defaultEngineCameraFarPlane = 100.0f;

Camera *Renderer::initEngineCamera()
{
    const glm::vec3 lookAt = glm::normalize(WORLD_ORIGIN - defaultEngineCameraPosition);
    const glm::vec3 right = glm::normalize(glm::cross(lookAt, worldUp));
    const glm::vec3 up = glm::normalize(glm::cross(right, lookAt));
    return new Camera (
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
void Renderer::drawTemplate()
{

    // On choisit le programme du vertex shader
    GLuint programID;
    switch (m)
    {
    case ViewportMode::SOLID:
            programID = ShaderManager::getShaderProgram(VIEWPORT_SOLID);
            break;
    case ViewportMode::WIREFRAME:

            programID = ShaderManager::getShaderProgram(VIEWPORT_WIREFRAME);
            #ifdef TEST_HALFEDGES
            programID = ShaderManager::getShaderProgram("viewport_test_halfedges");
            #endif
            break;
    default:
        programID = 0;
    }
    mGlFuncs->glUseProgram(programID);
    // Arguments de la caméra
    const int viewMatrix= mGlFuncs->glGetUniformLocation(programID, "viewMatrix");
    mGlFuncs->glUniformMatrix4fv (viewMatrix, 1, GL_FALSE, &mEngineCamera->computeViewMatrix()[0][0]);

    const int projMatrix= mGlFuncs->glGetUniformLocation(programID, "projMatrix");
    mGlFuncs->glUniformMatrix4fv (projMatrix, 1, GL_FALSE, &mEngineCamera->computePerspectiveMatrix()[0][0]);

    mVAO.bind();
    mVBO.bind();
    mEBO.bind();

    GLuint drawMode;
    switch (m)
    {
    case ViewportMode::SOLID:
        {
            const int cameraPos= mGlFuncs->glGetUniformLocation(programID, "cameraPos");
            const glm::vec3 cameraVec =  mEngineCamera->getPosition();
            mGlFuncs->glUniform3f (cameraPos, cameraVec.x,cameraVec.y,cameraVec.z );

            const int lightPos = mGlFuncs->glGetUniformLocation(programID, "lightPos");
            const glm::vec3 lightVec =  mEngineCamera->getPosition();
            mGlFuncs->glUniform3f (lightPos,lightVec.x,lightVec.y,lightVec.z);
            drawMode = GL_TRIANGLES;
            break;
        }
    case ViewportMode::WIREFRAME:
        {
            drawMode = GL_LINES;
            #ifdef TEST_HALFEDGES

            auto halfEdgesVect=mScene->getMeshes()[0].getHalfEdges();
            mTestHalfEdge%=halfEdgesVect.size();
            auto origin = mScene->getMeshes()[0].getVertices()[halfEdgesVect[mTestHalfEdge].origin] ;
            auto end = mScene->getMeshes()[0].getVertices()[halfEdgesVect[mTestHalfEdge].end];

            const int halfEdgeOrigin = mGlFuncs->glGetUniformLocation(programID, "halfEdgeOrigin");
            const int halfEdgeEnd = mGlFuncs->glGetUniformLocation(programID, "halfEdgeEnd");
            mGlFuncs->glUniform3f(halfEdgeOrigin,origin.x,origin.y,origin.z);
            mGlFuncs->glUniform3f(halfEdgeEnd,end.x,end.y,end.z);


            #endif

            break;
        }


    }
    glDrawElements(drawMode, nIndices, GL_UNSIGNED_INT, nullptr);
    mVAO.release();

    //on dessine d'abord la grid
    drawGrid();
}

void Renderer::draw(const ViewportMode mode)
{
    if (mode == ViewportMode::SOLID)
        drawTemplate<ViewportMode::SOLID>();

    if (mode == ViewportMode::WIREFRAME)
        drawTemplate<ViewportMode::WIREFRAME>();
}

template <ViewportMode m>
void Renderer::geometryRedrawTemplate()
{

    mVAO.bind();
    mVBO.bind();

    // Buffer de vertices
    std::vector<RenderVertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t indexOffset = 0;

    for (const Mesh &mesh : mScene->getMeshes() )
    {
        const auto meshVertices = mesh.getVertices();
        vertices.reserve(vertices.size() + meshVertices.size());
        vertices.insert(vertices.end(), meshVertices.begin(), meshVertices.end());

        if (m == ViewportMode::SOLID)
        {
            // Buffer des faces
            const auto triangles = mesh.getTriangles();
            indices.resize(indices.size() + triangles.size()*3);
            for (const auto& t: triangles)
            {
                indices.push_back(t[0] + indexOffset);
                indices.push_back(t[1] + indexOffset);
                indices.push_back(t[2] + indexOffset);
            }
        }
        else if (m == ViewportMode::WIREFRAME)
        {
            //Buffer des edges
            const auto edges = mesh.getEdges();
            indices.resize(indices.size() + edges.size()*2);
            for (const auto& [origin, end]: edges)
            {
                indices.push_back(origin + indexOffset);
                indices.push_back(end + indexOffset);
            }
        }
        indexOffset += meshVertices.size();
    }

    const RenderVertex *vertices_data = vertices.data(); // Pointeur vers les vertices
    mVBO.allocate(vertices_data,vertices.size() * sizeof(RenderVertex));

    const uint32_t *data = indices.data();
    nIndices = indices.size();

    mEBO.bind();
    mEBO.allocate(data,nIndices * sizeof(uint32_t));
    drawTemplate<m>();
    mVAO.release();
}

void Renderer::geometryRedraw(const ViewportMode mode)
{
    if (mode == ViewportMode::SOLID)
        geometryRedrawTemplate<ViewportMode::SOLID>();

    if (mode == ViewportMode::WIREFRAME)
        geometryRedrawTemplate<ViewportMode::WIREFRAME>();
}

void Renderer::initShaders()
{
    GLuint vertexShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/viewport_solid.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/viewport_solid.frag", GL_FRAGMENT_SHADER);
    std::vector<GLuint> shaders = {vertexShader, fragmentShader};
    ShaderManager::createProgram(VIEWPORT_SOLID, shaders);


    vertexShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/viewport_wireframe.vert", GL_VERTEX_SHADER);
    fragmentShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/viewport_wireframe.frag", GL_FRAGMENT_SHADER);
    shaders = {vertexShader, fragmentShader};
    ShaderManager::createProgram(VIEWPORT_WIREFRAME, shaders);

#ifdef TEST_HALFEDGES
    vertexShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/viewport_test_halfedges.vert", GL_VERTEX_SHADER);
    fragmentShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/viewport_test_halfedges.frag", GL_FRAGMENT_SHADER);
    shaders = {vertexShader, fragmentShader};
    ShaderManager::createProgram("viewport_test_halfedges", shaders);
#endif


    vertexShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/grid.vert", GL_VERTEX_SHADER);
    fragmentShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/grid.frag", GL_FRAGMENT_SHADER);
    shaders = {vertexShader, fragmentShader};
    ShaderManager::createProgram(GRID, shaders);
}

void Renderer::initialize(QOpenGLFunctions* glFuncs)
{
    //3D
    mGlFuncs = glFuncs;
    ShaderManager::initialize(glFuncs);

    if (!mVAO.create()) exit(1);
    if (!mVBO.create()) exit(1);
    if (!mEBO.create()) exit(1);

    mVAO.bind();
    mVBO.bind();
    // 0: position du vertex
    mGlFuncs->glEnableVertexAttribArray(0);
    mGlFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (void*)offsetof(RenderVertex, x));

    // 1: normales du vertex
    mGlFuncs->glEnableVertexAttribArray(1);
    mGlFuncs->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (void*)offsetof(RenderVertex, nx));

    // 2: uv du vertex
    mGlFuncs->glEnableVertexAttribArray(2);
    mGlFuncs->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (void*)offsetof(RenderVertex, u));

    mEBO.bind();
    mVAO.release();


    glGenVertexArrays(1, &mGridVAO);

    initShaders();
}

void Renderer::resize(const int width, int height) const
{
    if (height == 0) height = 1;
    const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    mEngineCamera->setAspectRatio(aspectRatio);
}


template void Renderer::drawTemplate<ViewportMode::SOLID>();
template void Renderer::drawTemplate<ViewportMode::WIREFRAME>();




void Renderer::drawGrid() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint programID = ShaderManager::getShaderProgram(GRID);
    mGlFuncs->glUseProgram(programID);
    // Arguments de la caméra
    const int viewMatrix= mGlFuncs->glGetUniformLocation(programID, "viewMatrix");
    mGlFuncs->glUniformMatrix4fv (viewMatrix, 1, GL_FALSE, &mEngineCamera->computeViewMatrix()[0][0]);

    const int projMatrix= mGlFuncs->glGetUniformLocation(programID, "projMatrix");
    mGlFuncs->glUniformMatrix4fv (projMatrix, 1, GL_FALSE, &mEngineCamera->computePerspectiveMatrix()[0][0]);


    glBindVertexArray(mGridVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}