cmake_minimum_required(VERSION 3.23)
project("Infiltrator")

#
# Sources
#

set(SOURCE_FILES
    "Source/Main.cpp"
    "Source/Precompiled.hpp"
)

#
# Executable
#

add_executable(Game ${SOURCE_FILES})
target_include_directories(Game PRIVATE "Source")
target_precompile_headers(Game PRIVATE "Source/Precompiled.hpp")

set_target_properties(Game PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    UNITY_BUILD ON
)

if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(Game PUBLIC "/ZI")
    target_link_options(Game PUBLIC "/INCREMENTAL")
endif()

#
# External
#

# External/glad
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/glad/Include")
target_link_libraries(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/glad/Library/Windows/$<CONFIG>/glad.lib")

# External/GLFW
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/GLFW/Include")
target_link_libraries(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/GLFW/Library/Windows/$<CONFIG>/glfw3.lib")
