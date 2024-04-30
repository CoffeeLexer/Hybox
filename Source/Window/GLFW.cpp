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

static uint32_t counter = 0;

static void GlobalInit()
{
    if(counter == 0)
    {
        if(!glfwInit())
        {
            fprintf(stderr, "GLFW INIT FAILED\n");
            exit(1);
        }
    }
    counter++;
}

static void GlobalTerminate()
{
    counter--;
    if(counter == 0)
        glfwTerminate();
}

WindowGLFW::WindowGLFW(const char* name, const uint32_t width, const uint32_t height)
{
    this->width = width;
    this->height = height;

    GlobalInit();
    glfwSetErrorCallback(error_callback);
    
    window = glfwCreateWindow(width, height, name, NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

WindowGLFW::~WindowGLFW()
{
    glfwDestroyWindow(window);
    GlobalTerminate();
}

bool WindowGLFW::IsActive()
{
    return !glfwWindowShouldClose(window);
}

void WindowGLFW::PoolEvents()
{
    glfwPollEvents();
}

void WindowGLFW::SwapBuffers()
{
    glfwSwapBuffers(window);
}
