#ifndef OGL_MATERIAL_H
#define OGL_MATERIAL_H

#include <glm/glm.hpp>
#include <ogl/Core.h>

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  f32 shininess;
};

#endif
