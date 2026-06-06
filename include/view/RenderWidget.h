//
// Created by Mathis Pean on 30/05/2026.
//

#ifndef MIRADREAM3D_RENDERWIDGET_H
#define MIRADREAM3D_RENDERWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>

#include "model/Renderer.h"

/**
 * @brief Widget in which we show the 3D render
 */
class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    explicit RenderWidget(int framesPerSecond=0, QWidget *parent = nullptr);
    virtual void initializeGL() override;
    virtual void resizeGL(int width, int height) override;
    virtual void paintGL() override;
    void setHasChanged(bool b);
    void setRenderer(Renderer *renderer);
    void setScene(const Scene *scene)
    {
        mRenderer->setScene(scene);
    }
    ~RenderWidget();

public slots:
    virtual void timeOutSlot();

signals:
    void initRenderer(QOpenGLFunctions* fonctionsGL, float aspectRatio);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    Renderer *mRenderer;
    QTimer *mTimer;
    QPoint mLastMousePosition;
    float mMouseSensitivity = 0.0035f;
    bool mWasTeleported = false;
};


#endif //MIRADREAM3D_RENDERWIDGET_H
