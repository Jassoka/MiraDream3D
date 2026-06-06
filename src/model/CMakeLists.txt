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
        ../../include/model/PrimitiveMeshes.hpp
)

target_include_directories(Model PUBLIC
        ${DIR_INC}
        ${CMAKE_SOURCE_DIR}/libs
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/util

)

target_link_libraries(Model PRIVATE
        assimp
        Qt${QT_VERSION_MAJOR}::OpenGL
)
