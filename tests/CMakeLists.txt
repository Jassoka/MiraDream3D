include(GoogleTest)

# Utility functions for testing
add_library(TestUtils STATIC
        testUtils/MeshComparison.h
)

target_include_directories(TestUtils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${DIR_INC}
)
target_link_libraries(TestUtils PRIVATE
        Model)

# Module for general testing
add_executable(AllTests
        ImporterTests/SmoothTangentsImportTest.cpp
)

target_include_directories(AllTests PRIVATE ${DIR_INC})

target_compile_definitions(AllTests PRIVATE
        TEST_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/testAssets"
)

target_link_libraries(AllTests PRIVATE
        Model
        View
        Controller
        TestUtils
        GTest::gtest_main)

gtest_discover_tests(AllTests)