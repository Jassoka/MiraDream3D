#include "controller/Engine.h"

#include <QApplication>
#include <QPushButton>
#include <QObject>
#include <filesystem>

#include "controller/AssetImporter.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile); // obligatoire sur macOS
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    Engine mainEngine;
    mainEngine.start();




    return QApplication::exec();
}
