#include "controller/Engine.h"

#include <QApplication>
#include <QPushButton>
#include <QObject>
#include <filesystem>
#include <QSurfaceFormat>


#ifdef ENABLE_RENDERDOC
    #include "RenderDocHelper.hpp"
    #include <QLibrary>
    #include <QDebug>
#endif

int main(int argc, char *argv[])
{
#ifdef ENABLE_RENDERDOC
    qDebug() << "DEBUG mode";
    initRenderDoc();
#endif
#ifdef TEST_HALFEDGES
    qDebug() << "half edges test";
#endif

#ifdef Q_OS_LINUX
    // Check if the user is running a Wayland session
    QByteArray sessionType = qgetenv("XDG_SESSION_TYPE");
    QByteArray waylandDisplay = qgetenv("WAYLAND_DISPLAY");

    if (sessionType == "wayland" || !waylandDisplay.isEmpty()) {
        // Force Qt to use XWayland (X11 compatibility layer)
        qputenv("QT_QPA_PLATFORM", "xcb");
    }
#endif

    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile); // obligatoire sur macOS
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    Engine mainEngine;
    mainEngine.start();




    return QApplication::exec();
}
