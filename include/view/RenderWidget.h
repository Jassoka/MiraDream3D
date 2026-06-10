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
    ~RenderWidget();

    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

public slots:
    virtual void timeOutSlot();

signals:
    void initialize(QOpenGLFunctions *glFuncs);
    void resize(int width, int height);
    void paint();
    void rotateAroundAnchor(float dPhi, float dTheta);
    void strafeCamera(float dx, float dy);
    void zoom(float zoomFactor);
    void setViewportMode(ViewportMode mode);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    QTimer *mTimer;
    QPoint mMouseAnchor;
    QPoint mMouseLastPosition;
    int mMouseDragRotateX = 0; // variable qui stocke la valeur de rotation
    int mMouseDragRotateY = 0;

    int mMouseDragTranslateX = 0; // variable qui stocke la valeur de translation
    int mMouseDragTranslateY = 0;

    int mMouseScroll = 0;

    QSet<int> mPressedKeys;
    float mMouseRotateSensitivity = 0.0035f;
    float mMouseTranslateSensitivity = 0.0035f;
    float mScrollSensitivity = 0.00035f;
    bool mIsTeleportingCursor = false;
    bool mWasTeleported = false;
};


#endif //MIRADREAM3D_RENDERWIDGET_H
