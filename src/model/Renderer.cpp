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
#include <QOpenGLExtraFunctions>

#include "model/TextureManager.h"

const std::string VIEWPORT_SOLID = "viewport_solid";
const std::string VIEWPORT_MATERIAL = "viewport_material";
const std::string GRID = "grid";
const std::string LINES = "viewport_lines";
const std::string VERTICES = "viewport_vertices";
const std::string PICKING = "picking";

static constexpr glm::vec3 defaultEngineCameraPosition {4.0f, 4.0f, 4.0f};
static constexpr float defaultEngineCameraFOV = glm::radians(45.0f);
static constexpr float defaultEngineCameraNearPlane = 0.1f;
static constexpr float defaultEngineCameraFarPlane = 10000.0f;


Camera *Renderer::initEngineCamera()
{
    const glm::vec3 lookAt = glm::normalize(WORLD_ORIGIN - defaultEngineCameraPosition);
    const glm::vec3 right = glm::normalize(glm::cross(lookAt, WORLD_UP));
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
static GLuint getShaderForMode()
{
    if constexpr (m == ViewportMode::SOLID) return ShaderManager::getShaderProgram(VIEWPORT_SOLID);
    else if constexpr (m == ViewportMode::WIREFRAME) return ShaderManager::getShaderProgram(LINES);
    else if constexpr (m == ViewportMode::MATERIAL) return ShaderManager::getShaderProgram(VIEWPORT_MATERIAL);
}
template <ViewportMode m>
void Renderer::setShaderArguments(const GLuint programID)
{
    // Arguments de la caméra
    const int viewMatrix= mGlFuncs->glGetUniformLocation(programID, "viewMatrix");
    mGlFuncs->glUniformMatrix4fv (viewMatrix, 1, GL_FALSE, &mEngineCamera->computeViewMatrix()[0][0]);

    const int projMatrix= mGlFuncs->glGetUniformLocation(programID, "projMatrix");
    mGlFuncs->glUniformMatrix4fv (projMatrix, 1, GL_FALSE, &mEngineCamera->computePerspectiveMatrix()[0][0]);
    if constexpr (m == ViewportMode::SOLID || m == ViewportMode::MATERIAL)
    {
        const int cameraPos= mGlFuncs->glGetUniformLocation(programID, "cameraPos");
        const glm::vec3 cameraVec =  mEngineCamera->getPosition();
        mGlFuncs->glUniform3f (cameraPos, cameraVec.x,cameraVec.y,cameraVec.z );

        const int lightPos = mGlFuncs->glGetUniformLocation(programID, "lightPos");
        const glm::vec3 lightVec =  mEngineCamera->getPosition();
        mGlFuncs->glUniform3f (lightPos,lightVec.x,lightVec.y,lightVec.z);
    }
    else if constexpr (m == ViewportMode::WIREFRAME)
    {
        /*
#ifdef TEST_HALFEDGES

        const Mesh &currentMesh = mScene->getMeshes()[mTestMesh];

        const auto halfEdgesVect=currentMesh.getHalfEdges();
        mTestHalfEdge%=halfEdgesVect.size();

        const int halfEdgeOrigin = mGlFuncs->glGetUniformLocation(programID, "halfEdgeOrigin");
        const int halfEdgeEnd = mGlFuncs->glGetUniformLocation(programID, "halfEdgeEnd");

        const glm::vec3 origin = currentMesh.getGeometricVertexPosition(halfEdgesVect[mTestHalfEdge].origin);
        const glm::vec3 end = currentMesh.getGeometricVertexPosition(halfEdgesVect[mTestHalfEdge].end);


        mGlFuncs->glUniform3f(halfEdgeOrigin,origin.x,origin.y,origin.z);
        mGlFuncs->glUniform3f(halfEdgeEnd,end.x,end.y,end.z);
#endif */
    }
}

template <ViewportMode m>
void Renderer::drawForMode(const GLuint programID)
{
    if constexpr (m == ViewportMode::SOLID)
    {
        mRenderVAO.bind();
        mGlFuncs->glDrawElements(GL_TRIANGLES, mNbFaceIndices, GL_UNSIGNED_INT, nullptr);
        mRenderVAO.release();
        if (mCurrentSelectionMode != SelectionMode::NONE)
            drawLines();
    }
    else if constexpr (m == ViewportMode::WIREFRAME)
    {
        drawLines();
        /*
        mGeometricVAO.bind();
        mGlFuncs->glDrawElements(GL_LINES, mNbEdgeIndices, GL_UNSIGNED_INT, nullptr);
        mGeometricVAO.release();*/
    }
    else if constexpr (m == ViewportMode::MATERIAL)
    {

        mRenderVAO.bind();
        const int Ks= mGlFuncs->glGetUniformLocation(programID, "Ks");
        const int Ka= mGlFuncs->glGetUniformLocation(programID, "Ka");
        const int Kd= mGlFuncs->glGetUniformLocation(programID, "Kd");
        const int Ns= mGlFuncs->glGetUniformLocation(programID, "Ns");
        const int alpha= mGlFuncs->glGetUniformLocation(programID, "alpha");
        const int scale = mGlFuncs->glGetUniformLocation(programID, "scale");



        uint32_t startTriangle=0,endTriangle=0;
        for (auto &mesh : mScene->getMeshes()) {
            auto &subMeshes = mesh.getSubMeshes();
            for (uint32_t submeshIndex = 0; submeshIndex < subMeshes.size(); submeshIndex++)
            {
                const Material &mat= mScene->getMaterial(mesh.getMaterialId(submeshIndex));

                mGlFuncs->glActiveTexture(GL_TEXTURE0);
                GLint textureSlot = TextureManager::loadSceneTexture(mat.ColorTextureID, mScene);
                mGlFuncs->glBindTexture(GL_TEXTURE_2D, textureSlot);  // ton ID de texture
                int texLoc = mGlFuncs->glGetUniformLocation(programID, "colorTexture");
                mGlFuncs->glUniform1i(texLoc, 0);


                mGlFuncs->glUniform3f(Ks,mat.Ks.r,mat.Ks.g,mat.Ks.b);
                mGlFuncs->glUniform3f(Ka,mat.Ka.r,mat.Ka.g,mat.Ka.b);
                mGlFuncs->glUniform3f(Kd,mat.Kd.r,mat.Kd.g,mat.Kd.b);
                mGlFuncs->glUniform1f(Ns,mat.shininess);
                mGlFuncs->glUniform1f(alpha,mat.alpha);

                mGlFuncs->glUniform1f(scale, mesh.getScale());


                endTriangle+= 3*mesh.getTriangles(submeshIndex).size();
                mGlFuncs->glDrawElements(
                GL_TRIANGLES,
                    endTriangle - startTriangle,
                    GL_UNSIGNED_INT,
                        (void*)(startTriangle * sizeof(uint32_t))
                );
                startTriangle=endTriangle  ;
            }
        }
        mRenderVAO.release();
    }
}

void Renderer::drawPoints()
{

    // Drawing vertices
    mGeometricVAO.bind();
    mGeometricVBO.bind();
    mGeometricEBO.bind();
    mGlFuncs->glEnable(GL_PROGRAM_POINT_SIZE);
    const GLuint programID = ShaderManager::getShaderProgram(VERTICES);
    mGlFuncs->glUseProgram(programID);
    auto viewMatrix= mGlFuncs->glGetUniformLocation(programID, "viewMatrix");
    mGlFuncs->glUniformMatrix4fv (viewMatrix, 1, GL_FALSE, &mEngineCamera->computeViewMatrix()[0][0]);

    auto projMatrix= mGlFuncs->glGetUniformLocation(programID, "projMatrix");
    mGlFuncs->glUniformMatrix4fv (projMatrix, 1, GL_FALSE, &mEngineCamera->computePerspectiveMatrix()[0][0]);

    auto selectedVertex = mGlFuncs->glGetUniformLocation(programID, "selectedVertex");
    mGlFuncs->glUniform1i (selectedVertex, (mCurrentSelectionMode == SelectionMode::VERTEX)? mSelection : -1);
    mGlFuncs->glDrawArrays(GL_POINTS, 0, nVertices);

    mGeometricVAO.release();
    mGeometricVBO.release();
    mGeometricEBO.release();
}

void Renderer::drawLines()
{
    // Drawing vertices
    mGeometricVAO.bind();
    mGeometricVBO.bind();
    mGeometricEBO.bind();
    const GLuint programID = ShaderManager::getShaderProgram(LINES);
    mGlFuncs->glUseProgram(programID);
    auto viewMatrix= mGlFuncs->glGetUniformLocation(programID, "viewMatrix");
    mGlFuncs->glUniformMatrix4fv (viewMatrix, 1, GL_FALSE, &mEngineCamera->computeViewMatrix()[0][0]);

    auto projMatrix= mGlFuncs->glGetUniformLocation(programID, "projMatrix");
    mGlFuncs->glUniformMatrix4fv (projMatrix, 1, GL_FALSE, &mEngineCamera->computePerspectiveMatrix()[0][0]);


    //TODO faire une vraie logique de selection
    const auto selectedEdge= mGlFuncs->glGetUniformLocation(programID, "selectedEdge");
    mGlFuncs->glUniform1i(selectedEdge, false? 1 : 0);
    const auto edgeOrigin= mGlFuncs->glGetUniformLocation(programID, "edgeOrigin");
    mGlFuncs->glUniform3f(edgeOrigin, 0.0, 0.0, 0.0);
    const auto edgeEnd= mGlFuncs->glGetUniformLocation(programID, "edgeEnd");
    mGlFuncs->glUniform3f(edgeEnd, 0.0, 0.0, 0.0);

    mGlFuncs->glDrawElements(GL_LINES, mNbEdgeIndices, GL_UNSIGNED_INT, nullptr);

    mGeometricVAO.release();
    mGeometricVBO.release();
    mGeometricEBO.release();
}

template <ViewportMode m>
void Renderer::drawTemplate()
{
    // On choisit le programme du vertex shader
    GLuint programID = getShaderForMode<m>();
    mGlFuncs->glUseProgram(programID);

    setShaderArguments<m>(programID);
    drawForMode<m>(programID);

    if (mCurrentSelectionMode != SelectionMode::NONE)
        drawPoints();
    //on dessine d'abord la grid
    if (mGridToggled) drawGrid();

    updatePickingBuffer(); // Toujours faire en dernier
}



void Renderer::draw(const ViewportMode mode)
{
    if (mode == ViewportMode::SOLID)
        drawTemplate<ViewportMode::SOLID>();

    else if (mode == ViewportMode::WIREFRAME)
        drawTemplate<ViewportMode::WIREFRAME>();

    else if (mode == ViewportMode::MATERIAL)
        drawTemplate<ViewportMode::MATERIAL>();
}

void Renderer::buildFaceBuffer(const std::vector<Mesh> &meshes)
{
    std::vector<RenderVertex> vertices;
    // Buffer des faces
    std::vector<uint32_t> indices;
    uint32_t indexOffset = 0;
    for (const Mesh &mesh : meshes)
    {
        const auto meshVertices = mesh.getRenderVertices();
        vertices.reserve(vertices.size() + meshVertices.size());
        vertices.insert(vertices.end(), meshVertices.begin(), meshVertices.end());
        std::vector<Triangle> triangles;
        for (uint32_t i = 0; i < mesh.getSubMeshes().size(); i++)
        {
            const auto currentTriangles = mesh.getTriangles(i);
            triangles.insert(triangles.end(), currentTriangles.begin(), currentTriangles.end());
        }
        indices.reserve(indices.size() + triangles.size()*3);
        for (const auto& t: triangles)
        {
            indices.push_back(t[0] + indexOffset);
            indices.push_back(t[1] + indexOffset);
            indices.push_back(t[2] + indexOffset);
        }
        indexOffset += meshVertices.size();
    }

    const RenderVertex *vertices_data = vertices.data(); // Pointeur vers les vertices
    mRenderVBO.bind();
    mRenderVBO.allocate(vertices_data,vertices.size() * sizeof(RenderVertex));

    const uint32_t *data = indices.data();
    mNbFaceIndices = indices.size();

    mRenderEBO.bind();
    mRenderEBO.allocate(data,mNbFaceIndices * sizeof(uint32_t));
    mRenderVBO.release();
}


void Renderer::buildEdgeBuffer(const std::vector<Mesh> &meshes)
{
    std::vector<glm::vec3> vertexPositions;
    std::vector<uint32_t> indices;
    uint32_t indexOffset = 0;
    for (const Mesh &mesh : meshes)
    {
        // Construction des positions;
        vertexPositions.reserve(vertexPositions.size() + mesh.getGeometricVertices().size());
        for (int i = 0; i < mesh.getGeometricVertices().size(); i++)
            vertexPositions.push_back(mesh.getGeometricVertexPosition(i));

        //Buffer des edges
        const auto edges = mesh.getEdges();
        const auto geometricVertices = mesh.getGeometricVertices();

        indices.reserve(indices.size() + edges.size()*2);
        for (const auto& [origin, end]: edges)
        {
            indices.push_back(origin + indexOffset);
            indices.push_back(end + indexOffset);
        }
        indexOffset += geometricVertices.size();
    }

    mGeometricVBO.bind();
    const glm::vec3 *positions_data = vertexPositions.data();
    mGeometricVBO.allocate(positions_data, vertexPositions.size() * sizeof(glm::vec3));
    nVertices = vertexPositions.size();

    const uint32_t *data = indices.data();
    mNbEdgeIndices = indices.size();
    mGeometricEBO.bind();
    mGeometricEBO.allocate(data,mNbEdgeIndices * sizeof(uint32_t));
    mGeometricVBO.release();
}

void Renderer::updatePickingBuffer()
{
    mGlFuncs->glBindFramebuffer(GL_FRAMEBUFFER, mPickingFBO);

    GLint prevViewport[4];
    mGlFuncs->glGetIntegerv(GL_VIEWPORT, prevViewport);

    GLfloat prevClearColor[4];
    mGlFuncs->glGetFloatv(GL_COLOR_CLEAR_VALUE, prevClearColor);

    const GLboolean prevDepthTest = mGlFuncs->glIsEnabled(GL_DEPTH_TEST);
    mGlFuncs->glEnable(GL_PROGRAM_POINT_SIZE);
    mGlFuncs->glViewport(0, 0, mWidth, mHeight);
    mGlFuncs->glDepthMask(GL_TRUE);

    QOpenGLExtraFunctions *extraFuncs = QOpenGLContext::currentContext()->extraFunctions();
    GLuint clearValue = 0;
    // 3. Clear Color Attachment 0 with the unsigned integer
    extraFuncs->glClearBufferuiv(GL_COLOR, 0, &clearValue);

    mGlFuncs->glClear(GL_DEPTH_BUFFER_BIT);
    mGlFuncs->glEnable(GL_DEPTH_TEST);
    mGlFuncs->glDisable(GL_BLEND);

    const GLuint programID = ShaderManager::getShaderProgram(PICKING);
    mGlFuncs->glUseProgram(programID);

    const int viewMatrix= mGlFuncs->glGetUniformLocation(programID, "viewMatrix");
    mGlFuncs->glUniformMatrix4fv (viewMatrix, 1, GL_FALSE, &mEngineCamera->computeViewMatrix()[0][0]);

    const int projMatrix= mGlFuncs->glGetUniformLocation(programID, "projMatrix");
    mGlFuncs->glUniformMatrix4fv (projMatrix, 1, GL_FALSE, &mEngineCamera->computePerspectiveMatrix()[0][0]);

    // 3. Draw geometry (Depth test automatically hides obscured vertices)
    mGeometricVAO.bind();
    mGlFuncs->glDrawArrays(GL_POINTS, 0, nVertices);

    mGeometricVAO.release();

    mGlFuncs->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mGlFuncs->glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
    mGlFuncs->glClearColor(prevClearColor[0], prevClearColor[1], prevClearColor[2], prevClearColor[3]);

    if (prevDepthTest) {
        mGlFuncs->glEnable(GL_DEPTH_TEST);
    } else {
        mGlFuncs->glDisable(GL_DEPTH_TEST);
    }
}

int32_t Renderer::readPickingBuffer(const int x, int y)
{
    mGlFuncs->glBindFramebuffer(GL_FRAMEBUFFER, mPickingFBO);
    uint32_t selectedID = 0;

    // OpenGL uses bottom-left origin coordinates
    y = mHeight - y*2 - 1;
    mGlFuncs->glReadPixels(x*2, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &selectedID);

    mGlFuncs->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    mSelection = selectedID -1;
    return selectedID - 1;
}



void Renderer::initPickingBuffer()
{
    // 2. Create and configure the integer texture
    mGlFuncs->glGenTextures(1, &mPickingTexture);
    mGlFuncs->glBindTexture(GL_TEXTURE_2D, mPickingTexture);
    mGlFuncs->glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, mWidth, mHeight, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);

    // CRITICAL: Apply GL_NEAREST filtering immediately
    mGlFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    mGlFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    mGlFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    mGlFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 3. Create the Depth Buffer (so depth testing works during picking)
    mGlFuncs->glGenRenderbuffers(1, &mDepthBuffer);
    mGlFuncs->glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
    mGlFuncs->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mWidth, mHeight);

    // 4. Create the Framebuffer Object and attach the texture + depth buffer
    mGlFuncs->glGenFramebuffers(1, &mPickingFBO);
    mGlFuncs->glBindFramebuffer(GL_FRAMEBUFFER, mPickingFBO);

    mGlFuncs->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPickingTexture, 0);
    mGlFuncs->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);

    // 5. Verify OpenGL is happy with the setup
    if (mGlFuncs->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        qDebug() << "Integer FBO is incomplete!";
    }

    // 6. Unbind to restore clean state
    mGlFuncs->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    mGlFuncs->glBindTexture(GL_TEXTURE_2D, 0);
    mGlFuncs->glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderer::resizePickingBuffer()
{
    mGlFuncs->glDeleteFramebuffers(1, &mPickingFBO);
    mGlFuncs->glDeleteTextures(1, &mPickingTexture);
    mGlFuncs->glDeleteRenderbuffers(1, &mDepthBuffer);
    initPickingBuffer();
}


