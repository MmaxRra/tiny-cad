
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <cstdio>

#include "debug/verify_deps.h"
#include "core/platform.h"
#include "core/window.h"
#include "app/app.h"

extern "C" {   // ask to use dedicated GPU
    __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main()
{
    // init + root window
    Platform platform; if (!platform.isValid()) return 1;
    Window window; if (!window.isValid()) return 1;

    #ifdef _DEBUG
        VerifyDeps();
    #endif

    App app(window);
    app.run();

    return 0;
}   // destoryed in reverse order: app -> window -> platform