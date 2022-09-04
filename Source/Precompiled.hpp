#pragma once

// Standard
#include <functional>

// Common
#include "Common/Compiler.hpp"
#include "Common/Logger.hpp"
#include "Common/Debug.hpp"
#include "Common/ScopeGuard.hpp"

// Disable all warnings for external headers
// This also guards against leaking warning pragmas
COMPILER_WARNINGS_DISABLE_ALL_PUSH()

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

// Restore disabled warnings
COMPILER_WARNINGS_DISABLE_ALL_POP()
