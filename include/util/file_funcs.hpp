//
// Created by jassoka on 6/1/26.
//

#ifndef MIRADREAM3D_FILE_FUNCS_H
#define MIRADREAM3D_FILE_FUNCS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

inline std::string readFileToString(const std::string &path)
{
    std::ifstream file(path);
    if (!file) {
        std::cerr << path << "does not exist." << "\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

#endif //MIRADREAM3D_FILE_FUNCS_H
