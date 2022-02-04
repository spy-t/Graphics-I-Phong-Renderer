#include <ogl/Core.h>
#include <ogl/VertexArray.h>

VertexArray::VertexArray() { GL_CALL(glGenVertexArrays(1, &this->rendererID)); }
VertexArray::~VertexArray() {
  if (this->rendererID != 0) {
    GL_CALL(glDeleteVertexArrays(1, &this->rendererID));
  }
}

VertexArray::VertexArray(VertexArray &&other) { *this = std::move(other); }
VertexArray &VertexArray::operator=(VertexArray &&other) {
  if (this != &other) {
    this->rendererID = other.rendererID;
    other.rendererID = 0;
  }
  return *this;
}

void VertexArray::set_layout(const VertexBufferLayout &layout) {
  const auto &elements = layout.get_elements();
  u32 offset = 0;
  for (u32 i{0}; i < elements.size(); ++i) {
    const auto &element = elements[i];
    GL_CALL(glEnableVertexAttribArray(i));
    GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get_stride(),
                                  (const void *)offset));
    offset += element.count * ogl::get_type_size(element.type);
  }
}

void VertexArray::bind() const { GL_CALL(glBindVertexArray(this->rendererID)); }
void VertexArray::unbind() const { GL_CALL(glBindVertexArray(0)); }
