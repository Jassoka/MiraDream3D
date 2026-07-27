#include "view/RenderWidget.h"
#include "types.h"

#include <QTimer>
#include <iostream>
#include <QMouseEvent>

RenderWidget::RenderWidget(int framesPerSecond, QWidget *parent) :
    QOpenGLWidget(parent)
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
    setFocusPolicy(Qt::ClickFocus);
    setUpdateBehavior(QOpenGLWidget::PartialUpdate);//TODO ca explose si pas ca sur mac (clignotements). Probleme de buffer
}

void RenderWidget::requestRedraw()
{
    mHasToRedraw = true;
}

void RenderWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    auto* glFuncs = QOpenGLContext::currentContext()->functions();
    emit initialize(glFuncs);
}
void RenderWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    const double scale = devicePixelRatioF();
    emit resize(width * scale, height * scale);
}
void RenderWidget::paintGL() {
    emit paint();
}


void RenderWidget::mousePressEvent(QMouseEvent *event) {
    const QPoint localpos = mapFromGlobal(QCursor::pos());
    emit mousePress(event->button(), localpos.x(), localpos.y());
}

void RenderWidget::teleportMouseToCenter()
{
    mIsTeleportingCursor = true;
    const QPoint screenCenter = mapToGlobal(rect().center());
    QCursor::setPos(screenCenter);
    mMouseLastPosition = screenCenter;
}

void RenderWidget::initMouseDrag()
{
    mMouseAnchor = QCursor::pos();
    mMouseLastPosition = mMouseAnchor;
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event) {
    const QPoint currPos = QCursor::pos();
    emit mouseDrag(currPos.x() - mMouseLastPosition.x(), currPos.y() - mMouseLastPosition.y(), event->buttons());
}
void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseRelease(event->button());
}

void RenderWidget::wheelEvent(QWheelEvent *event) {
    emit mouseScroll(event->angleDelta().x(), event->angleDelta().y());
    //if (event->modifiers() & Qt::ShiftModifier) deltaX*=2;
}

void RenderWidget::centerCursor()
{
    QCursor::setPos(mMouseAnchor);
}


void RenderWidget::keyPressEvent(QKeyEvent *event) {
    mPressedKeys.insert(event->key());
    switch (event->key()) {
    case(Qt::Key_1):
        emit setViewportMode(ViewportMode::SOLID);
        break;
    case(Qt::Key_2):
        emit setViewportMode(ViewportMode::WIREFRAME);
        break;
    case(Qt::Key_3):
        emit setViewportMode(ViewportMode::MATERIAL);
        break;
#ifdef TEST_HALFEDGES
    case (Qt::Key_N): // Next half edge
        emit addTestHalfEdgeSignal(1);
        break;
    case (Qt::Key_B): // Prev half edge
        emit addTestHalfEdgeSignal(-1);
        break;
    case (Qt::Key_T): // Twin half edge
        emit addTestHalfEdgeSignal(0);
        break;
    case (Qt::Key_M): // Next mesh
        emit nextTestMeshSignal();
        break;
    case (Qt::Key_C): // Next mesh
        emit nextTestComponentSignal();
        break;
#endif

    }
}


void RenderWidget::keyReleaseEvent(QKeyEvent* event) {
    mPressedKeys.remove(event->key());
}

void RenderWidget::timeOutSlot() {

    emit refresh();
    if (mHasToRedraw)
    {
        update();
        mHasToRedraw = false;
    }
}

RenderWidget::~RenderWidget()
{
    delete mTimer;
}