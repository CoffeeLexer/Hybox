#include <cstdio>
#include <cstdlib>
#include <vector>
#include "Window.h"


int main(int argc, char const *argv[])
{
    printf("Startup!\n");

    Window w = Window("App", 500, 500);

    while(w.IsActive())
    {
        w.PoolEvents();
        w.SwapBuffers();
    }

    printf("Clean Exit!\n");
    return 0;
}
