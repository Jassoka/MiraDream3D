add_library(Model STATIC
        ${DIR_INC}/model/geometry.hpp
        ${DIR_INC}/model/Camera.h
        ${DIR_INC}/model/Scene.h
        Camera.cpp
        ../../include/model/Node.h
        ../../include/model/Mesh.h

        ../../include/model/Renderer.h
        ../../include/model/ShaderManager.h
        ../../include/model/Shader.h
        ShaderManager.cpp
        Renderer.cpp
        Scene.cpp
        Shader.cpp
        Node.cpp
        Mesh.cpp
)

target_include_directories(Model PUBLIC
        ${DIR_INC}
        ${CMAKE_SOURCE_DIR}/include/util
        ${CMAKE_SOURCE_DIR}/libs
)

target_link_libraries(Model PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        assimp

)

target_link_libraries(Model PUBLIC
        Qt${QT_VERSION_MAJOR}::Gui # ça prend ptetre trop de place -> revoir le constructeur d'une texture qui prend un Qimage
)