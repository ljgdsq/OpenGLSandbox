#include <glsandbox/texture.h>

#include <glad/glad.h>
#include <stb_image.h>

#include <stdexcept>
#include <string>

namespace glsandbox {

namespace {

GLenum channelFormat(int channels) {
    switch (channels) {
    case 1:
        return GL_RED;
    case 3:
        return GL_RGB;
    case 4:
        return GL_RGBA;
    default:
        throw std::runtime_error("Unsupported texture format: expected 1, 3, or 4 channels.");
    }
}

}  // namespace

unsigned int loadTexture2D(const std::filesystem::path& texturePath, bool flipVertically) {
    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* pixels = stbi_load(texturePath.string().c_str(), &width, &height, &channels, 0);
    if (pixels == nullptr) {
        throw std::runtime_error(std::string("Failed to load texture: ") + texturePath.string());
    }

    const GLenum format = channelFormat(channels);

    unsigned int textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);
    return textureId;
}

}  // namespace glsandbox
