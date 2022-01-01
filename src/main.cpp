// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "Sphere.h"
#include "core.h"
#include <iostream>

i32 main(void) {
  GLFWwindow *window;

  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  try {
    window = glfwCreateWindow(640, 480, "Phong Lit Head", NULL, NULL);
    if (!window) {
      throw std::runtime_error("Failed to initialize window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      throw std::runtime_error("Failed to initialize OpenGL context");
    }

#ifdef DEBUG
    std::cout << "Running in debug mode\n";
#endif

    Sphere s{10, 15};
    while (!glfwWindowShouldClose(window)) {
      GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

      s.Draw();

      glfwSwapBuffers(window);

      glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
  } catch (...) {
    std::exception_ptr e = std::current_exception();
    if (e) {
      try {
        std::rethrow_exception(e);
      } catch (const std::exception &ex) {
        std::cout << "EXCEPTION: " << ex.what() << "\n";
      }
    }
    glfwTerminate();
    return EXIT_FAILURE;
  }
}
