//
// Created by jassoka on 8/2/26.
//

#ifndef MIRADREAM3D_HEADERBAR_H
#define MIRADREAM3D_HEADERBAR_H

#include <QMenuBar>

#include "EditorToolBar.h"

enum class ViewportMode;
class QToolButton;

class HeaderBar: public QMenuBar
{
Q_OBJECT
public:
    explicit HeaderBar(QWidget* parent = nullptr);
    signals:
    void clickedImport(const std::string &path);
    void clickedClearScene();
    void clickedToggleGrid();
    public slots:
    void onToggledGrid(bool toggled) const;
private:
    void initLayout();
    QToolButton* mImportButton = nullptr;
    QToolButton* mClearButton = nullptr;
    QToolButton* mToggleGridButton = nullptr;

};


#endif //MIRADREAM3D_HEADERBAR_H
