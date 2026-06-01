//
// Created by jassoka on 6/2/26.
//

#ifndef MIRADREAM3D_RESSOURCEMANAGER_HPP
#define MIRADREAM3D_RESSOURCEMANAGER_HPP
#include <iostream>
#include <qfile.h>
#include <qtextstream.h>
#include <string>

class QTResourceManager
{
public:
    /**
     * @brief Reads a file that's been embedded by QT Resource Collection
     */
    static std::string readEmbeddedRessource(const std::string& virtualPath)
    {
        QFile file(QString::fromStdString(virtualPath));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cerr << "Can't read resource " << virtualPath << std::endl;
            exit(1);
        }
        QTextStream in(&file);
        const QString content = in.readAll();

        return content.toStdString();
    }
};

#endif //MIRADREAM3D_RESSOURCEMANAGER_HPP
