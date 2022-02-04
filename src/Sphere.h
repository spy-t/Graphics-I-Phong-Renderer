#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include <vector>

#include <ogl/ElementBuffer.h>
#include <ogl/Mesh.h>
#include <ogl/VertexArray.h>
#include <ogl/VertexBuffer.h>

// #include "Shader.h"

class Sphere {

public:
  const std::filesystem::path &runtime_path;
  VertexArray vao;
  VertexBuffer vbo;
  ElementBuffer ebo;

  Mesh mesh;

  glm::vec3 position;
  glm::vec3 scale;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> Indices;

public:
  Sphere(u32 xSegments, u32 ySegments, const std::filesystem::path &runtime_path,
         const glm::vec3 &pos = glm::vec3{0.0f}, const glm::vec3 scale = glm::vec3{1.0f});

  // You must implement these functions!
  void Draw(Shader &);
  void setupSphere();
};

#endif
