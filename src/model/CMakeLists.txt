add_library(Model STATIC
        ${DIR_INC}/model/geometry.hpp
        ${DIR_INC}/model/Camera.h
        ${DIR_INC}/model/graphicPipeline.h
        ${DIR_INC}/model/Projection.h
        ${DIR_INC}/model/Scene.h
        Projection.cpp
        Camera.cpp
        ../../include/model/Object.h
        ../../include/model/Mesh.h
        ../../include/model/texture_types.hpp
)

target_include_directories(Model PUBLIC
        ${DIR_INC}
        ${CMAKE_SOURCE_DIR}/libs
)