#ifndef GR02__CORE_H
#define GR02__CORE_H

#include <GLFW/glfw3.h>
#include <csignal>
#include <cstdint>
#include <iostream>

#ifdef DEBUG
#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
  raise(SIGTRAP)
#else
#define ASSERT(x) x
#endif

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef uint8_t byte;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef float f32;
typedef double f64;

namespace prgl {
inline void glClearError() {
  while (GLenum error = glGetError()) {
    (void)error;
  }
}

inline bool glLogError(const char *func, const char *file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error] (" << error << "): " << file << ":" << func
              << ":" << line << "\n";
    return false;
  }

  return true;
}
} // namespace prgl

#define GL_CALL(f)                                                             \
  prgl::glClearError();                                                        \
  f;                                                                           \
  ASSERT(prgl::glLogError(#f, __FILE__, __LINE__))

#endif
