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
        ${CMAKE_SOURCE_DIR}/libs
        PRIVATE
        ${DIR_INC}
)

#ASSIMP
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp_build)

target_compile_options(assimp PRIVATE
        -Wno-error
        -Wno-unknown-warning-option
)



target_link_libraries(Model PUBLIC
        Qt${QT_VERSION_MAJOR}::OpenGL
        PRIVATE
        assimp
)
