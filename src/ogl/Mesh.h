#ifndef OGL_MESH_H
#define OGL_MESH_H

#include <ogl/Core.h>
#include <ogl/ElementBuffer.h>
#include <ogl/Material.h>
#include <ogl/Shader.h>
#include <ogl/Texture.h>
#include <ogl/Vertex.h>
#include <ogl/VertexArray.h>
#include <ogl/VertexBuffer.h>

#include <vector>

class Mesh {

public:
  std::vector<Vertex> vertices;
  std::vector<u32> indices;
  std::vector<Texture> textures;
  std::vector<Material> materials;

  VertexArray vao;
  VertexBuffer vbo;
  ElementBuffer ebo;

  std::string mesh_id;

  Mesh(std::vector<Vertex> vertices, std::vector<u32> indices, std::vector<Texture> &&textures,
       std::vector<Material> materials, std::string mesh_id);
  Mesh();

  Mesh(const Mesh &) = delete;

  Mesh(Mesh &&);
  Mesh &operator=(Mesh &&);

  void draw(Shader &);

private:
  void setup();
};

#endif
