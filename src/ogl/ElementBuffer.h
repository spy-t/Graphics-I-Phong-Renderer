#ifndef OGL_ELEMENT_BUFFER_H
#define OGL_ELEMENT_BUFFER_H

#include <ogl/Core.h>

class ElementBuffer {
  u32 rendererID = 0;
  u32 count = 0;

public:
  ElementBuffer();
  ElementBuffer(ElementBuffer &) = delete;
  ElementBuffer(ElementBuffer &&);
  ElementBuffer &operator=(ElementBuffer &&);
  ~ElementBuffer();

  void set_data(const u32 *data, u32 count, GLenum usage = GL_STATIC_DRAW);
  void bind() const;
  void unbind() const;

  inline u32 get_count() const { return count; }
};

#endif
