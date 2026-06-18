cmake_minimum_required(VERSION 3.16)

project(MiraDream3D VERSION 0.1 LANGUAGES CXX)

# Emplacements des répertoires utiles
set(DIR_INC ${CMAKE_SOURCE_DIR}/include)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gestion des assets avec QT
set(CMAKE_AUTORCC ON)

#Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets Gui)


add_library(EngineCore STATIC)

# Mode debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(EngineCore PUBLIC
            src/RenderDocHelper.cpp)
    add_compile_definitions(ENABLE_DEBUG)
    add_compile_definitions(TEST_HALFEDGES)
    add_compile_definitions(ENABLE_RENDERDOC)
endif()

# Chemin vers les modules
add_subdirectory(src/model)
add_subdirectory(src/view)
add_subdirectory(src/controller)


set(PROJECT_SOURCES
        src/main.cpp
        include/model/MeshTopologyBuilder.h
        include/model/MeshBuilder.h
)

qt_add_executable(MiraDream3D
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)


# Précompilation

target_precompile_headers(EngineCore PRIVATE
        <vector>
        <string>
        <unordered_map>
        <glm/glm.hpp>
        <QOpenGLFunctions>
)



# Liste des modules
target_link_libraries(EngineCore PUBLIC
        Model
        View
        Controller
)

target_include_directories(EngineCore PUBLIC
        ${DIR_INC}
)

target_sources(EngineCore PUBLIC
        resources.qrc)

target_link_libraries(MiraDream3D PUBLIC EngineCore)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(Qt6_VERSION VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MiraDream3D)
endif()
set_target_properties(MiraDream3D PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MiraDream3D
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(MiraDream3D)


# Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(tests)