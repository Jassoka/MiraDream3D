add_library(Model STATIC
        ${DIR_INC}/model/geometry.hpp
        ${DIR_INC}/model/Camera.h
        ${DIR_INC}/model/Scene.h
        Camera.cpp
        ../../include/model/Node.h
        ../../include/model/Mesh.h

        ../../include/model/Renderer.h
)

target_include_directories(Model PUBLIC
        ${DIR_INC}
        ${CMAKE_SOURCE_DIR}/libs
)
