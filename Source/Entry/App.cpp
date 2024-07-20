#include <cstdio>
#include <cstdlib>
#include <vector>
#include "Window/Window.h"
#include "Engine/Instance.h"
#include "Engine/Device.h"
#include "Engine/Carcass.h"

int main(int argc, char const *argv[])
{
    printf("Startup!\n");
    Carcass carcass = {};

    CreateInstance(&carcass);
    CreateDevice(&carcass);

    Window window = Window("App", 500, 500);

    window.VkCreateSurface<NativeWindowType::Win32>(&carcass);

    while(window.IsActive())
    {
        window.PoolEvents();
        window.SwapBuffers();
    }

    printf("Clean Exit!\n");
    return 0;
}
