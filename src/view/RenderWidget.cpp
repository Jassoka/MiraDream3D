#include "view/RenderWidget.h"

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
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);


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
        int deltaX = event->pos().x() - mLastMousePosition.x();
        int deltaY = event->pos().y() - mLastMousePosition.y();

        float dPhi   = -deltaX * mMouseSensitivity;
        float dTheta = -deltaY * mMouseSensitivity;

        mRenderer->getEngineCamera().rotateAroundAnchor(dPhi, dTheta);

        mLastMousePosition = event->pos();

        this->update();
    }
}
void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        unsetCursor();
    }
}


void RenderWidget::timeOutSlot() {
}

RenderWidget::~RenderWidget()
{
    delete mRenderer;
    delete mTimer;
}