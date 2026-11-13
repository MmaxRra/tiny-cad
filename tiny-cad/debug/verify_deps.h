#pragma once

// Runs a self-test of every third-party dependency and prints the results
// to the console: OpenGL context, GLM, stb_image(_write), stb_truetype,
// nativefiledialog-extended, DSA, anisotropic filtering and shader compilation.

// Requires a current OpenGL context (call it after gladLoadGLLoader succeeds).
// Returns true if every check passed.

bool VerifyDeps();