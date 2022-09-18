#pragma once

// Standard
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

// Common
#include "Common/Compiler.hpp"
#include "Common/Logger.hpp"
#include "Common/Debug.hpp"

// Disable all warnings for external headers
// This also guards against leaking warning pragmas
COMPILER_WARNINGS_DISABLE_ALL_PUSH()

// External/fmt
#include <fmt/format.h>

// External/spdlog
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// External/zlib
#include <zlib.h>

// External/glm
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

// External/glad
#include <glad/gl.h>

// External/GLFW
#include <GLFW/glfw3.h>

// Restore disabled warnings
COMPILER_WARNINGS_DISABLE_ALL_POP()

// Common
#include "Common/ScopeGuard.hpp"
#include "Common/Utility.hpp"
