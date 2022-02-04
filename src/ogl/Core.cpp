#include <ogl/Core.h>

namespace ogl {
void gl_clear_error() {
  while (GLenum error = glGetError()) {
  }
}

bool gl_log_error(const char *func, const char *file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error] (" << error << "): " << file << ":" << func << ":" << line << "\n";
    return false;
  }

  return true;
}

u32 get_type_size(u32 type) {
  switch (type) {
  case GL_FLOAT:
    return sizeof(GLfloat);
  case GL_UNSIGNED_INT:
    return sizeof(GLuint);
  case GL_UNSIGNED_BYTE:
    return sizeof(GLbyte);
  default:
    ASSERT(false);
    return 0;
  }
}
} // namespace ogl
