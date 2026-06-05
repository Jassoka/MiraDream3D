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
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);


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
    mRenderer->initShaders();
}


void RenderWidget::timeOutSlot() {
}

RenderWidget::~RenderWidget()
{
    delete mRenderer;
    delete mTimer;
}