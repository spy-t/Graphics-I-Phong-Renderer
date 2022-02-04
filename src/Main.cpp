// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "Sphere.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <iostream>
#include <ogl/Camera.h>
#include <ogl/Core.h>
#include <ogl/Model.h>
#include <ogl/Renderer.h>
#include <ogl/Scene.h>
#include <ogl/Text.h>

class MainScene : public Scene {
  GLFWwindow *window;
  const std::filesystem::path &runtime_path;

  bool cursor = false;

  f32 camera_speed = 5.00f;
  f32 camera_sensitivity = 0.05f;
  Camera camera{{glm::vec3{0.0f, 0.0f, 30.0f}}};

  Sphere sphere;
  Shader sphere_shader;
  f32 sphere_speed = 0.25f;  // rads/s
  f32 rotation_angle = 0.0f; // rads

  Model woman_model;
  Shader woman_shader;

  Text text;

  struct MousePos {
    f64 x = 0.0f;
    f64 y = 0.0f;
  };

  MousePos mouse;

public:
  MainScene(GLFWwindow *window, const std::filesystem::path &runtime_path)
      : window{window}, runtime_path{runtime_path},
        sphere{30, 30, runtime_path, glm::vec3{15.0f, 0.0f, 15.0f}, glm::vec3{1.55f}}, text{runtime_path} {
    static glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);

    this->woman_model.load(runtime_path / "models/woman1.obj");

    this->woman_shader = Shader{runtime_path / "shaders/woman.glsl"};
    this->woman_shader.bind();
    this->woman_shader.set_uniformmat4f("u_projection_matrix", projection_matrix);

    this->sphere_shader = Shader{runtime_path / "shaders/sphere.glsl"};
    this->sphere_shader.bind();
    this->sphere_shader.set_uniformmat4f("u_projection_matrix", projection_matrix);

    glfwGetCursorPos(window, &this->mouse.x, &this->mouse.y);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  ~MainScene() {}

  void on_update(f32 delta) override {
    static bool r_pressed = false;
    if (KEY_PRESSED(window, GLFW_KEY_R)) {
      if (!r_pressed) {
        if (this->cursor) {
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        this->cursor = !this->cursor;
        r_pressed = true;
      }
    }
    if (KEY_RELEASED(window, GLFW_KEY_R)) {
      r_pressed = false;
    }

    static bool l_pressed = false;
    if (KEY_PRESSED(window, GLFW_KEY_L)) {
      if (!l_pressed) {
        static glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
        this->woman_shader.reload();
        this->woman_shader.bind();
        this->woman_shader.set_uniformmat4f("u_projection_matrix", projection_matrix);
      }
    }
    if (KEY_RELEASED(window, GLFW_KEY_L)) {
      l_pressed = false;
    }

    f32 x = 0;
    f32 y = 0;
    if (KEY_PRESSED(window, GLFW_KEY_W)) {
      y = this->camera_speed * delta;
    }
    if (KEY_PRESSED(window, GLFW_KEY_S)) {
      y = -(this->camera_speed * delta);
    }
    if (KEY_PRESSED(window, GLFW_KEY_A)) {
      x = -(this->camera_speed * delta);
    }
    if (KEY_PRESSED(window, GLFW_KEY_D)) {
      x = this->camera_speed * delta;
    }
    this->camera.move(x, y, 0.0f);

    f64 last_x = this->mouse.x;
    f64 last_y = this->mouse.y;

    glfwGetCursorPos(window, &this->mouse.x, &this->mouse.y);

    f64 x_offset = -(last_x - this->mouse.x) * this->camera_speed * this->camera_sensitivity;
    f64 y_offset = (last_y - this->mouse.y) * this->camera_speed * this->camera_sensitivity;
    this->camera.rotate(x_offset, y_offset);
    this->camera.update();

    if (KEY_PRESSED(window, GLFW_KEY_H)) {
      this->sphere_speed += 0.05;
    }
    if (KEY_PRESSED(window, GLFW_KEY_J)) {
      this->sphere_speed -= 0.05;
      if (this->sphere_speed < 0) {
        this->sphere_speed = 0;
      }
    }
    this->rotation_angle = this->sphere_speed * delta;
    glm::mat4 sphere_rotation_matrix = glm::rotate(glm::mat4{1.0f}, this->rotation_angle, glm::vec3{0.0f, 1.0f, 0.0f});
    glm::vec4 sphere_new_pos{this->sphere.position, 1.0f};
    this->sphere.position = sphere_rotation_matrix * sphere_new_pos;
  }
  void on_render() override {
    glm::mat4 sphere_scale_matrix = glm::scale(glm::mat4{1.0f}, this->sphere.scale);
    glm::mat4 sphere_translation_matrix = glm::translate(glm::mat4{1.0f}, this->sphere.position);
    glm::mat4 sphere_model_matrix = sphere_translation_matrix * sphere_scale_matrix;
    glm::mat4 sphere_view_space_matrix = this->camera.get_view_matrix() * sphere_model_matrix;
    this->sphere_shader.bind();
    this->sphere_shader.set_uniformmat4f("u_model_view_matrix", sphere_view_space_matrix);
    this->sphere.Draw(this->sphere_shader);

    static glm::mat4 model_scale_matrix = glm::scale(glm::mat4{1.0f}, glm::vec3{0.10f, 0.10f, 0.10f});
    static glm::mat4 model_rotation_matrix =
        glm::rotate(glm::mat4{1.0f}, glm::radians(-90.0f), glm::vec3{0.0f, 1.0f, 0.0f});
    static glm::mat4 model_translation_matrix = glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, 0.0f, 0.0f});

