#pragma once

#include <cstdint>

class Window
{
private:
    uint32_t width;
    uint32_t height;

    void* internal;
    template<typename T>
    T GetInternal() 
    {
        return reinterpret_cast<T>(internal);
    }
    template<typename T>
    void SetInternal(T* ptr)
    {
        internal = ptr;
    }
public:
    Window(const char* name, const uint32_t width, const uint32_t height);
    ~Window();
    bool IsActive();
    void PoolEvents();
    void SwapBuffers();
};
