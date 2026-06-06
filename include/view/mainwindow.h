#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "RenderWidget.h"
class Engine;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr,Engine* engine=nullptr);
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
    QPushButton* mImportButton;
    Engine* mEngine;
};
#endif // MAINWINDOW_H
