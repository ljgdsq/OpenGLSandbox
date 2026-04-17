#pragma once

#include <filesystem>

namespace glsandbox {

unsigned int loadTexture2D(const std::filesystem::path& texturePath, bool flipVertically = true);

}  // namespace glsandbox
