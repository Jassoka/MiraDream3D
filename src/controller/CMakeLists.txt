
add_library(Controller STATIC
        ../../include/controller/RenderController.h
        ../../include/controller/SceneController.h
        ../../include/controller/Engine.h
        SceneController.cpp
        Engine.cpp
        RenderController.cpp
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
)