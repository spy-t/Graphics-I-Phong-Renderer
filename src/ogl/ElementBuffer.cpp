#include <ogl/Core.h>
#include <ogl/ElementBuffer.h>

ElementBuffer::ElementBuffer() { GL_CALL(glGenBuffers(1, &this->rendererID)); }

ElementBuffer::ElementBuffer(ElementBuffer &&other) { *this = std::move(other); }
ElementBuffer &ElementBuffer::operator=(ElementBuffer &&other) {
  if (this != &other) {
    this->rendererID = other.rendererID;
    this->count = other.count;
    other.rendererID = 0;
  }

  return *this;
}

void ElementBuffer::set_data(const u32 *data, u32 count, GLenum usage) {
  GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), data, usage));
  this->count = count;
}

ElementBuffer::~ElementBuffer() {
  if (this->rendererID != 0) {
    GL_CALL(glDeleteBuffers(1, &this->rendererID));
  }
}

void ElementBuffer::bind() const { GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->rendererID)); }
void ElementBuffer::unbind() const { GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }
