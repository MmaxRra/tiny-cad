
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <cstdio>

#include "debug/verify_deps.h"
#include "engine/shader.h"
#include "engine/frameTimer.h"

// ask to use dedicated GPU
extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

static void APIENTRY glDebugCallback(GLenum, GLenum type, GLuint id, GLenum severity,
    GLsizei, const GLchar* message, const void*)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return; // too chatty
    std::printf("[GL %s] (%u) %s\n",
        type == GL_DEBUG_TYPE_ERROR ? "ERROR" : "msg", id, message);
}

struct FramebufferSize { int w = 0, h = 0; };
FramebufferSize handleResize(GLFWwindow* window) {
    
    FramebufferSize s;
    glfwGetFramebufferSize(window, &s.w, &s.h);
    glViewport(0, 0, s.w, s.h);
    return s;
}

int main()
{
    // init glfw
    if (!glfwInit()) {
        std::printf("glfwInit failed\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    // init window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "tiny-cad", nullptr, nullptr);

    if (!window) {
        std::printf("Failed to create window with a GL 4.6 core context\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync on --> 1 || 0

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::printf("Failed to load OpenGL functions (glad)\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugCallback, nullptr);

    // run verify
#ifdef _DEBUG
    VerifyDeps();
#endif



    // main ..
    {
        FrameTimer timer;
        while (!glfwWindowShouldClose(window)) {

            timer.frameTick();
            glfwPollEvents(); // render

            if (timer.secondTicked())  { } // reload shader if changed
            auto size = handleResize(window);

            // is minimized? hold..
            if (size.w == 0 || size.h == 0) { glfwWaitEvents(); continue; }
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(window); // end of render
        }
    }  
    

    // exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}