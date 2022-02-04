#ifndef OGL_CAMERA_H
#define OGL_CAMERA_H

#include <ogl/Core.h>

#include <glm/glm.hpp>

#include <optional>

class Camera {

public:
  glm::vec3 position;
  // -Z axis
  glm::vec3 front;
  // Y axis
  glm::vec3 up;
  // X axis
  glm::vec3 right;
  glm::vec3 world_up;

  f32 yaw;
  f32 pitch;
  f32 zoom;

public:
  f32 speed;

  static constexpr auto DEFAULT_FRONT = glm::vec3{0.0f, 0.0f, -1.0f};
  static constexpr auto DEFAULT_UP = glm::vec3{0.0f, 1.0f, 0.0f};
  static constexpr f32 DEFAULT_YAW = -90.0f;
  static constexpr f32 DEFAULT_PITCH = 0.0f;
  static constexpr f32 DEFAULT_ZOOM = 45.0f;
  static constexpr f32 DEFAULT_SPEED = 0.25f;

  struct CameraOptions {
    glm::vec3 pos;
    std::optional<glm::vec3> front = std::nullopt;
    std::optional<glm::vec3> up = std::nullopt;
    std::optional<glm::vec3> world_up = std::nullopt;
    std::optional<f32> yaw = std::nullopt;
    std::optional<f32> pitch = std::nullopt;
    std::optional<f32> zoom = std::nullopt;
    std::optional<f32> speed = std::nullopt;
  };

  Camera(CameraOptions opts);
  ~Camera();

  glm::mat4 get_view_matrix() const;
  void move(f32 x, f32 y, f32 z);
  void rotate(f32 yaw, f32 pitch);
  void update();
};

#endif
