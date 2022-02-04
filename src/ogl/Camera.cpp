#include <ogl/Camera.h>

#include <glm/ext/matrix_transform.hpp>

#include <cmath>

Camera::Camera(Camera::CameraOptions opts)
    : position{opts.pos}, front{opts.front.value_or(DEFAULT_FRONT)}, up{opts.up.value_or(DEFAULT_UP)},
      world_up{opts.world_up.value_or(DEFAULT_UP)}, yaw{opts.yaw.value_or(DEFAULT_YAW)},
      pitch{opts.pitch.value_or(DEFAULT_PITCH)}, zoom{opts.zoom.value_or(DEFAULT_ZOOM)}, speed{opts.speed.value_or(
                                                                                             DEFAULT_SPEED)} {
  this->update();
}

Camera::~Camera() {}

void Camera::move(f32 x, f32 y, f32 z) {
  if (x != 0.0f) {
    this->position += this->right * x;
  }

  if (y != 0) {
    this->position += this->up * y;
  }

  if (z != 0.0f) {
    this->position += this->front * z;
  }
}

void Camera::rotate(f32 yaw, f32 pitch) {
  this->yaw += yaw;
  this->pitch += pitch;

  if (this->pitch > 89.0f) {
    this->pitch = 89.0f;
  } else if (this->pitch < -89.0f) {
    this->pitch = -89.0f;
  }

  if (this->yaw >= 360.0f) {
    this->yaw = 0.0f;
  } else if (this->yaw <= -360.0f) {
    this->yaw = 0.0f;
  }
}

glm::mat4 Camera::get_view_matrix() const {
  return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::update() {
  glm::vec3 front = glm::vec3{std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch)),
                              std::sin(glm::radians(this->pitch)),
                              std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch))};
  this->front = glm::normalize(front);
  this->right = glm::normalize(glm::cross(this->front, this->world_up));
  this->up = glm::normalize(glm::cross(this->right, this->front));
}
