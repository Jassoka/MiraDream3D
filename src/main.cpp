#include "controller/Engine.h"

#include <QApplication>
#include <QPushButton>
#include <QObject>
#include <filesystem>

#include "controller/AssetImporter.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Engine mainEngine;
    mainEngine.start();

    return QApplication::exec();
}
