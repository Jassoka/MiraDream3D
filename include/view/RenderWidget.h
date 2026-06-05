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
    virtual void initializeGL();    
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
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
private:
    Renderer *mRenderer;
    QTimer *mTimer;
};


#endif //MIRADREAM3D_RENDERWIDGET_H
