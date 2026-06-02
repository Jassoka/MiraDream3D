#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "RenderWidget.h"
#include "controller/Engine.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    RenderWidget* mRenderWidget;
    Engine *mEngine;
};
#endif // MAINWINDOW_H
