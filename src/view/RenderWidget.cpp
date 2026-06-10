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
    setUpdateBehavior(QOpenGLWidget::PartialUpdate);//TODO ca explose si pas ca sur mac (clignotements). Probleme de buffer
}

void RenderWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    auto* glFuncs = QOpenGLContext::currentContext()->functions();
    emit initialize(glFuncs);
}
void RenderWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    emit resize(width, height);
}
void RenderWidget::paintGL() {
    emit paint();
}


void RenderWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        grabMouse();
        mMouseAnchor = QCursor::pos();
        mMouseLastPosition = mMouseAnchor;
        setCursor(Qt::BlankCursor); // Nice UX touch: change cursor when dragging
        if (event->button() == Qt::LeftButton)
            mMouseDragRotateX = mMouseDragRotateY = 0;
        else
            mMouseDragTranslateX = mMouseDragTranslateY = 0;
    }
}
void RenderWidget::mouseMoveEvent(QMouseEvent *event) {

    auto teleport = [this]()
    {
        mIsTeleportingCursor = true;
        const QPoint screenCenter = mapToGlobal(rect().center());
        QCursor::setPos(screenCenter);
        mMouseLastPosition = screenCenter;
    };

    auto hasTeleported = [this]() -> bool
    {
        if (mIsTeleportingCursor) {
            mIsTeleportingCursor = false;
        }
        return mIsTeleportingCursor;
    };
    if (event->buttons() & Qt::LeftButton ) {

        if (hasTeleported()) return;
        QPoint currPos = QCursor::pos();

        mMouseDragRotateX += currPos.x() - mMouseLastPosition.x();
        mMouseDragRotateY += currPos.y() - mMouseLastPosition.y();
        teleport();
    }
    if (event->buttons() & Qt::RightButton) {

        if (hasTeleported()) return;
        QPoint currPos = QCursor::pos();

        mMouseDragTranslateX += currPos.x() - mMouseLastPosition.x();
        mMouseDragTranslateY += currPos.y() - mMouseLastPosition.y();
        teleport();
    }
}
void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        QCursor::setPos(mMouseAnchor); //TODO faire une méthode centerCursor
        releaseMouse();
        unsetCursor(); // Restore the normal arrow cursor
    }
}

void RenderWidget::wheelEvent(QWheelEvent *event) {
    int deltaY = event->angleDelta().y();
    int deltaX = event->angleDelta().x();
    if (event->modifiers() & Qt::ShiftModifier) deltaX*=2;
    mMouseScroll += deltaX + 2*deltaY;
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
    }
}


void RenderWidget::keyReleaseEvent(QKeyEvent* event) {
    mPressedKeys.remove(event->key());
}

void RenderWidget::timeOutSlot() {
    if (mMouseDragRotateX != 0 || mMouseDragRotateY != 0) // Engager rotation de la caméra
    {
        float dPhi   = -mMouseDragRotateX * mMouseSensitivity;
        float dTheta = -mMouseDragRotateY * mMouseSensitivity;
        mMouseDragRotateX = mMouseDragRotateY = 0;

        emit rotateAroundAnchor(dPhi, dTheta);
    }

    if (mMouseDragTranslateX != 0 || mMouseDragTranslateY != 0) // Engager rotation de la caméra
    {
        float dx = mMouseDragTranslateX * mMouseSensitivity;
        float dy = -mMouseDragTranslateY * mMouseSensitivity;
        mMouseDragTranslateX = mMouseDragTranslateY = 0;

        emit strafeCamera(dx, dy);
    }
    if (mMouseScroll != 0)
    {
        emit zoom(exp(-mMouseScroll*mScrollSensitivity));
        mMouseScroll = 0;
    }
    this->update();
}

RenderWidget::~RenderWidget()
{
    delete mTimer;
}