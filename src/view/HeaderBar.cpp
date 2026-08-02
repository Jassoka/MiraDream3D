//
// Created by jassoka on 8/2/26.
//

#include "../../include/view/HeaderBar.h"

#include <QActionGroup>
#include <QFileDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QToolButton>
#include <QFrame>

#include "types.h"

HeaderBar::HeaderBar(QWidget* parent):
    QMenuBar(parent)
{
    setNativeMenuBar(false);
    const QString modernHeaderStyle = R"(
    QToolButton {
        background-color: transparent;
        color: #cccccc;
        border: none;
        outline: none;
        border-radius: 4px;
        padding: 4px 10px;
    }

    QToolButton:hover {
        background-color: rgba(255, 255, 255, 0.08);
        color: #ffffff;
    }

    QToolButton:pressed {
        background-color: rgba(0, 120, 215, 0.25);
        color: #ffffff;
    }
    )";


    this->setStyleSheet(modernHeaderStyle);

    mImportButton = new QToolButton(this);
    mImportButton->setText("&Import file");
    connect(mImportButton, &QToolButton::clicked, this, [this]()
    {
        const QString path = QFileDialog::getOpenFileName(
            this,
            "Import a file",
            "",
            "3D Models (*.obj);;All file types (*)"
        );
        if (!path.isEmpty()) {
            emit clickedImport(path.toStdString());
        }
    });

    mClearButton = new QToolButton(this);
    mClearButton->setText("&Clear");
    connect(mClearButton, &QToolButton::clicked, this, [this]()
    {
       emit clickedClearScene();
    });


    mToggleGridButton = new QToolButton(this);
    mToggleGridButton->setText("&Toggle Grid");
    connect(mToggleGridButton, &QToolButton::clicked, this, [this]()
    {
       emit clickedToggleGrid();
    });


    initLayout();
}

void HeaderBar::onToggledGrid(const bool toggled) const
{
    if (toggled)
        mToggleGridButton->setText("&Toggle grid (On)");
    else
        mToggleGridButton->setText("&Toggle grid (Off)");
    mToggleGridButton->updateGeometry();
    if (mToggleGridButton->parentWidget()) {
        mToggleGridButton->parentWidget()->adjustSize();
    }
}

void HeaderBar::initLayout()
{
    auto *buttonsContainer = new QWidget(this);

    auto *buttonsLayout = new QHBoxLayout(buttonsContainer);
    buttonsLayout->setContentsMargins(8, 0, 8, 0);
    buttonsLayout->setSpacing(4);

    mImportButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    buttonsLayout->addWidget(mImportButton);

    auto *divider = new QFrame(buttonsContainer);
    divider->setFrameShape(QFrame::VLine);
    divider->setStyleSheet("QFrame { color: #3c3c3c; margin: 4px 2px; }");
    buttonsLayout->addWidget(divider);

    mClearButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    buttonsLayout->addWidget(mClearButton);


    divider = new QFrame(buttonsContainer);
    divider->setFrameShape(QFrame::VLine);
    divider->setStyleSheet("QFrame { color: #3c3c3c; margin: 4px 2px; }");
    buttonsLayout->addWidget(divider);

    mToggleGridButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    buttonsLayout->addWidget(mToggleGridButton);

    this->setCornerWidget(buttonsContainer, Qt::TopLeftCorner);
}


