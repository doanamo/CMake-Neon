cmake_minimum_required(VERSION 3.23)
project("Game-Neon")

#
# Options
#

# Require specific C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Treat warnings as errors
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

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

# Disable console window for release configuration
if(MSVC)
    set(CMAKE_WIN32_EXECUTABLE $<$<CONFIG:Release>:ON>)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /ENTRY:mainCRTStartup")
endif()

# Define available configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Enable folders in Visual Studio solution
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

#
# Files
#

set(SOURCE_FILES
    "Source/Main.cpp"
    "Source/Precompiled.hpp"
    "Source/Application.hpp"
    "Source/Application.cpp"
    "Source/Common/Compiler.hpp"
    "Source/Common/Debug.hpp"
    "Source/Common/Logger.hpp"
    "Source/Common/Logger.cpp"
    "Source/Common/ScopeGuard.hpp"
    "Source/Common/Utility.hpp"
    "Source/Common/Utility.cpp"
    "Source/System/Image.hpp"
    "Source/System/Image.cpp"
    "Source/Graphics/OpenGL.hpp"
    "Source/Graphics/OpenGL.cpp"
    "Source/Graphics/Buffer.hpp"
    "Source/Graphics/Buffer.cpp"
    "Source/Graphics/VertexArray.hpp"
    "Source/Graphics/VertexArray.cpp"
    "Source/Graphics/Shader.hpp"
    "Source/Graphics/Shader.cpp"
    "Source/Graphics/Texture.hpp"
    "Source/Graphics/Texture.cpp"
)

set(DEPLOY_FILES
    "Deploy/Data/Shaders/vertex_color_vs.glsl"
    "Deploy/Data/Shaders/vertex_color_fs.glsl"
    "Deploy/Data/Shaders/vertex_texture_vs.glsl"
    "Deploy/Data/Shaders/vertex_texture_fs.glsl"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${DEPLOY_FILES})

#
# Target
#

# Executable target
add_executable(Game ${SOURCE_FILES} ${DEPLOY_FILES})
target_include_directories(Game PRIVATE "Source")

# Enable precompiled header
target_precompile_headers(Game PRIVATE "Source/Precompiled.hpp")
source_group("Generated\\Precompiled" REGULAR_EXPRESSION "cmake_pch")

# Enable unity build
set_property(TARGET Game PROPERTY UNITY_BUILD ON)
source_group("Generated\\Unity" REGULAR_EXPRESSION "unity_")

# Set as startup project
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)

# Write path to deploy directory for debugging
get_filename_component(ABSOLUTE_DEPLOY_PATH "Deploy" ABSOLUTE)
file(WRITE "${CMAKE_BINARY_DIR}/DeployPath.txt" ${ABSOLUTE_DEPLOY_PATH})

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

# External/zlib
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/zlib/Include")
target_link_libraries(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/zlib/Library/Windows/$<CONFIG>/zlib.lib")

# External/libpng
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/libpng/Include")
target_link_libraries(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/libpng/Library/Windows/$<CONFIG>/libpng.lib")

# External/glm
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/glm/Include")

# External/glad
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/glad/Include")
target_link_libraries(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/glad/Library/Windows/$<CONFIG>/glad.lib")

# External/GLFW
target_include_directories(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/GLFW/Include")
target_link_libraries(Game PRIVATE "${PROJECT_SOURCE_DIR}/External/GLFW/Library/Windows/$<CONFIG>/glfw.lib")
