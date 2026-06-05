#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "RenderWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void setScene(const Scene *scene)
    {
        mRenderWidget->setScene(scene);
    }
private:
    RenderWidget* mRenderWidget;
};
#endif // MAINWINDOW_H
