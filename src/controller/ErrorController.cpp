//
// Created by Mathis Pean on 16/06/2026.
//


#include "controller/ErrorController.h"
#include "view/ErrorViewer.hpp"


void ErrorController::showError(const std::runtime_error &e,const std::string& title) {
    ErrorViewer::showErrorMsg(e,title);
}