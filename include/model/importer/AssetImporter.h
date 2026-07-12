#ifndef MIRADREAM3D_ASSETIMPORTER_H
#define MIRADREAM3D_ASSETIMPORTER_H

class SceneImport;

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
     * @param sceneOutput Destination buffer where all imported scene data is stored
     * @param warnings Reference for storing output warnings as a string
     */
    static void importFile(fileExtension ext, const std::string &path, SceneImport& sceneOutput, std::string &warnings);
};

#endif //MIRADREAM3D_ASSETIMPORTER_H