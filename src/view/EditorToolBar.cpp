//
// Created by jassoka on 7/26/26.
//

#include "view/EditorToolBar.h"

#include <qactiongroup.h>

#include "types.h"

EditorToolBar::EditorToolBar(QWidget* parent)
    : QToolBar("Editor Tools", parent)
{
    setIconSize(QSize(24, 24));
    setMovable(false);

    mToolGroup = new QActionGroup(this);
    mToolGroup->setExclusive(true);

    mNavigateAction = new QAction("Navigate", this);
    mNavigateAction->setCheckable(true);

    mSelectAction = new QAction("Select", this);
    mSelectAction->setCheckable(true);

    mToolGroup->addAction(mNavigateAction);
    mToolGroup->addAction(mSelectAction);
    addAction(mNavigateAction);
    addAction(mSelectAction);

    connect(mNavigateAction, &QAction::triggered, this, [this]() {
        emit clickedEditorTool(EditorTool::NAVIGATE);
    });
    connect(mSelectAction, &QAction::triggered, this, [this]() {
        emit clickedEditorTool(EditorTool::SELECTION);
    });

    mNavigateAction->setChecked(true); // TODO faire en sorte que ça s'active direct (pas possible actuellement car initialisé avant editorcontroller)
}
