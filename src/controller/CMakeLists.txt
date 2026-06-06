
add_library(Controller STATIC
        ../../include/controller/SceneController.h
        ../../include/controller/Engine.h
        SceneController.cpp
        Engine.cpp
)
target_include_directories(Controller PUBLIC
        ${DIR_INC}
        ${CMAKE_SOURCE_DIR}/libs
)

target_link_libraries(Controller PRIVATE
        Model
        View
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        Qt${QT_VERSION_MAJOR}::OpenGL

target_link_libraries(Controller PUBLIC assimp::assimp)