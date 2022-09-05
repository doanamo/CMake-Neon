#pragma once

namespace Utility
{
    std::optional<std::string> ReadTextFile(const fs::path& path);
}

template<>
struct fmt::formatter<fs::path> : formatter<std::string>
{
    template<typename FormatContext>
    auto format(const fs::path& path, FormatContext& ctx)
    {
        return formatter<std::string>::format(path.generic_string(), ctx);
    }
};

