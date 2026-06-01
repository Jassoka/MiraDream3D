
#include <iostream>
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>

void loadFile(std::string file) {

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(file,
        //aiProcess_Triangulate
                0
    );

    if (!scene) {
        std::cerr << importer.GetErrorString() << std::endl;
    }
}

