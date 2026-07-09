//
// Created by Mathis Pean on 16/06/2026.
//


#include "controller/ErrorController.h"
#include "view/ErrorViewer.hpp"

void ErrorController::showWarnings(const std::string& warnings, const std::string& title)
{
    ErrorViewer::showErrorMsg(warnings, title);
}

void ErrorController::showError(const std::runtime_error &e,const std::string& title) {
    ErrorViewer::showErrorMsg(e.what(),title);
}