#include "Precompiled.hpp"
#include "Utility.hpp"

std::optional<std::string> Utility::ReadTextFile(const fs::path& path)
{
    std::ifstream file(path);
    if(!file.is_open())
        return std::nullopt;

    // Allocate string buffer
    file.seekg(0, std::ios::end);
    std::string content;
    content.resize(file.tellg());

    // Read file contents
    file.seekg(0, std::ios::beg);
    file.read(content.data(), content.size());

    return content;
}