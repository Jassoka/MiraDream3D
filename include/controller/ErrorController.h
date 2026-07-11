//
// Created by Mathis Pean on 16/06/2026.
//

#ifndef MIRADREAM3D_ERRORCONTROLLER_H
#define MIRADREAM3D_ERRORCONTROLLER_H
#include <stdexcept>
#include <string>

/**
 * @brief Controller class for managing runtime errors
 */
class ErrorController {
public:
    /**
     * @brief Calls the view to show warnings
     * @param warnings String containing the warnings
     * @param title Pop-up title
     */
    static void showWarnings(const std::string &warnings,const std::string& title="Warning");
    /**
     * @brief Calls the view to show errors
     * @param e Instance of exception
     * @param title Pop-up title
     */
    static void showError(const std::runtime_error &e,const std::string& title="Error");
};



#endif //MIRADREAM3D_ERRORCONTROLLER_H
