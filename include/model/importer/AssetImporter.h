#ifndef MIRADREAM3D_ASSETIMPORTER_H
#define MIRADREAM3D_ASSETIMPORTER_H

enum fileExtension
{
    OBJ
};

class Scene;
/**
 * @brief Class which implements static methods for assert importation within the software
 */
class AssetImporter
{
public:
    /**
     * @brief Loads a scene from any file
     * @param ext File extension
     * @param path Path of the source file
     * @param scene Destination scene to which is appended the imported scene
     * @param warnings Reference for storing output warnings as a string
     */
    static void importFile(fileExtension ext, const std::string &path, Scene* scene, std::string &warnings);
};

#endif //MIRADREAM3D_ASSETIMPORTER_H