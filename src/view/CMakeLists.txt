add_library(View STATIC
        MainWindow.cpp
        RenderWidget.cpp
        ../../include/view/MainWindow.h
        ../../include/view/RenderWidget.h
)

target_include_directories(View PUBLIC
        ${DIR_INC}
)

target_link_libraries(View PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        Qt${QT_VERSION_MAJOR}::OpenGL
)