template <ViewportMode m>
void Renderer::geometryRedrawTemplate()
{
    buildFaceBuffer(mScene->getMeshes());
    buildEdgeBuffer(mScene->getMeshes());
    drawTemplate<m>();
}

void Renderer::geometryRedraw(const ViewportMode mode)
{
    if (mode == ViewportMode::SOLID)
        geometryRedrawTemplate<ViewportMode::SOLID>();

    else if (mode == ViewportMode::WIREFRAME)
        geometryRedrawTemplate<ViewportMode::WIREFRAME>();

    else if (mode == ViewportMode::MATERIAL)
        geometryRedrawTemplate<ViewportMode::MATERIAL>();

}

void Renderer::initShaders()
{
    for (int i = 0; i < 4; i++)
    {
        const GLuint vertexShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/viewport.vert",
            GL_VERTEX_SHADER,
            {{.field = "RENDER_MODE", .value = std::to_string(i)}});
        const GLuint fragmentShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/viewport.frag",
            GL_FRAGMENT_SHADER,
            {{.field = "RENDER_MODE", .value = std::to_string(i)}});
        std::vector shaders = {vertexShader, fragmentShader};
        switch (i)
        {
        case 0:
            ShaderManager::createProgram(VERTICES, shaders);
            break;
        case 1:
            ShaderManager::createProgram(LINES, shaders);
            break;
        case 2:
            ShaderManager::createProgram(VIEWPORT_SOLID, shaders);
            break;
        case 3:
            ShaderManager::createProgram(VIEWPORT_MATERIAL, shaders);
            break;
        }
    }

    auto vertexShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/grid.vert", GL_VERTEX_SHADER);
    auto fragmentShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/grid.frag", GL_FRAGMENT_SHADER);
    const auto gridShaders = {vertexShader, fragmentShader};
    ShaderManager::createProgram(GRID, gridShaders);


    vertexShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/picking.vert", GL_VERTEX_SHADER);
    fragmentShader = ShaderManager::compileQTRessourceShader(":/assets/shaders/picking.frag", GL_FRAGMENT_SHADER);
    const auto pickingShaders = {vertexShader, fragmentShader};
    ShaderManager::createProgram(PICKING, pickingShaders);
}

