#include "verify_deps.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_truetype.h>

#include <nfd.h>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <vector>

namespace {

    int g_failures = 0;

    void check(const char* what, bool ok)
    {
        std::printf("[%s] %s\n", ok ? " OK " : "FAIL", what);
        if (!ok) ++g_failures;
    }

    void verifyContext()
    {
        std::printf("       OpenGL:   %s\n", (const char*)glGetString(GL_VERSION));
        std::printf("       Renderer: %s\n", (const char*)glGetString(GL_RENDERER));
        std::printf("       Vendor:   %s\n", (const char*)glGetString(GL_VENDOR));

        GLint major = 0, minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        check("context is GL 4.6 or newer", major > 4 || (major == 4 && minor >= 6));

        GLint flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        check("GL debug context", (flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0);
    }

    void verifyGlm()
    {
        glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
        glm::vec4 p = m * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        check("GLM translate", p == glm::vec4(1.0f, 2.0f, 3.0f, 1.0f));
    }

    // Encodes a checkerboard to PNG in memory, decodes it, and uploads it as a DSA texture.
    void verifyImagesAndTextures()
    {
        const int W = 64, H = 64;
        std::vector<unsigned char> pixels(W * H * 4);
        for (int y = 0; y < H; ++y)
            for (int x = 0; x < W; ++x) {
                bool light = ((x / 8) + (y / 8)) % 2 == 0;
                unsigned char* px = &pixels[(y * W + x) * 4];
                px[0] = light ? 230 : 40;
                px[1] = light ? 140 : 40;
                px[2] = light ? 40 : 60;
                px[3] = 255;
            }

        // Encode to PNG in memory (nothing is written to disk)
        std::vector<unsigned char> png;
        auto appendToVector = [](void* ctx, void* bytes, int size) {
            auto* out = static_cast<std::vector<unsigned char>*>(ctx);
            auto* b = static_cast<unsigned char*>(bytes);
            out->insert(out->end(), b, b + size);
            };
        int written = stbi_write_png_to_func(appendToVector, &png, W, H, 4, pixels.data(), W * 4);
        check("stb_image_write: encoded PNG in memory", written != 0 && !png.empty());

        // Decode it again and compare
        int w = 0, h = 0, n = 0;
        unsigned char* data = stbi_load_from_memory(png.data(), (int)png.size(), &w, &h, &n, 4);
        bool same = data != nullptr && w == W && h == H &&
            std::memcmp(data, pixels.data(), pixels.size()) == 0;
        check("stb_image: decoded PNG matches original pixels", same);
        if (!data) return;

        // DSA texture creation (GL 4.5)
        GLuint tex = 0;
        glCreateTextures(GL_TEXTURE_2D, 1, &tex);
        glTextureStorage2D(tex, 1, GL_RGBA8, w, h);
        glTextureSubImage2D(tex, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
        check("DSA texture created", tex != 0);

        // Anisotropic filtering (core in GL 4.6)
        GLfloat maxAniso = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
        glTextureParameterf(tex, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);
        std::printf("       max anisotropy: %.0fx\n", maxAniso);
        check("anisotropic filtering available", maxAniso >= 1.0f);

        glDeleteTextures(1, &tex);
        stbi_image_free(data);
    }

    void verifyFont()
    {
        std::ifstream file("C:/Windows/Fonts/arial.ttf", std::ios::binary);
        if (!file) {
            check("stb_truetype: open C:/Windows/Fonts/arial.ttf", false);
            return;
        }
        std::vector<unsigned char> ttf((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        const int AW = 512, AH = 512;
        std::vector<unsigned char> atlas(AW * AH);
        stbtt_bakedchar glyphs[96]; // ASCII 32..127
        int result = stbtt_BakeFontBitmap(ttf.data(), 0, 32.0f, atlas.data(), AW, AH,
            32, 96, glyphs);
        check("stb_truetype: baked Arial into a 512x512 atlas", result > 0);
    }

    void verifyFileDialog()
    {
        bool ok = NFD_Init() == NFD_OKAY;
        check("nativefiledialog init", ok);
        if (ok) NFD_Quit();
    }

    GLuint compileShader(GLenum type, const char* src)
    {
        GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &src, nullptr);
        glCompileShader(s);
        GLint ok = 0;
        glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char log[1024];
            glGetShaderInfoLog(s, sizeof(log), nullptr, log);
            std::printf("       shader error:\n%s\n", log);
        }
        return s;
    }

    void verifyShaders()
    {
        const char* vsSrc = R"(
        #version 460 core
        layout(location = 0) in vec3 aPos;
        uniform mat4 uMVP;
        void main() { gl_Position = uMVP * vec4(aPos, 1.0); }
    )";
        const char* fsSrc = R"(
        #version 460 core
        out vec4 FragColor;
        void main() { FragColor = vec4(1.0); }
    )";

        GLuint vs = compileShader(GL_VERTEX_SHADER, vsSrc);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
        GLuint p = glCreateProgram();
        glAttachShader(p, vs);
        glAttachShader(p, fs);
        glLinkProgram(p);

        GLint ok = 0;
        glGetProgramiv(p, GL_LINK_STATUS, &ok);
        check("GLSL 4.60 shaders compile and link", ok != 0);

        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteProgram(p);
    }

} // namespace

bool VerifyDeps()
{
    g_failures = 0;
    std::printf("=== tiny-cad dependency check ===\n");

    verifyContext();
    verifyGlm();
    verifyImagesAndTextures();
    verifyFont();
    verifyFileDialog();
    verifyShaders();

    if (g_failures == 0) std::printf("All checks passed.\n\n");
    else                 std::printf("%d check(s) FAILED - see above.\n\n", g_failures);

    return g_failures == 0;
}