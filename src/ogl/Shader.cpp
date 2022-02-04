#include <ogl/Shader.h>

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader() {}

Shader::Shader(const std::filesystem::path &filepath) : shader_path{filepath.string()}, rendererID{0} {
  this->load(filepath);
}

Shader::Shader(const std::string &unified_shader) : shader_path(""), rendererID(0) { this->load(unified_shader); }

Shader::Shader(const std::string &vertex_shader, const std::string &fragment_shader) : shader_path(""), rendererID(0) {
  this->load(vertex_shader, fragment_shader);
}

Shader::Shader(Shader &&other) { *this = std::move(other); }
Shader &Shader::operator=(Shader &&other) {
  if (this != &other) {
    this->rendererID = other.rendererID;
    this->shader_path = other.shader_path;
    other.rendererID = 0;
  }

  return *this;
}

Shader::~Shader() {
  if (this->rendererID != 0) {
    GL_CALL(glDeleteProgram(this->rendererID));
  }
}

void Shader::load(const std::filesystem::path &path) {
  auto shader_source = parse_unified_shader(path);

  this->rendererID = create_shader_program(shader_source.vertex, shader_source.fragment);
}
void Shader::load(const std::string &unified_shader) {
  auto shader_source = parse_unified_shader(unified_shader);

  this->rendererID = create_shader_program(shader_source.vertex, shader_source.fragment);
}
void Shader::load(const std::string &vertex_shader, const std::string &fragment_shader) {
  this->rendererID = create_shader_program(vertex_shader, fragment_shader);
}

void Shader::reload() {
  if (this->shader_path.empty()) {
    return;
  }

  this->load(this->shader_path);
}

ShaderProgramSource Shader::parse_unified_shader(std::istream &unified_shader) {
  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (std::getline(unified_shader, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::FRAGMENT;
      } else {
        throw std::runtime_error("Could not parse shader: unknown shader type");
      }
    } else {
      ss[(int)type] << line << '\n';
    }
  }

  return {/* vertex */ ss[0].str(), /* fragment */ ss[1].str()};
}

ShaderProgramSource Shader::parse_unified_shader(const std::filesystem::path &shader_path) {
  std::ifstream f(shader_path);
  if (!f) {
    throw std::runtime_error("Failed to open file " + shader_path.string());
  }
  return parse_unified_shader(f);
}

u32 Shader::compile_shader(u32 type, const std::string &source) {

  u32 id = glCreateShader(type);
  const char *src = source.c_str();
  GL_CALL(glShaderSource(id, 1, &src, nullptr));
  GL_CALL(glCompileShader(id));

  i32 result;
  GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    int length;
    GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    std::string msg;
    msg.reserve(length);
    GL_CALL(glGetShaderInfoLog(id, length, &length, msg.data()));
    std::string t = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
    GL_CALL(glDeleteShader(id));
    throw std::runtime_error("Failed to compile " + t + " shader: " + msg);
    return 0;
  }

  return id;
}

u32 Shader::create_shader_program(const std::string &vertex_shader, const std::string &fragment_shader) {
  GL_CALL(u32 program = glCreateProgram());
  u32 vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
  u32 fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

  GL_CALL(glAttachShader(program, vs));
  GL_CALL(glAttachShader(program, fs));
  GL_CALL(glLinkProgram(program));
  GL_CALL(glValidateProgram(program));

  GL_CALL(glDeleteShader(vs));
  GL_CALL(glDeleteShader(fs));

  return program;
}

void Shader::bind() const {
  if (this->rendererID == 0) {
    throw std::runtime_error("Shader has not been initialized");
  }
  GL_CALL(glUseProgram(this->rendererID));
}
void Shader::unbind() const { GL_CALL(glUseProgram(0)); }

void Shader::set_uniform3f(const std::string &name, f32 v0, f32 v1, f32 v2) {
  u32 uniform_location = get_uniform_location(name);
  GL_CALL(glUniform3f(uniform_location, v0, v1, v2));
}

void Shader::set_uniform3f(const std::string &name, const glm::vec3 &vec) {
  this->set_uniform3f(name, vec.x, vec.y, vec.z);
}

void Shader::set_uniform4f(const std::string &name, f32 v0, f32 v1, f32 v2, f32 v3) {
  u32 uniform_location = get_uniform_location(name);
  GL_CALL(glUniform4f(uniform_location, v0, v1, v2, v3));
}

void Shader::set_uniform4f(const std::string &name, const glm::vec4 &vec) {
  this->set_uniform4f(name, vec.x, vec.y, vec.z, vec.w);
}

void Shader::set_uniform1i(const std::string &name, i32 v) {
  u32 uniform_location = get_uniform_location(name);
  GL_CALL(glUniform1i(uniform_location, v));
}

void Shader::set_uniform1f(const std::string &name, f32 v) {
  u32 uniform_location = get_uniform_location(name);
  GL_CALL(glUniform1f(uniform_location, v));
}

void Shader::set_uniformmat4f(const std::string &name, const glm::mat4 &matrix) {
  u32 uniform_location = get_uniform_location(name);
  GL_CALL(glUniformMatrix4fv(uniform_location, 1, GL_FALSE, &matrix[0][0]));
}

u32 Shader::get_uniform_location(const std::string &name) {
  auto cached_uniform_location = uniform_location_cache.find(name);
  if (cached_uniform_location != uniform_location_cache.end()) {
    return cached_uniform_location->second;
  }
  GL_CALL(i32 uniform_location = glGetUniformLocation(this->rendererID, name.c_str()));
  if (uniform_location == -1) {
    throw std::runtime_error("uniform does not exist: " + name);
  }

  uniform_location_cache[name] = uniform_location;
  return uniform_location;
}

bool Shader::uniform_exists(const std::string &name) {
  auto cached_uniform_location = uniform_location_cache.find(name);
  if (cached_uniform_location != uniform_location_cache.end()) {
    return cached_uniform_location->second != -1;
  }
  GL_CALL(i32 uniform_location = glGetUniformLocation(this->rendererID, name.c_str()));

  uniform_location_cache[name] = uniform_location;
  return uniform_location != -1;
}
