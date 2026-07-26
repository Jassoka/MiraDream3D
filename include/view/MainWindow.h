#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

enum class EditorTool;
class EditorToolBar;
class RenderWidget;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    RenderWidget *getRenderWidget() const {return mRenderWidget;}
public slots:
    //void timeoutControl();
    signals:
    void importSceneRequested(const std::string &path);
    void changedEditorTool(EditorTool tool);
private:
    RenderWidget* mRenderWidget;
    QPushButton* mImportButton;
    EditorToolBar *mEditorToolBar;
};
#endif // MAINWINDOW_H
