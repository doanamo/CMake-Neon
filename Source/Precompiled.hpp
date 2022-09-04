#pragma once

// External/fmt
#include <fmt/format.h>

// External/spdlog
#ifdef NDEBUG
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#else
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// External/Eigen
#include <Eigen/Dense>

// External/glad
#include <glad/gl.h>

// External/GLFW
#include <GLFW/glfw3.h>
