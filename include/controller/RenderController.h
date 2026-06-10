//
// Created by jassoka on 6/9/26.
//

#ifndef MIRADREAM3D_RENDERCONTROLLER_H
#define MIRADREAM3D_RENDERCONTROLLER_H

#include <QObject>

class Scene;
class Renderer;
class QOpenGLFunctions;
enum class ViewportMode;


class RenderController: public QObject
{
public:
    explicit RenderController(QObject* parent);
    ~RenderController() override;

    void setScene(const Scene *scene) const;

    bool hasGeometryChanged() const { return mHasGeometryChanged; }
    bool hasTopologyChanged() const { return mHasTopologyChanged; }
    bool hasCameraChanged() const { return mHasCameraChanged; }

    void changedGeometry() { mHasGeometryChanged = hasToRedraw = true; }
    void changedTopology() { mHasTopologyChanged = hasToRedraw = true; }
    void changedCamera() { mHasCameraChanged = hasToRedraw = true; }

    void geometryRedrawn() { mHasGeometryChanged = mHasTopologyChanged = mHasCameraChanged = hasToRedraw = false; }
    void topologyRedrawn() { mHasTopologyChanged = mHasCameraChanged = hasToRedraw = false; }
    void cameraRedrawn() { mHasCameraChanged = hasToRedraw = false; }

public slots:
    void paint();
    void onResize(int width, int height);
    void onInitialize(QOpenGLFunctions* glFuncs) const;

    void onRotateAroundAnchor(float dPhi, float dTheta);

    void onCameraStrafe(float dx, float dy);
    void onZoom(float factor);
    void onSetViewportMode(ViewportMode mode);

private:
    bool hasToRedraw = false;
    bool mHasGeometryChanged = false;
    bool mHasTopologyChanged = false;
    bool mHasCameraChanged = false;
    Renderer *mRenderer;
    ViewportMode mCurrViewportMode;
};


#endif //MIRADREAM3D_RENDERCONTROLLER_H
