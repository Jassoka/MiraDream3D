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

            glDrawElements(GL_TRIANGLES, numTriangles, GL_UNSIGNED_INT, nullptr);
            break;
        }
    case ViewportMode::WIREFRAME:
        {

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
            glDrawElements(GL_LINES, numEdges, GL_UNSIGNED_INT, nullptr);
            break;
        }
    }
    mVAO.release();
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

    for (const Mesh &mesh : mScene->getMeshes() )
    {
        // Buffer de vertices
        const auto vertices = mesh.getVertices();
        const Vertex *vertices_data = vertices.data(); // Pointeur vers les vertices

        mVBO.allocate(vertices_data,vertices.size() * sizeof(Vertex));

        if (m == ViewportMode::SOLID)
        {
            // Buffer des faces //TODO rajouter algo de triangulation
            //mesh.triangulate(); //TODO le faire ailleurs mais pas dans le draw
            const auto triangles = mesh.getTriangles();
            std::vector<uint32_t> triangleIndices;
            for (const auto& t: triangles)
            {
                triangleIndices.push_back(t[0]);
                triangleIndices.push_back(t[1]);
                triangleIndices.push_back(t[2]);
            }
            const uint32_t *trig_data = triangleIndices.data();
            numTriangles = triangleIndices.size();

            mEBO.bind();
            mEBO.allocate(trig_data,numTriangles * sizeof(uint32_t));
        }
        else if (m == ViewportMode::WIREFRAME )
        {
            //Buffer des edges
            //mesh.generateEdges(); //TODO ça aussi
            const auto& edges = mesh.getEdges();
            std::vector<uint32_t> edgeIndices;
            for (const auto& [origin, end]: edges)
            {
                edgeIndices.push_back(origin);
                edgeIndices.push_back(end);
            }
            const uint32_t *edges_data = edgeIndices.data();
            numEdges = edgeIndices.size();

            mEBO.bind();
            mEBO.allocate(edges_data,numEdges * sizeof(uint32_t));
        }
    }
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

}

void Renderer::initialize(QOpenGLFunctions* glFuncs)
{
    mGlFuncs = glFuncs;
    ShaderManager::initialize(glFuncs);

    if (!mVAO.create()) exit(1);
    if (!mVBO.create()) exit(1);
    if (!mEBO.create()) exit(1);

    mVAO.bind();
    mVBO.bind();
    // 0: position du vertex
    mGlFuncs->glEnableVertexAttribArray(0);
    mGlFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

    // 1: normales du vertex
    mGlFuncs->glEnableVertexAttribArray(1);
    mGlFuncs->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));

    // 2: uv du vertex
    mGlFuncs->glEnableVertexAttribArray(2);
    mGlFuncs->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

    mEBO.bind();
    mVAO.release();

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
