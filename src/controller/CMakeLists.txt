
add_library(Controller STATIC
        loadFile.cpp
        ../../include/controller/loadFile.h
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

)

target_link_libraries(Controller PUBLIC assimp::assimp)