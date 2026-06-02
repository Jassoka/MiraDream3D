add_library(View STATIC
        mainwindow.cpp
        ${DIR_INC}/view/mainwindow.h
        ../../include/view/RenderWidget.h
        RenderWidget.cpp
)

target_include_directories(View PUBLIC
        ${DIR_INC}
)

target_link_libraries(View PRIVATE
        Model
        assimp
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
)