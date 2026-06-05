//
// Created by jassoka on 6/1/26.
//

#include "model/Renderer.h"
#include "util/QTResourceManager.hpp"

Renderer::Renderer():
mEngineCamera(Camera (
        defaultEngineCameraUp,
        defaultEngineCameraRight,
        defaultEngineCameraLookAt,
        defaultEngineCameraPosition,
        defaultEngineCameraFOV,
        defaultEngineCameraNearPlane,
        defaultEngineCameraFarPlane,
        1.0
    ))
{
}

void Renderer::render()
{
    mVBO.bind();

    const auto firstMesh = mScene->getMeshes()[0]; //TODO faire une vraie boucle mdr
    // Buffer de vertices
    const auto vertices = firstMesh.getVertices();
    const Vertex *vertices_data = vertices.data(); // Pointeur vers les vertices

    mVBO.allocate(vertices_data,vertices.size() * sizeof(Vertex));


    // Buffer des faces //TODO rajouter algo de triangulation
    const auto faces = firstMesh.getFaces();
    std::vector<uint32_t> faceIndices;
    for (const auto face: faces)
    {
        faceIndices.push_back(face[0]);
        faceIndices.push_back(face[1]);
        faceIndices.push_back(face[2]);
    }
    const uint32_t *faces_data = faceIndices.data();
    mEBO.bind();
    mEBO.allocate(faces_data,faceIndices.size() * sizeof(uint32_t));

    // On choisit le programme du vertex shader
    const std::string programName = "Standard";
    const GLuint programID = mShaderManager.getShaderProgram(programName);
    mGlFuncs->glUseProgram(programID);

    // Arguments de la caméra
    const int viewMatrix= mGlFuncs->glGetUniformLocation(programID, "viewMatrix");
    mGlFuncs->glUniformMatrix4fv (viewMatrix, 1, GL_FALSE, &mEngineCamera.computeViewMatrix()[0][0]);

    glDrawElements(GL_TRIANGLES, faceIndices.size(), GL_UNSIGNED_INT, nullptr);
}

Renderer::~Renderer()
{
    delete mScene;
}

void Renderer::initShaders()
{
    std::string const VertexShaderCode = QTResourceManager::readEmbeddedRessource(":/assets/shaders/standard.vert");
    const GLuint vertexShader = ShaderManager::compileShader(VertexShaderCode, GL_VERTEX_SHADER, this->mGlFuncs);
    std::string const FragmentShaderCode = QTResourceManager::readEmbeddedRessource(":/assets/shaders/standard.frag");
    const GLuint fragmentShader = ShaderManager::compileShader(FragmentShaderCode, GL_FRAGMENT_SHADER, this->mGlFuncs);
    std::vector<GLuint> shaders;
    shaders.push_back(vertexShader);
    shaders.push_back(fragmentShader);

    std::string const programName = "Standard";
    mShaderManager.createProgram(programName, shaders, this->mGlFuncs);

}

void Renderer::initialize(QOpenGLFunctions* glFuncs)
{
    mGlFuncs = glFuncs;

    if (!mVAO.create()) exit(1);
    if (!mVBO.create()) exit(1);

    mVAO.bind();
    // 0: position du vertex
    mGlFuncs->glEnableVertexAttribArray(0);
    mGlFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    mVAO.release();

    initShaders();
}

void Renderer::resize(int width, int height)
{
    if (height == 0) height = 1;
    const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    mEngineCamera.setAspectRatio(aspectRatio);
}
