#pragma once

#include <cstdint>
#include <vector>

struct GLFWwindow;
struct Carcass;

enum class NativeWindowType : uint32_t
{
    Win32,
    Cocoa,
    X11,
    Wayland,
};

class Window
{
protected:
    uint32_t width;
    uint32_t height;
    GLFWwindow *window;

public:
    Window(const char* name, const uint32_t width, const uint32_t height);
    ~Window();

    static bool IsVulkanSupported();
    static std::vector<const char*> GetVulkanEssentialExtensions();

    bool IsActive();
    void PoolEvents();
    void SwapBuffers();

    template<NativeWindowType T>
    void VkCreateSurface(Carcass *carcass);
};
