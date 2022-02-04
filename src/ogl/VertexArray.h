#ifndef OGL_VERTEX_ARRAY_H
#define OGL_VERTEX_ARRAY_H

#include <ogl/VertexBuffer.h>
#include <ogl/VertexBufferLayout.h>

class VertexArray {

  u32 rendererID = 0;

public:
  explicit VertexArray();
  VertexArray(const VertexArray &other) = delete;

  explicit VertexArray(VertexArray &&other);
  VertexArray &operator=(VertexArray &&other);
  ~VertexArray();

  void set_layout(const VertexBufferLayout &layout);

  void bind() const;
  void unbind() const;
};

#endif
