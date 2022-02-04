#ifndef OGL_VERTEX_BUFFER_LAYOUT_H
#define OGL_VERTEX_BUFFER_LAYOUT_H

#include <ogl/Core.h>
#include <vector>

struct VertexBufferElement {
  u32 type;
  u32 count;
  byte normalized;

  VertexBufferElement(u32 type, u32 count, byte normalized = GL_FALSE)
      : type{type}, count{count}, normalized{normalized} {}
};

class VertexBufferLayout {

  std::vector<VertexBufferElement> elements;
  u32 stride;

public:
  VertexBufferLayout() : stride(0) {}
  ~VertexBufferLayout() {}

  void push(VertexBufferElement e) {
    elements.push_back(e);
    stride += e.count * ogl::get_type_size(e.type);
  }

  inline const std::vector<VertexBufferElement> &get_elements() const { return elements; }
  inline u32 get_stride() const { return stride; }
};

#endif
