//
// Created by Mathis Pean on 30/05/2026.
//

#ifndef MIRADREAM3D_RENDERWIDGET_H
#define MIRADREAM3D_RENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

enum class ViewportMode;

/**
 * @brief Widget in which we show the 3D render
 */
class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    explicit RenderWidget(int framesPerSecond=0, QWidget *parent = nullptr);
    ~RenderWidget() override;

    void teleportMouseToCenter();
    void initMouseDrag();
    void centerCursor();

public slots:
    virtual void timeOutSlot();
    void requestRedraw();

signals:
    void initialize(QOpenGLFunctions *glFuncs);
    void resize(int width, int height);
    void paint();

    void mouseDrag(int x, int y, Qt::MouseButtons boutons);
    void mouseScroll(int deltaX, int deltaY);
    void mousePress(Qt::MouseButton button, int localX, int localY);
    void mouseRelease(Qt::MouseButton button);
    void refresh();

    /*
    void rotateAroundAnchor(float dPhi, float dTheta);
    void strafeCamera(float dx, float dy);
    void zoom(float zoomFactor);
    */
    void setViewportMode(ViewportMode mode);
    void pickFromScreen(int x, int y);
#ifdef TEST_HALFEDGES
    void addTestHalfEdgeSignal(int i);
    void nextTestMeshSignal();
    void nextTestComponentSignal();
#endif
protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    QTimer *mTimer;
    QPoint mMouseAnchor;
    QPoint mMouseLastPosition;


    QSet<int> mPressedKeys;
    bool mIsTeleportingCursor = false;
    bool mWasTeleported = false;
    bool mHasToRedraw = true;
};


#endif //MIRADREAM3D_RENDERWIDGET_H
