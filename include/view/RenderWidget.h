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
    explicit RenderWidget(int framesPerSecond=0, QWidget *parent = 0);
    virtual void initializeGL();    
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    ~RenderWidget();

public slots:
    virtual void timeOutSlot();
private:
    Renderer *mRenderer;
    QTimer *mTimer;
};


#endif //MIRADREAM3D_RENDERWIDGET_H
