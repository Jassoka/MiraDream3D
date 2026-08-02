//
// Created by jassoka on 8/2/26.
//

#ifndef MIRADREAM3D_QT_UTILS_H
#define MIRADREAM3D_QT_UTILS_H
#include <QToolButton>


namespace QtUtils
{
    static QToolButton* createToolButton(QWidget *object, QAction *action, const int width = 24, const int height = 24)
    {
        auto *btn = new QToolButton(object);
        btn->setDefaultAction(action);
        btn->setIconSize(QSize(width, height));
        return btn;
    }
}

#endif //MIRADREAM3D_QT_UTILS_H
