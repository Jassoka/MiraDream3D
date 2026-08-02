//
// Created by jassoka on 8/2/26.
//

#include "../../include/view/ViewportLayout.h"

#include "util/qt_utils.h"
#include "types.h"

#include <QActionGroup>
#include <QBoxLayout>
#include <QFrame>
#include <QMouseEvent>

constexpr int iconSize = 32;

ViewportLayout::ViewportLayout(QWidget* parent):
    QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setFocusPolicy(Qt::StrongFocus);

    initActions();

    auto *hudPanel = new QFrame(this);

    hudPanel->setStyleSheet(
        "QFrame { "
        "   background-color: rgba(30, 30, 30, 200); "
        "   border-radius: 6px; "
        "}"
    );

    auto *hudLayout = new QVBoxLayout(hudPanel);
    hudLayout->setContentsMargins(4, 4, 4, 4);
    hudLayout->setSpacing(2);

    hudLayout->addWidget(QtUtils::createToolButton(hudPanel, mWireFrame, iconSize, iconSize));
    hudLayout->addWidget(QtUtils::createToolButton(hudPanel, mSolid, iconSize, iconSize));
    hudLayout->addWidget(QtUtils::createToolButton(hudPanel, mMaterial, iconSize, iconSize));

    auto *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(10, 10, 10, 10);
    outerLayout->addWidget(hudPanel, 0, Qt::AlignTop | Qt::AlignRight);
}


void ViewportLayout::initActions()
{
    mViewportModes = new QActionGroup(this);
    mViewportModes->setExclusive(true);

    mWireFrame = new QAction(QIcon(":/icons/wireframe.png"), "Wireframe", this);
    mWireFrame->setCheckable(true);

    mSolid = new QAction(QIcon(":/icons/solid.png"), "Solid", this);
    mSolid->setCheckable(true);

    mMaterial = new QAction(QIcon(":/icons/material.png"),"Material", this);
    mMaterial->setCheckable(true);

    mViewportModes->addAction(mSolid);
    mViewportModes->addAction(mWireFrame);
    mViewportModes->addAction(mMaterial);

    connect(mWireFrame, &QAction::triggered, this, [this]() {
        emit clickedViewportMode(ViewportMode::WIREFRAME);
    });
    connect(mSolid, &QAction::triggered, this, [this]() {
        emit clickedViewportMode(ViewportMode::SOLID);
    });
    connect(mMaterial, &QAction::triggered, this, [this]() {
        emit clickedViewportMode(ViewportMode::MATERIAL);
    });

    mSolid->setChecked(true);
}


void ViewportLayout::onViewportChange(const ViewportMode mode) const
{
    switch (mode) {
    case ViewportMode::WIREFRAME: mWireFrame->setChecked(true); break;
    case ViewportMode::SOLID:     mSolid->setChecked(true);     break;
    case ViewportMode::MATERIAL:  mMaterial->setChecked(true);  break;
    }
}

void ViewportLayout::mousePressEvent(QMouseEvent* event)
{
    this->setFocus(Qt::MouseFocusReason);
    event->accept();
}
