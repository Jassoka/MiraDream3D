#include "view/mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QObject>

#include "controller/loadFile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Scene scene = Scene();
    loadFile("/Users/mathispean/Desktop/Programmation/c++/Projet_C_2/MiraDream3D/testObjects/cube.obj",scene);
    return QApplication::exec();
}
