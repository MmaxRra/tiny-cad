
#include "gl_debug.h"
#include <glad/glad.h>
#include <cstdio>

static void APIENTRY glDebugCallback(GLenum, GLenum type, GLuint id, GLenum severity,
    GLsizei, const GLchar* message, const void*)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return; // too chatty
    std::printf("[GL %s] (%u) %s\n",
        type == GL_DEBUG_TYPE_ERROR ? "ERROR" : "msg", id, message);
}

void enableGLDebugOutput()
{
    GLint flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT)) return; // not a debug context

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugCallback, nullptr);
}