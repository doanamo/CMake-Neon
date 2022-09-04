cmake_minimum_required(VERSION 3.23)
project("Infiltrator")

#
# Options
#

# Require specific C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable fast math
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
endif()

# Enable edit and continue for debug configuration
if(MSVC)
    string(REPLACE "/Zi" "/ZI" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()

# Enable debug symbols for release configuration
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

#
# Sources
#

set(SOURCE_FILES
    "Source/Main.cpp"
    "Source/Precompiled.hpp"
)

#
# Target
#

# Executable target
add_executable(Game ${SOURCE_FILES})
target_include_directories(Game PRIVATE "Source")

# Enable precompiled header
target_precompile_headers(Game PRIVATE "Source/Precompiled.hpp")

# Enable unity build
set_property(TARGET Game PROPERTY UNITY_BUILD ON)

#
# External
#

# External/fmt
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/fmt/Include")
target_link_libraries(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/fmt/Library/Windows/$<CONFIG>/fmt.lib")

# External/spdlog
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/spdlog/Include")
target_link_libraries(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/spdlog/Library/Windows/$<CONFIG>/spdlog.lib")
target_compile_definitions(Game PRIVATE SPDLOG_FMT_EXTERNAL SPDLOG_COMPILED_LIB)

# External/Eigen
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/Eigen/Include")

# External/glad
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/glad/Include")
target_link_libraries(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/glad/Library/Windows/$<CONFIG>/glad.lib")

# External/GLFW
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/GLFW/Include")
target_link_libraries(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/GLFW/Library/Windows/$<CONFIG>/glfw.lib")
