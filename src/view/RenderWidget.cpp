#include "view/RenderWidget.h"

#include <iostream>
#include <QOpenGLFunctions>

RenderWidget::RenderWidget(int framesPerSecond, QWidget *parent) :
    QOpenGLWidget(parent), mRenderer(new Renderer())
{

    if (framesPerSecond==0)
        mTimer=NULL;
    else {
        //1seconde=1000ms
        int timerInterval = 1000/framesPerSecond;
        mTimer=new QTimer(this);
        connect(mTimer,SIGNAL(timeout()),this,SLOT(timeOutSlot()));
        mTimer->start(timerInterval);
    }
}

void RenderWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    auto* glFuncs = QOpenGLContext::currentContext()->functions();
    mRenderer->initialize(glFuncs);
    //setupShaders
    //setupBuffers

    //vao.create(); vao.bind();
    //vbo.create(); vbo.bind();

}
void RenderWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    mRenderer->resize(this->width(), this->height());
}
void RenderWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (mRenderer->hasGeometryChange)
    {
        mRenderer->geometryRedraw();
        mRenderer->hasGeometryChange = false;
    }
    else if (mRenderer->hasTopologyChange)
    {
        //TODO
    }
    else
    {
        mRenderer->draw();
    }
}


void RenderWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        mLastMousePosition = event->pos();
        setCursor(Qt::ClosedHandCursor); // Nice UX touch: change cursor when dragging
    }
}
void RenderWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::MiddleButton) {

        // 1. Calculate how many pixels the mouse moved since the last frame
        int deltaX = event->pos().x() - mLastMousePosition.x();
        int deltaY = event->pos().y() - mLastMousePosition.y();

        // 2. Convert pixels to radians using your sensitivity factor
        float dPhi   = -deltaX * mMouseSensitivity; // Horizontal rotation
        float dTheta = -deltaY * mMouseSensitivity; // Vertical rotation

        // 3. Inject these deltas directly into your matrix rotation function!
        mRenderer->getEngineCamera().rotateAroundAnchor(dPhi, dTheta);

        // 4. Save the current position as the "last position" for the next move frame
        mLastMousePosition = event->pos();

        // 5. Force Qt to redraw the OpenGL canvas
        this->update();
    }
}
void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        unsetCursor(); // Restore the normal arrow cursor
    }
}


void RenderWidget::timeOutSlot() {
}

RenderWidget::~RenderWidget()
{
    delete mRenderer;
    delete mTimer;
}