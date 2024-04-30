#pragma once

#include <cstdint>

class Window
{
protected:
    uint32_t width;
    uint32_t height;

    Window() {};
    Window(const char* name, const uint32_t width, const uint32_t height) {};
    ~Window() {};
public:
    virtual bool IsActive() = 0;
    virtual void PoolEvents() = 0;
    virtual void SwapBuffers() = 0;
};
