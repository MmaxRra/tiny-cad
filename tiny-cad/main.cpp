#include <glad/glad.h> // must come before GLFW
#include <GLFW/glfw3.h>

#include "debug/verify_deps.h"
#include <cstdio>

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

int main()
{
    if (!glfwInit()) {
        std::printf("glfwInit failed\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "tiny-cad", nullptr, nullptr);
    if (!window) {
        std::printf("Failed to create window with a GL 4.6 core context\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::printf("Failed to load OpenGL functions (glad)\n");
        return 1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugCallback, nullptr);

#ifdef _DEBUG
    VerifyDeps();
#endif

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int w = 0, h = 0;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);

        glClearColor(0.15f, 0.16f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}