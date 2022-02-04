#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

#include <filesystem>
#include <ogl/Core.h>
#include <string>

class Texture {
public:
  enum Type { NONE = 0, DIFFUSE, SPECULAR };

private:
  u32 rendererID = 0;
  std::filesystem::path filepath;
  byte *data = nullptr;
  bool loaded = false;
  u32 width = 0;
  u32 height = 0;
  u32 bpp = 0;
  Type type;

public:
  Texture();
  explicit Texture(const std::filesystem::path &path, Type type);
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;

  Texture(Texture &&);
  Texture &operator=(Texture &&);
  ~Texture();

  void load();
  void load(const std::filesystem::path &filepath, Type type);
  void bind(u32 slot = 0) const;
  void unbind() const;

  inline u32 get_width() const { return width; }
  inline u32 get_height() const { return height; }
};

#endif
