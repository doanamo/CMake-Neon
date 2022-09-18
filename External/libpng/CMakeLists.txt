cmake_minimum_required(VERSION 3.23)
project(External)

# Enable debug symbols for release configuration
if(MSVC)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
endif()

include_directories("../zlib/Include")
add_subdirectory(Repository)
