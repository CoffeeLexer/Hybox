#include <cstdio>
#include <cstdlib>
#include <vector>
#include "Window/GLFW.h"
#include "Engine/Instance.h"
#include "Engine/Device.h"
#include "Engine/Carcass.h"

int main(int argc, char const *argv[])
{
    printf("Startup!\n");
    Carcass carcass = {};
    CreateInstance(carcass);
    CreateDevice(carcass);


    WindowGLFW w = WindowGLFW("App", 500, 500);

    while(w.IsActive())
    {
        w.PoolEvents();
        w.SwapBuffers();
    }

    printf("Clean Exit!\n");
    return 0;
}
