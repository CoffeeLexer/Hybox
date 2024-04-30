#pragma once
#include "Window.h"

class GLFWwindow;

class WindowGLFW : public Window
{
    GLFWwindow *window;
public:
    WindowGLFW(const char* name, const uint32_t width, const uint32_t height);
    ~WindowGLFW();
    bool IsActive() override;
    void PoolEvents() override;
    void SwapBuffers() override;
};
