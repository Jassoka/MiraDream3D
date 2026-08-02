//
// Created by jassoka on 7/26/26.
//

#ifndef MIRADREAM3D_TOOLBAR_H
#define MIRADREAM3D_TOOLBAR_H
#include <QToolBar>

class QPushButton;
enum class EditorTool;

class EditorToolBar : public QToolBar
{
Q_OBJECT
public:
    explicit EditorToolBar(QWidget* parent = nullptr);
    ~EditorToolBar() override;
    signals:
    void clickedEditorTool(EditorTool tool);
private:
    QActionGroup* mToolGroup = nullptr;
    QAction* mNavigateAction = nullptr;
    QAction* mSelectAction = nullptr;
};


#endif //MIRADREAM3D_TOOLBAR_H
