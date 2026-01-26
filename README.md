# scop – OpenGL OBJ Viewer

A 3D model viewer built with OpenGL and c++20. Load and display `.obj` files and support basic interactions.

## 📦 Dependencies

- [GLFW](https://www.glfw.org/) - window and input handling
- [GLAD](https://glad.dav1d.de/) - OpenGL function loader

## 🛠️ Installation & Usage

```bash
git clone https://github.com/Smafl/scop.git
cd scop
make
./scop models/cube.obj textureSources/wood.bmp
```

### 📚 Info sources (might be not available):
- [OpenGL Specification](https://registry.khronos.org/OpenGL/specs/gl/glspec46.core.pdf)
- [GLFW documentation](https://www.glfw.org/docs/3.3/index.html)
- [LearnOpenGL](https://learnopengl.com/Getting-started/OpenGL)
- [Learning Modern 3D Graphics Programming](https://nicolbolas.github.io/oldtut/index.html)
- [Example Code](https://wikis.khronos.org/opengl/Example_Code#GLSL_expression_examples)
- [C++ reading and writing BMP images](https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/)
- [BMP file format](https://en.wikipedia.org/wiki/BMP_file_format)
- [Tutorial 3: Matrices](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/)


### Interaction

Movement:
| Action | macOS | Windows/Linux |
|--------|-------|---------------|
| Move Left | `Cmd + ←` | `Ctrl + ←` |
| Move Right | `Cmd + →` | `Ctrl + →` |
| Move Up | `Cmd + ↑` | `Ctrl + ↑` |
| Move Down | `Cmd + ↓` | `Ctrl + ↓` |
| Move Forward | `Cmd + W` | `Ctrl + W` |
| Move Backward | `Cmd + S` | `Ctrl + S` |

Rendering Options:
| Action | macOS | Windows/Linux |
|--------|-------|---------------|
| Toggle Auto-rotation | `Cmd + R` | `Ctrl + R` |
| Toggle Texture/Color | `Cmd + T` | `Ctrl + T` |
| Cycle Color Modes | `Cmd + F` | `Ctrl + F` |