#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>

#include <glsandbox/glsandbox.h>

#include <array>
#include <filesystem>
#include <iostream>

namespace {

constexpr std::array<float, 20> kQuadVertices = {
    0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
};

constexpr std::array<unsigned int, 6> kQuadIndices = {0, 1, 3, 1, 2, 3};

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

}  // namespace

int main() {
    try {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

        GLFWwindow* window = glfwCreateWindow(800, 600, "Textured Quad Minimal", nullptr, nullptr);
        if (window == nullptr) {
            throw std::runtime_error("Failed to create GLFW window.");
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
            throw std::runtime_error("Failed to initialize GLAD.");
        }

        glViewport(0, 0, 800, 600);

        const std::filesystem::path shaderDirectory = std::filesystem::path(GLSANDBOX_EXAMPLE_DIR) / "shaders";
        const std::string vertexShaderPath = (shaderDirectory / "box.vert").string();
        const std::string fragmentShaderPath = (shaderDirectory / "box.frag").string();
        Shader shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

        unsigned int vao = 0;
        unsigned int vbo = 0;
        unsigned int ebo = 0;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(kQuadVertices)), kQuadVertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(kQuadIndices)), kQuadIndices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        const unsigned int texture = glsandbox::loadTexture2D(glsandbox::assetPath("textures/container2.png"));

        shader.use();
        shader.setInt("texture1", 0);

        while (!glfwWindowShouldClose(window)) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteTextures(1, &texture);
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        glfwTerminate();
        return 1;
    }
}
