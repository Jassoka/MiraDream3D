# Cmake vide
add_library(Controller INTERFACE)
target_include_directories(Controller INTERFACE ${DIR_INC})

target_link_libraries(Controller INTERFACE
        Model
        View
        Qt${QT_VERSION_MAJOR}::Widgets
)