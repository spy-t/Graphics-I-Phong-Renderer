#ifndef OGL_VERTEX_BUFFER_H
#define OGL_VERTEX_BUFFER_H

#include <ogl/Core.h>

class VertexBuffer {
  u32 rendererID = 0;

public:
  VertexBuffer();
  VertexBuffer(VertexBuffer &) = delete;

  VertexBuffer(VertexBuffer &&);
  VertexBuffer &operator=(VertexBuffer &&);
  ~VertexBuffer();

  void set_data(const void *data, u32 size, GLenum usage = GL_STATIC_DRAW);
  void bind() const;
  void unbind() const;
};

#endif
