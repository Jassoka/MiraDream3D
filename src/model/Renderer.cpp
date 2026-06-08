//
// Created by jassoka on 6/1/26.
//

#include "model/Renderer.h"

const std::string VIEWPORT_SOLID = "viewport_solid";
const std::string VIEWPORT_WIREFRAME = "viewport_wireframe";


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
            break;
    default:
        programID = 0;
    }
    mGlFuncs->glUseProgram(programID);
    // Arguments de la caméra
    const int viewMatrix= mGlFuncs->glGetUniformLocation(programID, "viewMatrix");
    mGlFuncs->glUniformMatrix4fv (viewMatrix, 1, GL_FALSE, &mEngineCamera.computeViewMatrix()[0][0]);

    const int projMatrix= mGlFuncs->glGetUniformLocation(programID, "projMatrix");
    mGlFuncs->glUniformMatrix4fv (projMatrix, 1, GL_FALSE, &mEngineCamera.computePerspectiveMatrix()[0][0]);

    mVAO.bind();
    mVBO.bind();
    mEBO.bind();

    switch (m)
    {
    case ViewportMode::SOLID:
        {
            const int cameraPos= mGlFuncs->glGetUniformLocation(programID, "cameraPos");
            const glm::vec3 cameraVec =  mEngineCamera.getPosition();
            mGlFuncs->glUniform3f (cameraPos, cameraVec.x,cameraVec.y,cameraVec.z );

            const int lightPos = mGlFuncs->glGetUniformLocation(programID, "lightPos");
            const glm::vec3 lightVec =  mEngineCamera.getPosition();
            mGlFuncs->glUniform3f (lightPos,lightVec.x,lightVec.y,lightVec.z);

            glDrawElements(GL_TRIANGLES, numTriangles, GL_UNSIGNED_INT, nullptr);
            break;
        }
    case ViewportMode::WIREFRAME:
        {
            glDrawElements(GL_LINES, numEdges, GL_UNSIGNED_INT, nullptr);
            break;
        }
    }
    mVAO.release();
}

void Renderer::draw()
{
    if (mCurrViewportMode == SOLID)
        drawTemplate<SOLID>();

    if (mCurrViewportMode == WIREFRAME)
        drawTemplate<WIREFRAME>();
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
        else if (m == ViewportMode::WIREFRAME)
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
    draw();
    mVAO.release();

}

void Renderer::geometryRedraw()
{
    if (mCurrViewportMode == SOLID)
        geometryRedrawTemplate<SOLID>();

    if (mCurrViewportMode == WIREFRAME)
        geometryRedrawTemplate<WIREFRAME>();
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
}

void Renderer::initialize(QOpenGLFunctions* glFuncs)
{
    mGlFuncs = glFuncs;
    ShaderManager::initialize(glFuncs);
    mEBO = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

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

void Renderer::resize(int width, int height)
{
    if (height == 0) height = 1;
    const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    mEngineCamera.setAspectRatio(aspectRatio);
}
template void Renderer::drawTemplate<ViewportMode::SOLID>();
template void Renderer::drawTemplate<ViewportMode::WIREFRAME>();
