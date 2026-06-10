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

#ASSIMP
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)

#Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Gui )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)

option(TEST_HALFEDGES "Active les tests halfedges" OFF)
set(TEST_HALFEDGES OFF CACHE BOOL "" FORCE)

if(TEST_HALFEDGES)
    add_compile_definitions(TEST_HALFEDGES)
endif()

# Chemin vers les modules
add_subdirectory(src/model)
add_subdirectory(src/view)
add_subdirectory(src/controller)


add_subdirectory(libs/assimp)

target_compile_options(assimp PRIVATE
        -Wno-error
        -Wno-unknown-warning-option
)
set(PROJECT_SOURCES
        src/main.cpp
        resources.qrc
        include/RenderDocHelper.hpp
        src/RenderDocHelper.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MiraDream3D
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MiraDream3D APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MiraDream3D SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MiraDream3D
            ${PROJECT_SOURCES}
        )
    endif()
endif()

add_library(EngineCore STATIC)

# Précompilation

target_precompile_headers(EngineCore PRIVATE
        <vector>
        <string>
        <assimp>
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

target_link_libraries(MiraDream3D PRIVATE EngineCore)

# target_link_libraries(MiraDream3D PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
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

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MiraDream3D)
endif()


# Mode debug avec RenderDoc
add_executable(debugMain ${PROJECT_SOURCES})
target_link_libraries(debugMain PRIVATE EngineCore)
target_compile_definitions(debugMain PRIVATE DEBUG)
target_compile_definitions(debugMain PRIVATE ENABLE_RENDERDOC)

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