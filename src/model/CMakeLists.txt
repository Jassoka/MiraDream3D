add_library(Model STATIC
        ${DIR_INC}/model/geometry.hpp
        ${DIR_INC}/model/Camera.h
        ${DIR_INC}/model/Scene.h
        Camera.cpp
        ../../include/model/Node.h
        ../../include/model/Mesh.h
        ../../include/model/Renderer.h
        ../../include/model/ShaderManager.h
        ShaderManager.cpp
        Renderer.cpp
        Scene.cpp
        Node.cpp
        Mesh.cpp
        importer/AssetImporter.cpp
        ../../include/model/importer/AssetImporter.h
        importer/ObjParser.cpp
        ../../include/model/importer/ObjParser.hpp
        ../../include/model/PrimitiveMeshes.h
        PrimitiveMeshes.cpp
        MeshTopologyBuilder.cpp
        MeshBuilder.cpp
        TextureManager.cpp
        importer/MtlParser.cpp
        ../../include/model/importer/MtlParser.hpp
        parser/Parser.cpp
        ../../include/model/parser/Parser.h
        parser/Lexer.cpp
        ../../include/model/parser/Lexer.h
        SceneImport.cpp
        importer/ImportParser.cpp
        ShaderParser.cpp
        Editor.cpp
)

target_include_directories(Model PUBLIC
        ${CMAKE_SOURCE_DIR}/libs
        PRIVATE
        ${DIR_INC}
)

target_link_libraries(Model PUBLIC
        Qt${QT_VERSION_MAJOR}::OpenGL
        PRIVATE
)
