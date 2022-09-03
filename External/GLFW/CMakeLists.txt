cmake_minimum_required(VERSION 3.23)
project(External)

# Enable debug symbols for release configuration
if(MSVC)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
endif()

add_subdirectory(Repository)
