#ifndef OGL_CORE_H
#define OGL_CORE_H

#include <glad/glad.h>

#include <csignal>
#include <cstdint>
#include <iostream>

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

#define OGL_INTERNAL static

#ifdef DEBUG

#define ASSERT(x)                                                                                                     \
  if (!(x))                                                                                                           \
  raise(SIGTRAP)

#else

#define ASSERT(x) x

#endif

#define KEY_PRESSED(window, key) (GLFW_PRESS == glfwGetKey((window), (key)))
#define KEY_RELEASED(window, key) (GLFW_RELEASE == glfwGetKey((window), (key)))

namespace ogl {
void gl_clear_error();
bool gl_log_error(const char *func, const char *file, int line);
u32 get_type_size(u32 type);
} // namespace ogl

#define GL_CALL(f)                                                                                                    \
  ogl::gl_clear_error();                                                                                              \
  f;                                                                                                                  \
  ASSERT(ogl::gl_log_error(#f, __FILE__, __LINE__))

#endif
