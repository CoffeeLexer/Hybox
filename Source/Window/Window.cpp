#include "Window.h"

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

Window::Window(const char* name, const uint32_t width, const uint32_t height)
{
    this->width = width;
    this->height = height;

    GlobalInit();
    glfwSetErrorCallback(error_callback);
    
    window = glfwCreateWindow(width, height, name, NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    GlobalTerminate();
}

bool Window::IsVulkanSupported()
{
    return GLFW_TRUE == glfwVulkanSupported();
}

std::vector<const char*> Window::GetVulkanEssentialExtensions()
{
    const char **extensions;
    uint32_t extensionCount;
    extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
    std::vector<const char*> ext;
    ext.resize(extensionCount);
    for(uint32_t i = 0; i < extensionCount; i++)
        ext.push_back(extensions[i]);

    return ext;
}

bool Window::IsActive()
{
    return !glfwWindowShouldClose(window);
}

void Window::PoolEvents()
{
    glfwPollEvents();
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(window);
}
