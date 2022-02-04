#ifndef OGL_RENDERER_H
#define OGL_RENDERER_H

#include <ogl/ElementBuffer.h>
#include <ogl/Shader.h>
#include <ogl/VertexArray.h>

class Renderer {

public:
  u32 render_width;
  u32 render_height;

  Renderer(u32 width, u32 height);

  void clear() const;
  void draw(const VertexArray &va, const ElementBuffer &eb, const Shader &shader) const;
};

#endif
