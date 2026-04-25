#pragma once

#include <filesystem>
#include <string_view>

#include <glsandbox/shader.h>
#include <glsandbox/texture.h>
#include <glsandbox/window.h>

namespace glsandbox {

inline std::filesystem::path assetPath(std::string_view relativePath) {
    return std::filesystem::path(GLSANDBOX_ASSET_DIR) / relativePath;
}

}  // namespace glsandbox
