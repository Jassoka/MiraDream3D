//
// Created by Mathis Pean on 16/06/2026.
//
#include <QMessageBox>
#include "view/ErrorViewer.hpp"

void ErrorViewer::showErrorMsg(const std::string &msg,const std::string &title)
{
    QMessageBox::warning(nullptr,QString::fromStdString(title),QString::fromStdString(msg));
}