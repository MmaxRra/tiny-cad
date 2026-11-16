
## Todo(s)
- fix grid/camera conflict at 45*yaw..

### Now!
- [x] Implement shader loader
- [x] Frame timing (delta time, fps)
- [x] Window resize handling
- [x] Input system (mouse, keyboard)
- [x] Implement a camera system (rotation, zoom, pan)
- [x] Render a starting grid/planes
- [ ] 3D render layer for parts
- [ ] 2D batch renderer layer for UI

### Structures
- [ ] Mesh structure (vertices, indices, gpu buffers, boundingbox)
- [ ] Transform structure (position, rotation, scale, built matrix)
- [ ] SketchPlane structure (origin, x axis, y axis, normal, 2D -> world conversion)
- [ ] Sketch Entities (point, line, arc, circle - lines/arcs reference point ids)
- [ ] Sketch structure (id, name, plane, list of entities, found profiles)
- [ ] Feature structure (type: extrude/cut, sketch reference + profile, distance, operation)
- [ ] Part structure (name, transform, timeline of sketches + features, result mesh, dirty flag <rebuild mesh>)

### UI
- [ ] Display timeline of sketches and features
- [ ] Implement a way to select a sketch and edit it (add/remove entities, move points, etc..)
- [ ] Make block interactive (move tool, selectable)
- [ ] Build UI system and elements (buttons, sliders, text, textfields, numberfields, etc..)
- [ ] Implement text rendering (bitmap font, text layout, etc..)
- [ ] Tool/mode system (select, sketch, move, etc..)
- [ ] Properties panel (editable values for whatever is selected, sketch/feature/part)
- [ ] Theme struct (colors, sizes, DPI scaling, etc..)
- [ ] Keyboard shortcuts (for tools, undo/redo, etc..)
- [ ] File dialogs (with nfd for save, load, import, export)

### Rendering
- [ ] Implement sketch rendering overlay upon sketch
- [ ] Edge/outline rendering for parts
- [ ] Selection and hover highlightning for parts, sketches, and sketch entities
- [ ] Implement Anti-aliasing MSAA (2x, 4x, 8x) for UI and part lines/edges
- [ ] Select plane to make sketch upon, move camera
- [ ] Orientation gizmo for camera and plane selection, clickable and draggable to rotate/move camera 

### Engine
- [ ] Implement a way to build a mesh into finnished part (using timeline of sketches + features)
- [ ] Rebuild mesh if part is dirty (sketch/feature changed)
- [ ] Implement a way to save/load parts (sketches + features) to/from disk (own format)
- [ ] Implement a way to export mesh to STL format and import STL files (for reference) along with obj
- [ ] Picking system (raycast towards scene, find clicked objects, return closest one)
- [ ] Undo/redo (timeline of sketches/features, along with user actions)
- [ ] Profile detection (find closed loops of sketch entities, find profiles in sketches)
- [ ] Extrude to mesh (triangulate caps? plus side faces and normals)
- [ ] Units (mm) standard metric system with conversion between unit sizes
- [ ] Snapping (to grid, to points, to lines, to planes, to angles, etc..)
- [ ] Failed features (detect when a feature fails, mark as failed, throw error)
- [ ] Sketch constraints + dimensions (solver)
- [ ] B-rep topology (half-edge: vertex, edge, loop, face, shell)
- [ ] B-rep geometry (plane first, then cylinder)
- [ ] Extrued -> B-rep
- [ ] Tessellate B-rep -> mesh
- [ ] Booleans on B-rep

### Features to mesh builder
- [ ] Extrude (new body, plus join/cut)
- [ ] Cut (remove material from existing body)
- [ ] Revolve (spin profile around axis)
- [ ] Mirror (mirror existing body across plane)
- [ ] Pattern (linear, circular, along path)
- [ ] Hole (drill hole, with options for countersink, counterbore, etc..)
- [ ] Sweep/loft (sweep profile along path, or loft between profiles)
- [ ] Shell (hollow out body, with wall thickness)
- [ ] Fillet/chamfer (round or bevel edges of body) <switch to b-rep before this>

### Later
- [ ] Add a way to select GPU to use (later)

```
	 **evaluate**
		  |			 
timeline -|-> B-rep -|-> render mesh --> render to gpu
					 |
			   **tessellate**
```

timeline : (sketches + features)
B-rep	 : (exact solid: faces, edges, vertices, true surfaces)
mesh	 : (triangles, normals, uvs, gpu buffers)