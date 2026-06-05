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
protected:
    // Surcharge de la fonction d'événement clavier de Qt
    void keyPressEvent(QKeyEvent *event) override;
private:
    RenderWidget* mRenderWidget;
};
#endif // MAINWINDOW_H
