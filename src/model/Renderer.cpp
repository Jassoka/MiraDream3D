//
// Created by jassoka on 6/1/26.
//

#include "model/Renderer.h"

const std::string VIEWPORT_SOLID = "viewport_solid";
const std::string VIEWPORT_WIREFRAME = "viewport_wireframe";

void Renderer::draw()
{
    // On choisit le programme du vertex shader
    const std::string programName = VIEWPORT_SOLID;
    const GLuint programID = ShaderManager::getShaderProgram(programName);
    mGlFuncs->glUseProgram(programID);
    // Arguments de la caméra
    const int viewMatrix= mGlFuncs->glGetUniformLocation(programID, "viewMatrix");
    mGlFuncs->glUniformMatrix4fv (viewMatrix, 1, GL_FALSE, &mEngineCamera.computeViewMatrix()[0][0]);

    const int projMatrix= mGlFuncs->glGetUniformLocation(programID, "projMatrix");
    mGlFuncs->glUniformMatrix4fv (projMatrix, 1, GL_FALSE, &mEngineCamera.computePerspectiveMatrix()[0][0]);

    const int cameraPos= mGlFuncs->glGetUniformLocation(programID, "cameraPos");
    const glm::vec3 cameraVec =  mEngineCamera.getPosition();
    mGlFuncs->glUniform3f (cameraPos, cameraVec.x,cameraVec.y,cameraVec.z );

    const int lightPos = mGlFuncs->glGetUniformLocation(programID, "lightPos");
    const glm::vec3 lightVec =  mEngineCamera.getPosition();
    mGlFuncs->glUniform3f (lightPos,lightVec.x,lightVec.y,lightVec.z);

    mVAO.bind();
    mVBO.bind();
    mEBO.bind();
    glDrawElements(GL_LINES, numEdges*2, GL_UNSIGNED_INT, nullptr);
    mVAO.release();
}
void Renderer::geometryRedraw()
{
    mVAO.bind();
    mVBO.bind();

    auto firstMesh = mScene->getMeshes()[0]; //TODO faire une vraie boucle mdr
    // Buffer de vertices
    const auto vertices = firstMesh.getVertices();
    const Vertex *vertices_data = vertices.data(); // Pointeur vers les vertices

    mVBO.allocate(vertices_data,vertices.size() * sizeof(Vertex));

    // Buffer des faces //TODO rajouter algo de triangulation
    const auto faces = firstMesh.getFaces();
    std::vector<uint32_t> faceIndices;
    for (const auto& face: faces)
    {
        faceIndices.push_back(face[0]);
        faceIndices.push_back(face[1]);
        faceIndices.push_back(face[2]);
    }
    const uint32_t *faces_data = faceIndices.data();
    numTriangles = faceIndices.size();

    //Buffer des edges
    firstMesh.generateEdges();
    const auto edges = firstMesh.getEdges();
    std::vector<uint32_t> edgeIndices;
    for (const auto& [origin, end]: edges)
    {
        edgeIndices.push_back(origin);
        edgeIndices.push_back(end);
        std::cout << origin << " " << end;
        std::cout << '\n';
    }
    const uint32_t *edges_data = edgeIndices.data();
    numEdges = edgeIndices.size();

    mEBO.bind();
    mEBO.allocate(edges_data,numEdges * sizeof(uint32_t));


    draw();
    mVAO.release();

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
