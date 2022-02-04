#include "Sphere.h"

const float PI = 3.14159265359f;
const float TAU = 6.28318530717f;

Sphere::Sphere(u32 xSegments, u32 ySegments, const std::filesystem::path &runtime_path, const glm::vec3 &pos,
               const glm::vec3 scale)
    : runtime_path{runtime_path}, position{pos}, scale{scale} {

  for (u32 y = 0; y <= ySegments; ++y) {
    for (u32 x = 0; x <= xSegments; ++x) {

      Vertex vertex;
      glm::vec3 vector;

      f32 xSegment = (float)x / (float)xSegments;
      f32 ySegment = (float)y / (float)ySegments;
      f32 xPos = std::cos(xSegment * TAU) * std::sin(ySegment * PI); // TAU is 2PI
      f32 yPos = std::cos(ySegment * PI);
      f32 zPos = std::sin(xSegment * TAU) * std::sin(ySegment * PI);

      vector.x = xPos;
      vector.y = yPos;
      vector.z = zPos;

      vertex.Position = vector;
      vertex.TexCoords = glm::vec2(xSegment, ySegment);
      vertex.Normal = vector;

      vertices.push_back(vertex);
    }
  }

  for (u32 y = 0; y < ySegments; ++y) {
    for (u32 x = 0; x < xSegments; ++x) {
      Indices.push_back((y + 1) * (xSegments + 1) + x);
      Indices.push_back(y * (xSegments + 1) + x);
      Indices.push_back(y * (xSegments + 1) + x + 1);

      Indices.push_back((y + 1) * (xSegments + 1) + x);
      Indices.push_back(y * (xSegments + 1) + x + 1);
      Indices.push_back((y + 1) * (xSegments + 1) + x + 1);
    }
  }

  // You must implement this function!
  setupSphere();
}

void Sphere::setupSphere() {
  this->mesh =
      Mesh{this->vertices, this->Indices, std::vector<Texture>{}, std::vector<Material>{}, std::string("sphere")};
}

void Sphere::Draw(Shader &shader) {
  GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
  this->mesh.draw(shader);
  GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}
