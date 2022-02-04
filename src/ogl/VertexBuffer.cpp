#include <ogl/Core.h>
#include <ogl/VertexBuffer.h>

VertexBuffer::VertexBuffer() {
  GL_CALL(glGenBuffers(1, &this->rendererID));
  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->rendererID));
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) { *this = std::move(other); }
VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) {
  if (this != &other) {
    this->rendererID = other.rendererID;
    other.rendererID = 0;
  }

  return *this;
}

VertexBuffer::~VertexBuffer() {
  if (this->rendererID != 0) {
    GL_CALL(glDeleteBuffers(1, &this->rendererID));
  }
}

void VertexBuffer::set_data(const void *data, u32 size, GLenum usage) {
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}

void VertexBuffer::bind() const { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->rendererID)); }
void VertexBuffer::unbind() const { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
