add_library(View STATIC
        MainWindow.cpp
        RenderWidget.cpp
        ../../include/view/MainWindow.h
        ../../include/view/RenderWidget.h
        ../../include/view/ErrorViewer.hpp
        ErrorViewer.cpp
        EditorToolBar.cpp
        ../../include/view/EditorToolBar.h
        HeaderBar.cpp
        ../../include/view/NodeHierarchyWidget.h
        ../../include/view/HeaderBar.h
        ../../include/view/ViewportLayout.h
        ViewportLayout.cpp
)

target_include_directories(View PRIVATE
        ${DIR_INC}
)

target_link_libraries(View PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        Qt${QT_VERSION_MAJOR}::OpenGL
)