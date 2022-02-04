#ifndef OGL_SHADER_H
#define OGL_SHADER_H

#include <filesystem>
#include <ogl/Core.h>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct ShaderProgramSource {
  std::string vertex;
  std::string fragment;
};

class Shader {

  std::string shader_path;
  u32 rendererID = 0;
  std::unordered_map<std::string, i32> uniform_location_cache;

public:
  explicit Shader();
  explicit Shader(const std::filesystem::path &filepath);
  explicit Shader(const std::string &unified_shader);
  explicit Shader(const std::string &vertex_shader, const std::string &fragment_shader);
  Shader(Shader &) = delete;
  Shader(Shader &&);
  Shader &operator=(Shader &&);
  ~Shader();

  void reload();
  void load(const std::filesystem::path &path);
  void load(const std::string &unified_shader);
  void load(const std::string &vertex_shader, const std::string &fragment_shader);
  void bind() const;
  void unbind() const;

  void set_uniform3f(const std::string &name, f32 v0, f32 v1, f32 v2);
  void set_uniform3f(const std::string &name, const glm::vec3 &vec);

  void set_uniform4f(const std::string &name, f32 v0, f32 v1, f32 v2, f32 v3);
  void set_uniform4f(const std::string &name, const glm::vec4 &vec);

  void set_uniform1i(const std::string &name, i32 v);
  void set_uniform1f(const std::string &name, f32 v);
  void set_uniformmat4f(const std::string &name, const glm::mat4 &matrix);

  bool uniform_exists(const std::string &name);

private:
  u32 get_uniform_location(const std::string &name);
  u32 compile_shader(u32 type, const std::string &source);
  u32 create_shader_program(const std::string &vertex_shader, const std::string &fragment_shader);
  ShaderProgramSource parse_unified_shader(std::istream &unified_shader_stream);
  ShaderProgramSource parse_unified_shader(const std::filesystem::path &shader_path);
};

#endif
