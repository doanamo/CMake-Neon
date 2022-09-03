cmake_minimum_required(VERSION 3.23)
project("Infiltrator")

set(SOURCE_FILES
    "Source/Main.cpp"
)

add_executable(Game ${SOURCE_FILES})
set_property(TARGET Game PROPERTY CXX_STANDARD 20)
