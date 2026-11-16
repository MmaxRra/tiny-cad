# tiny-cad

A small parametric CAD program written from scratch in C++ and OpenGL.
Parts are built from a timeline of sketches and features (extrude, cut, revolve and so on), evaluated into a B-rep solid and tessellated for rendering, in the style of tools like Fusion 360.
**Status:** early development. The application framework, camera and rendering groundwork are in place; modeling features are not implemented yet.

## Building

Requirements:

- Windows
- Visual Studio 2026 (C++ desktop development workload)
- A GPU with OpenGL 4.6 support

Steps:

1. Clone the repository.
2. Open `tiny-cad.slnx` in Visual Studio.
3. Select the `x64` platform and build.

All dependencies are included in `deps/`, so no extra setup is needed.

## Controls

| Input | Action |
|---|---|
| Middle mouse drag | Orbit |
| Shift + middle mouse drag | Pan |
| Scroll wheel | Zoom |
| Home | Reset view |
| F | Frame view |
| O | Toggle orthographic / perspective |
| F2 | Toggle vsync |

## Architecture

```
timeline (sketches + features)
        |  evaluate
        v
B-rep (exact solid: faces, edges, vertices, surfaces)
        |  tessellate
        v
render mesh (triangles) --> GPU
```

## Project structure

```
deps/                 third-party libraries
tiny-cad/
  main.cpp
  app/                application, main loop
  core/               platform layer: window, input, timing
  render/             OpenGL: shaders, camera, renderers
  cad/                CAD model: sketches, features, B-rep
  ui/                 custom immediate-mode UI
  debug/              dependency checks, GL debug output
  assets/             shaders, fonts, icons
```

## Dependencies

| Library | Purpose | License |
|---|---|---|
| [GLFW](https://www.glfw.org) | Window, context and input | zlib |
| [glad](https://github.com/Dav1dde/glad) | OpenGL function loader | MIT / Apache 2.0 |
| [GLM](https://github.com/g-truc/glm) | Math | MIT |
| [stb](https://github.com/nothings/stb) | Image loading/writing, font rasterization | MIT / public domain |
| [nativefiledialog-extended](https://github.com/btzy/nativefiledialog-extended) | Native file dialogs | zlib |

## Roadmap

- Camera, grid and basic part rendering
- 2D batch renderer, text rendering and custom UI
- Sketches, profile detection and extrude
- B-rep topology and geometry, tessellation
- Booleans (join, cut, intersect)
- Undo/redo, save/load, STL/OBJ export
- Additional features: revolve, mirror, pattern, hole, sweep/loft, shell, fillet/chamfer

## License

tiny-cad is released under the MIT License. See `LICENSE` for details.
Third-party libraries in `deps/` are covered by their own licenses, included alongside each library.