void Renderer::initialize(QOpenGLFunctions* glFuncs)
{
    //3D
    mGlFuncs = glFuncs;
    ShaderManager::initialize(glFuncs);
    TextureManager::initialize(glFuncs);

    if (!mRenderVAO.create()) exit(1);
    if (!mRenderVBO.create()) exit(1);
    if (!mRenderEBO.create()) exit(1);


    if (!mGridVAO.create()) exit(1);

    if (!mGeometricVAO.create()) exit(1);
    if (!mGeometricVBO.create()) exit(1);
    if (!mGeometricEBO.create()) exit(1);

    mRenderVAO.bind();
    mRenderVBO.bind();
    // 0: position du vertex
    mGlFuncs->glEnableVertexAttribArray(0);
    mGlFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (void*)offsetof(RenderVertex, x));

    // 1: normales du vertex
    mGlFuncs->glEnableVertexAttribArray(1);
    mGlFuncs->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (void*)offsetof(RenderVertex, nx));

    // 2: uv du vertex
    mGlFuncs->glEnableVertexAttribArray(2);
    mGlFuncs->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (void*)offsetof(RenderVertex, u));

    mRenderEBO.bind();
    mRenderVAO.release();

    mGeometricVAO.bind();
    mGeometricVBO.bind();
    mGlFuncs->glEnableVertexAttribArray(0);
    mGlFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    mGeometricEBO.bind();
    mGeometricVAO.release();

    initPickingBuffer();

    initShaders();
}

void Renderer::resize(const int width, int height)
{
    if (height == 0) height = 1;
    const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    mHeight = height*2;
    mWidth = width*2; //TODO ENLEVER
    mEngineCamera->setAspectRatio(aspectRatio);
    resizePickingBuffer();
}


template void Renderer::drawTemplate<ViewportMode::SOLID>();
template void Renderer::drawTemplate<ViewportMode::WIREFRAME>();
template void Renderer::drawTemplate<ViewportMode::MATERIAL>();

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

    const int cameraPos = mGlFuncs->glGetUniformLocation(programID, "cameraPos");
    const auto cam = mEngineCamera->getPosition();
    mGlFuncs->glUniform3f(cameraPos, cam[0], cam[1], cam[2]);


    const int anchorPos = mGlFuncs->glGetUniformLocation(programID, "anchorPos");
    const auto anchor = mEngineCamera->getAnchorPoint();
    mGlFuncs->glUniform3f(anchorPos, anchor[0], anchor[1], anchor[2]);

    mGridVAO.bind();
    mGlFuncs->glDrawArrays(GL_TRIANGLES, 0, 3);
    mGridVAO.release();

    glDisable(GL_BLEND);
}