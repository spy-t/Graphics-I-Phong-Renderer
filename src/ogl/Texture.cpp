#include <filesystem>
#include <ogl/Texture.h>
#include <third_party/stb_image.h>

Texture::Texture() { GL_CALL(glGenTextures(1, &this->rendererID)); }

Texture::Texture(const std::filesystem::path &path, Type type) : filepath(path), type{type} {
  GL_CALL(glGenTextures(1, &this->rendererID));
}

Texture::Texture(Texture &&other) { *this = std::move(other); }
Texture &Texture::operator=(Texture &&other) {
  if (this != &other) {
    this->rendererID = other.rendererID;
    this->data = other.data;
    this->loaded = other.loaded;
    this->filepath = other.filepath;
    this->width = other.width;
    this->height = other.height;
    this->bpp = other.bpp;
    this->type = other.type;

    other.rendererID = 0;
  }

  return *this;
}

Texture::~Texture() {
  if (this->rendererID != 0) {
    GL_CALL(glDeleteTextures(1, &this->rendererID));
  }
}

void Texture::load() {
  if (this->loaded) {
    return;
  }
  if (filepath.empty()) {
    throw std::runtime_error("Texture filepath not initialized");
  }
  // stbi_set_flip_vertically_on_load(1);
  this->data = stbi_load(this->filepath.c_str(), (i32 *)&this->width, (i32 *)&this->height, (i32 *)&this->bpp, 4);
  if (!this->data) {
    throw std::runtime_error("Could not load texture: " + this->filepath.string());
  }
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GL_CALL(
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->data));
  GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

  this->loaded = true;
  stbi_image_free(this->data);
}

void Texture::load(const std::filesystem::path &path, Type type) {
  this->filepath = path.string();
  this->type = type;
  this->load();
}

void Texture::bind(u32 slot) const {
  GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
  GL_CALL(glBindTexture(GL_TEXTURE_2D, this->rendererID));
}
void Texture::unbind() const { GL_CALL(glBindTexture(GL_TEXTURE_2D, 0)); }