    static glm::mat4 model_matrix = model_translation_matrix * model_rotation_matrix * model_scale_matrix;
    glm::mat4 model_view_matrix = this->camera.get_view_matrix() * model_matrix;

    this->woman_shader.bind();
    this->woman_shader.set_uniformmat4f("u_model_view_matrix", model_view_matrix);

    this->woman_shader.set_uniform3f("u_light.position",
                                     sphere_view_space_matrix * glm::vec4{this->sphere.position, 1.0f});
    this->woman_shader.set_uniform3f("u_light.ambient_intensity", 0.2f, 0.2f, 0.2f);
    this->woman_shader.set_uniform3f("u_light.diffuse_intensity", 0.8f, 0.8f, 0.8f);
    this->woman_shader.set_uniform3f("u_light.specular_intensity", 0.2f, 0.2f, 0.2f);
    this->woman_shader.set_uniform1f("u_light.constant", 1.0f);
    this->woman_shader.set_uniform1f("u_light.linear", 0.007f);
    this->woman_shader.set_uniform1f("u_light.quadratic", 0.0002f);

    this->woman_model.draw(this->woman_shader);

    i32 width = 0;
    i32 height = 0;

    glfwGetWindowSize(window, &width, &height);

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "Camera pos: x " << this->camera.position.x << " y " << this->camera.position.y << " z "
       << this->camera.position.z << "\n";

    ss << "Camera front: x " << this->camera.front.x << " y " << this->camera.front.y << " z " << this->camera.front.z
       << "\n";

    ss << "Camera yaw " << this->camera.yaw << " pitch " << this->camera.pitch << "\n";

    ss << "Sphere pos: x " << this->sphere.position.x << " y " << this->sphere.position.y << " z "
       << this->sphere.position.z << "\n";

    ss << "Sphere speed: " << this->sphere_speed;

    auto str = ss.str();
    auto dimensions = text.text_dimensions(str);
    text.draw(str, dimensions.x / 2.0f, height - (dimensions.y / 2.0f), width, height, dimensions.x, dimensions.y);
  }
};

// From https://stackoverflow.com/a/31526753
GLFWmonitor *guess_current_monitor(GLFWwindow *window) {
  int nmonitors, i;
  int wx, wy, ww, wh;
  int mx, my, mw, mh;
  int overlap, bestoverlap;
  GLFWmonitor *bestmonitor;
  GLFWmonitor **monitors;
  const GLFWvidmode *mode;

  bestoverlap = 0;
  bestmonitor = NULL;

  glfwGetWindowPos(window, &wx, &wy);
  glfwGetWindowSize(window, &ww, &wh);
  monitors = glfwGetMonitors(&nmonitors);

  for (i = 0; i < nmonitors; i++) {
    mode = glfwGetVideoMode(monitors[i]);
    glfwGetMonitorPos(monitors[i], &mx, &my);
    mw = mode->width;
    mh = mode->height;

    overlap = std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
              std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

    if (bestoverlap < overlap) {
      bestoverlap = overlap;
      bestmonitor = monitors[i];
    }
  }

  return bestmonitor;
}

i32 main(i32 argc, char *argv[]) {
  std::string runtime_directory = argc > 1 ? argv[1] : "./";
  if (runtime_directory.back() != '/') {
    runtime_directory.append("/");
  }

  GLFWwindow *window = nullptr;

  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  try {
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(mode->width, mode->height, "Sphere", monitor, NULL);

    // Do a switcheroo to the current monitor
    monitor = guess_current_monitor(window);
    mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    if (!window) {
      throw std::runtime_error("Could not create window");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      throw std::runtime_error("Failed to initialize OpenGL context");
    }

    GL_CALL(const u8 *version = glGetString(GL_VERSION));
    std::cout << version << "\n";

    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glEnable(GL_DEPTH_TEST));

    Renderer renderer{static_cast<u32>(mode->width), static_cast<u32>(mode->height)};

    MainScene main_scene{window, runtime_directory};

    f32 delta = 0.0f;
    f32 last_frame_time = 0.0f;
    while (!glfwWindowShouldClose(window)) {

      f32 current_frame_time = glfwGetTime();
      delta = current_frame_time - last_frame_time;
      last_frame_time = current_frame_time;
      renderer.clear();

      main_scene.on_update(delta);
      main_scene.on_render();

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
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
    return -1;
  }

  glfwTerminate();
  return 0;
}
