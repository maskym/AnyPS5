#ifndef IO_FILEIDENTITY_HPP
#define IO_FILEIDENTITY_HPP

#include <filesystem>

namespace Io {

bool SameFile(const std::filesystem::path& first, const std::filesystem::path& second);
std::filesystem::path RealPath(const std::filesystem::path& path);

}

#endif
