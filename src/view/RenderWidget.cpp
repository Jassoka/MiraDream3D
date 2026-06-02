#include "view/RenderWidget.h"
#include <QOpenGLFunctions>

RenderWidget::RenderWidget(const Scene *scene, int framesPerSecond, QWidget *parent) : QOpenGLWidget(parent) {
    this->mRenderer = new Renderer(this->width()/this->height(), QOpenGLContext::currentContext()->functions());
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
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    //setupShaders
    //setupBuffers

    //vao.create(); vao.bind();
    //vbo.create(); vbo.bind();

}
void RenderWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);


}
void RenderWidget::paintGL() {

}


void RenderWidget::timeOutSlot() {
}

RenderWidget::~RenderWidget()
{
    delete mRenderer;
    delete mTimer;
}