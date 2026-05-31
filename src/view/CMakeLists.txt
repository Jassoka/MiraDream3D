add_library(View STATIC
        mainwindow.cpp
        ${DIR_INC}/view/mainwindow.h
)

target_include_directories(View PUBLIC
        ${DIR_INC}
)

target_link_libraries(View PRIVATE
        Model
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
)