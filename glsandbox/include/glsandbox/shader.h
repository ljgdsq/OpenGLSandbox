#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace glsandbox {

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath)
        : programId_(createProgram(vertexPath, fragmentPath)) {}

    ~Shader() {
        if (programId_ != 0) {
            glDeleteProgram(programId_);
        }
    }

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept : programId_(other.programId_) {
        other.programId_ = 0;
    }

    Shader& operator=(Shader&& other) noexcept {
        if (this != &other) {
            if (programId_ != 0) {
                glDeleteProgram(programId_);
            }
            programId_ = other.programId_;
            other.programId_ = 0;
        }
        return *this;
    }

    void use() const {
        glUseProgram(programId_);
    }

    GLuint id() const {
        return programId_;
    }

    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(programId_, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(programId_, name.c_str()), value);
    }

    void setMat4(const std::string& name, const glm::mat4& matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(programId_, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
    }

private:
    static std::string readTextFile(const char* path) {
        std::ifstream input(path, std::ios::in | std::ios::binary);
        if (!input) {
            throw std::runtime_error(std::string("Failed to open shader file: ") + path);
        }

        std::ostringstream buffer;
        buffer << input.rdbuf();
        return buffer.str();
    }

    static GLuint compileShader(GLenum type, const char* label, const std::string& source) {
        const char* rawSource = source.c_str();
        const GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &rawSource, nullptr);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_TRUE) {
            return shader;
        }

        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(static_cast<std::size_t>(logLength), '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        glDeleteShader(shader);

        throw std::runtime_error(std::string("Failed to compile ") + label + " shader:\n" + log);
    }

    static GLuint createProgram(const char* vertexPath, const char* fragmentPath) {
        const std::string vertexSource = readTextFile(vertexPath);
        const std::string fragmentSource = readTextFile(fragmentPath);

        const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "vertex", vertexSource);
        const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "fragment", fragmentSource);

        const GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        GLint success = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (success == GL_TRUE) {
            return program;
        }

        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(static_cast<std::size_t>(logLength), '\0');
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        glDeleteProgram(program);

        throw std::runtime_error(std::string("Failed to link shader program:\n") + log);
    }

    GLuint programId_ = 0;
};

}  // namespace glsandbox
