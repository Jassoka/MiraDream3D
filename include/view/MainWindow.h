#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ViewportLayout;
class HeaderBar;
enum class ViewportMode;
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
    void clearSceneRequested();
    void toggleGridRequested();
    void onToggledGrid(bool toggled);
    void changedEditorTool(EditorTool tool);
    void changedViewportMode(ViewportMode mode);

private:
    RenderWidget* mRenderWidget;
    //QPushButton* mImportButton;
    EditorToolBar *mEditorToolBar;
    HeaderBar *mHeaderBar;
    ViewportLayout *mViewportLayout;
};
#endif // MAINWINDOW_H
