#include "controller/Engine.h"

#include <QApplication>
#include <QPushButton>
#include <QObject>
#include <filesystem>
#include <qdir.h>
#include <qresource.h>
#include <QSurfaceFormat>

#ifdef ENABLE_RENDERDOC
    #include "RenderDocHelper.hpp"
    #include <QLibrary>
    #include <QDebug>
#endif

int main(int argc, char *argv[])
{
#ifdef TEST_HALFEDGES
    qDebug() << "Half edges test";
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

    // rcc loading
    const QString rccPath = QDir(QCoreApplication::applicationDirPath()).filePath("icons.rcc");

    if (!QResource::registerResource(rccPath)) {
        qWarning() << "Failed to load icons.rcc from:" << rccPath;
    }

    Engine mainEngine;
    mainEngine.start();
    return a.exec();
}
