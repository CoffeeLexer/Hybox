#include "Window.h"
#include "GLFW.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>
#include <cstdint>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

Window::Window(const char* name, const uint32_t width, const uint32_t height)
{
    this->width = width;
    this->height = height;

    if (!glfwInit())
    {
        printf("GLFW INIT FAIL");
        exit(1);
    }

    glfwSetErrorCallback(error_callback);
    
    SetInternal(new GLFW3Specific);
    GLFW3Specific* specific = GetInternal<GLFW3Specific*>();

    specific->window = glfwCreateWindow(width, height, name, NULL, NULL);

    glfwMakeContextCurrent(specific->window);
    glfwSwapInterval(1);
}

Window::~Window()
{
    GLFW3Specific* internal = GetInternal<GLFW3Specific*>();
    glfwDestroyWindow(internal->window);
    glfwTerminate();
    delete internal;
}

bool Window::IsActive()
{
    return !glfwWindowShouldClose(GetInternal<GLFW3Specific*>()->window);
}

void Window::PoolEvents()
{
    glfwPollEvents();
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(GetInternal<GLFW3Specific*>()->window);
}
