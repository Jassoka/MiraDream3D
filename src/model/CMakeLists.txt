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
        AssetImporter.cpp
        ../../include/model/AssetImporter.h
        ../../include/model/PrimitiveMeshes.h
        PrimitiveMeshes.cpp
)

target_include_directories(Model PUBLIC
        ${DIR_INC}
        ${CMAKE_SOURCE_DIR}/libs
        PRIVATE

)

target_link_libraries(Model PUBLIC
        Qt${QT_VERSION_MAJOR}::OpenGL
        PRIVATE
        assimp
)
