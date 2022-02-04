#include <ogl/Model.h>

Model::Model() : path{} {}
Model::Model(const std::filesystem::path &path) : path{path} {}

void Model::load() {
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(this->path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
  /* const aiScene *scene = importer.ReadFile( */
  /*     this->path.c_str(), aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs | */
  /*                             aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace | */
  /*                             aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FixInfacingNormals | */
  /*                             aiProcess_FindInvalidData | aiProcess_ValidateDataStructure); */

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    throw std::runtime_error("Failed to import model at: " + this->path.string() + ": " +
                             std::string(importer.GetErrorString()));
  }

  this->load_node(scene->mRootNode, scene);
}

void Model::load(const std::filesystem::path &path) {
  this->path = path;
  this->load();
}

void Model::load_node(aiNode *node, const aiScene *scene) {
  for (u32 i{0}; i < node->mNumMeshes; ++i) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    this->meshes.emplace_back(this->load_mesh(mesh, scene));
  }

  for (u32 i{0}; i < node->mNumChildren; ++i) {
    this->load_node(node->mChildren[i], scene);
  }
}

Mesh Model::load_mesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<u32> indices;
  std::vector<Texture> textures;
  std::vector<Material> materials;

  for (u32 i{0}; i < mesh->mNumVertices; ++i) {
    Vertex vert;
    vert.Position = glm::vec3{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
    vert.Normal = glm::vec3{mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
    if (mesh->HasTextureCoords(0)) {
      vert.TexCoords = glm::vec2{mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
    } else {
      vert.TexCoords = glm::vec2{0.0f, 0.0f};
    }
    vertices.push_back(vert);
  }

  for (u32 i{0}; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (u32 j{0}; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    // TODO(spyros): This should ideally be more complicated but this will do for this assignment
    // material->Get(AI_MATKEY_TEXTURE)
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      aiString str;
      material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
      std::filesystem::path model_dir = this->path.parent_path();
      textures.emplace_back(Texture{model_dir / std::string(str.C_Str()), Texture::DIFFUSE});
    }
    Material mat;
    aiColor3D ambient_color;
    material->Get(AI_MATKEY_COLOR_AMBIENT, ambient_color);
    mat.ambient = glm::vec3{ambient_color.r, ambient_color.g, ambient_color.b};

    aiColor3D diffuse_color;
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color);
    mat.diffuse = glm::vec3{diffuse_color.r, diffuse_color.g, diffuse_color.b};

    aiColor3D specular_color;
    material->Get(AI_MATKEY_COLOR_SPECULAR, specular_color);
    mat.specular = glm::vec3{specular_color.r, specular_color.g, specular_color.b};

    f32 shininess;
    material->Get(AI_MATKEY_SHININESS, shininess);
    mat.shininess = shininess;
    materials.push_back(mat);
  }

  return Mesh{std::move(vertices), std::move(indices), std::move(textures), std::move(materials),
              std::string(mesh->mName.C_Str())};
}

void Model::draw(Shader &shader) {
  for (auto &mesh : this->meshes) {
    mesh.draw(shader);
  }
}
