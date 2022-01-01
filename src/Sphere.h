#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include <vector>

// #include "Shader.h"

class Sphere {

  struct Vertex {

    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
  };

public:
  unsigned int VAO = 0;
  unsigned int VBO;
  unsigned int EBO;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> Indices;

public:
  Sphere(unsigned int xSegments, unsigned int ySegments);

  // You must implement these functions!
  void Draw();
  void setupSphere();
};

#endif
