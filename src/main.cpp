#include "view/mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QObject>
#include <filesystem>

#include "controller/AssetImporter.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Scene scene = Scene(); //TODO degager cette merde

    //TODO oblitérer ce main de la planete terre
    //fs::path chemin = fs::path(PROJECT_PATH) / "testAssets" / "cube_smoothNorm.obj";
    //loadFile(chemin.string(), scene);

    return QApplication::exec();
}
