#include <ogl/Mesh.h>

Mesh::Mesh() {}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<u32> indices, std::vector<Texture> &&textures,
           std::vector<Material> materials, std::string mesh_id)
    : vertices{vertices}, indices{indices}, textures{std::move(textures)}, materials{materials}, mesh_id{mesh_id} {
  this->setup();
}

Mesh::Mesh(Mesh &&other) { *this = std::move(other); }

Mesh &Mesh::operator=(Mesh &&other) {
  if (this != &other) {
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    textures = std::move(other.textures);
    materials = std::move(other.materials);
    vao = std::move(other.vao);
    vbo = std::move(other.vbo);
    ebo = std::move(other.ebo);
    mesh_id = std::move(other.mesh_id);
  }

  return *this;
}

void Mesh::setup() {
  if (this->vertices.size() == 0 && this->indices.size() == 0) {
    return;
  }
  this->vao.bind();
  this->vbo.bind();
  this->ebo.bind();

  this->vbo.set_data(this->vertices.data(), this->vertices.size() * sizeof(Vertex));
  this->ebo.set_data(this->indices.data(), this->indices.size());

  VertexBufferLayout layout;
  layout.push({GL_FLOAT, 3});
  layout.push({GL_FLOAT, 3});
  layout.push({GL_FLOAT, 2});

  this->vao.set_layout(layout);
}

void Mesh::draw(Shader &shader) {
  shader.bind();
  if (shader.uniform_exists("u_active_textures") && shader.uniform_exists("u_texture")) {
    if (this->textures.size() > 0) {
      Texture &tex = this->textures[0];
      tex.bind();
      tex.load();
      shader.set_uniform1i("u_texture", 0);
      shader.set_uniform1i("u_active_textures", 1);
    } else {
      shader.set_uniform1i("u_active_textures", 0);
    }
  }

  if (shader.uniform_exists("u_material.ambient")) {
    if (this->materials.size() > 0) {
      Material &mat = this->materials[0];
      shader.set_uniform3f("u_material.ambient", mat.ambient);
      shader.set_uniform3f("u_material.diffuse", mat.diffuse);
      shader.set_uniform3f("u_material.specular", mat.specular);
      shader.set_uniform1f("u_material.shininess", mat.shininess);
    }
  }
  this->vao.bind();
  GL_CALL(glDrawElements(GL_TRIANGLES, this->ebo.get_count(), GL_UNSIGNED_INT, 0));
}
