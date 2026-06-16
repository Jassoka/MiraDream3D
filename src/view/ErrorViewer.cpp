//
// Created by Mathis Pean on 16/06/2026.
//
#include <QMessageBox>
#include "view/ErrorViewer.hpp"

 void ErrorViewer::showErrorMsg(const std::runtime_error &e,const std::string& title="Error")
{
    QMessageBox::warning(nullptr,QString::fromStdString(title),e.what());
}