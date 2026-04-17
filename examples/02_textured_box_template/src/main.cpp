#include <glsandbox/glsandbox.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

namespace {

constexpr std::array<float, 20> kQuadVertices = {
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
};

constexpr std::array<unsigned int, 6> kQuadIndices = {0, 1, 3, 1, 2, 3};

class QuadTemplateApp {
public:
    QuadTemplateApp(int width, int height) : width_(width), height_(height) {}

    int run() {
        try {
            window_ = glsandbox::createWindow({width_, height_, "Textured Quad Template"});
            setupPipeline();
            mainLoop();
            shutdown();
            return 0;
        } catch (const std::exception& exception) {
            std::cerr << exception.what() << '\n';
            shutdown();
            return 1;
        }
    }

private:
    void setupPipeline() {
        const std::filesystem::path shaderDirectory = std::filesystem::path(GLSANDBOX_EXAMPLE_DIR) / "shaders";
        const std::string vertexShaderPath = (shaderDirectory / "box.vert").string();
        const std::string fragmentShaderPath = (shaderDirectory / "box.frag").string();
        shader_ = std::make_unique<glsandbox::Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
        texture_ = glsandbox::loadTexture2D(glsandbox::assetPath("textures/container2.png"));

        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(kQuadVertices)), kQuadVertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(kQuadIndices)), kQuadIndices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        shader_->use();
        shader_->setInt("texture1", 0);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window_)) {
            glsandbox::pollBasicInput(window_);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            renderFrame(static_cast<float>(glfwGetTime()));

            glfwSwapBuffers(window_);
            glfwPollEvents();
        }
    }

    void renderFrame(float timeInSeconds) {
        const glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(std::sin(timeInSeconds) * 0.35f, 0.0f, 0.0f));
        const glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

        shader_->use();
        shader_->setMat4("model", model);
        shader_->setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_);
        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void shutdown() {
        if (texture_ != 0) {
            glDeleteTextures(1, &texture_);
            texture_ = 0;
        }
        if (ebo_ != 0) {
            glDeleteBuffers(1, &ebo_);
            ebo_ = 0;
        }
        if (vbo_ != 0) {
            glDeleteBuffers(1, &vbo_);
            vbo_ = 0;
        }
        if (vao_ != 0) {
            glDeleteVertexArrays(1, &vao_);
            vao_ = 0;
        }
        shader_.reset();
        glsandbox::destroyWindow(window_);
        window_ = nullptr;
    }

    GLFWwindow* window_ = nullptr;
    int width_ = 800;
    int height_ = 600;
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    unsigned int ebo_ = 0;
    unsigned int texture_ = 0;
    std::unique_ptr<glsandbox::Shader> shader_;
};

}  // namespace

int main(int argc, char** argv) {
    QuadTemplateApp app(800, 600);
    return app.run();
}
