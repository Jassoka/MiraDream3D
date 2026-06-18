//
// Created by Mathis Pean on 16/06/2026.
//

#ifndef MIRADREAM3D_ERRORCONTROLLER_H
#define MIRADREAM3D_ERRORCONTROLLER_H
#include <stdexcept>
#include <string>
#include <stdexcept>
class ErrorController {

public:
    static void showError(const std::runtime_error &e,const std::string& title="Error");
};



#endif //MIRADREAM3D_ERRORCONTROLLER_H
