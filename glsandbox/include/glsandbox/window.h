#pragma once

struct GLFWwindow;

namespace glsandbox {

struct WindowConfig {
    int width = 1280;
    int height = 720;
    const char* title = "GLSandbox";
    int glMajor = 3;
    int glMinor = 3;
    bool enableVsync = true;
    bool resizable = true;
};

GLFWwindow* createWindow(const WindowConfig& config = {});
void destroyWindow(GLFWwindow* window);
void pollBasicInput(GLFWwindow* window);

}  // namespace glsandbox
