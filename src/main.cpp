#include "view/mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QObject>
#include <filesystem>

#include "controller/AssetImporter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
