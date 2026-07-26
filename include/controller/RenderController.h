//
// Created by jassoka on 6/9/26.
//

#ifndef MIRADREAM3D_RENDERCONTROLLER_H
#define MIRADREAM3D_RENDERCONTROLLER_H

#include <QObject>

class RenderWidget;
enum class EditorTool;
class Scene;
class Renderer;
class QOpenGLFunctions;
enum class ViewportMode;

/**
 * @brief Controller class for managing 3D rendering
 */
class RenderController: public QObject
{
  Q_OBJECT
public:
    /**
     * @brief Default constructor for RenderController
     * @param parent QT Parent Object
     * @param render_widget Render Widget
     */
    explicit RenderController(QObject* parent, RenderWidget *render_widget);

    ~RenderController() override;

    /**
     * @brief Sets the rendered scene
     */
    void setScene(const Scene *scene) const;

    /**
     * @getter{\ref RenderController::mHasGeometryChanged}
     */
    bool hasGeometryChanged() const { return mHasGeometryChanged; }
    /**
     * @getter{\ref RenderController::mHasTopologyChanged}
     */
    bool hasTopologyChanged() const { return mHasTopologyChanged; }
    /**
     * @getter{\ref RenderController::mHasCameraChanged}
     */
    bool hasCameraChanged() const { return mHasCameraChanged; }

    /**
      * @brief Called when geometry is modified by the software
      */
    void changedGeometry() { mHasGeometryChanged = true; emit callWidgetRedraw();}
    /**
      * @brief Called when topology is modified by the software
      */
    void changedTopology() { mHasTopologyChanged = true; emit callWidgetRedraw(); }
    /**
      * @brief Called when camera is modified by the software
      */
    void changedCamera() { mHasCameraChanged = true; emit callWidgetRedraw(); }

    /**
      * @brief Resets the flags once geometry is properly redrawn
      */
    void geometryRedrawn() { mHasGeometryChanged = mHasTopologyChanged = mHasCameraChanged = false; }
    /**
      * @brief Resets the flags once topology is properly redrawn
      */
    void topologyRedrawn() { mHasTopologyChanged = mHasCameraChanged = false; }
    /**
      * @brief Resets the flags once the camera is properly redrawn
      */
    void cameraRedrawn() { mHasCameraChanged = false; }

    void toolChanged(EditorTool tool);

public slots:
    /** @brief Signal called each frame to check if, and how the render needs to be redrawn */
    void paint();
    /** @brief Signal which resizes the camera aspect ratio upon resizing the widget */
    void onResize(int width, int height);
    /** @brief Signal which is called once OpenGL is initialised properly */
    void onInitialize(QOpenGLFunctions* glFuncs) const;


    /** @brief Signal called upon camera rotation */
    void onRotateAroundAnchor(float dPhi, float dTheta);
    /** @brief Signal called upon camera translation */
    void onCameraStrafe(float dx, float dy);
    /** @brief Signal called upon camera zoom */
    void onZoom(float factor);
    /** @brief Signal called upon changing the current viewport Mode (always redraws) */
    void onSetViewportMode(ViewportMode mode);
#ifdef TEST_HALFEDGES
    void onAddHalfEdgeTest(int i);
    void nextMeshTest();

    void nextComponentTest();
#endif
  signals:
    void callWidgetRedraw();

private:
    /** @brief Flag which returns true if geometry needs to be redrawn (implies \ref mHasTopologyChanged, \ref mHasCameraChanged, and \ref mHasToRedraw) */
    bool mHasGeometryChanged = false;
    /**
     * @brief Flag which returns true if topology needs to be redrawn (implies \ref mHasCameraChanged, and \ref mHasToRedraw)
     * @note Topology is referring to when faces, vertices or edges are moved, but no new one is created
     * There is then no need to reallocate GPU memory
     */
    bool mHasTopologyChanged = false;
    /** @brief Flag which returns true if the camera has moved (implies \ref mHasToRedraw) */
    bool mHasCameraChanged = false;
    /** @brief Pointer to the \ref Renderer object */
    Renderer *mRenderer;
    /** @brief Pointer to \ref RenderWidget object */
    RenderWidget *mRenderWidget;
    /** @brief Current viewport mode (if in viewport) */
    ViewportMode mCurrViewportMode;
};


#endif //MIRADREAM3D_RENDERCONTROLLER_H
