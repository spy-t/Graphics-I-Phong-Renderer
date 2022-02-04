#ifndef OGL_MODEL_H
#define OGL_MODEL_H

#include <ogl/Core.h>
#include <ogl/Mesh.h>
#include <ogl/Shader.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

class Model {
  std::vector<Mesh> meshes;

  std::filesystem::path path;

public:
  explicit Model();
  explicit Model(const std::filesystem::path &path);

  Model(const Model &) = delete;
  Model &operator=(const Model &) = delete;

  Model(Model &&) = default;

  void load(const std::filesystem::path &path);
  void draw(Shader &shader);

private:
  void load();
  void load_node(aiNode *node, const aiScene *scene);
  Mesh load_mesh(aiMesh *mesh, const aiScene *scene);
};

#endif
