#ifndef OGL_TEXT_H
#define OGL_TEXT_H

#include <filesystem>
#include <ogl/Core.h>
#include <vector>

#include <glm/glm.hpp>
#include <third_party/cute_font.h>

class Text {

  u32 tex_id;
  cute_font_t *font;

public:
  Text(const std::filesystem::path &runtime_path);
  ~Text();
  Text(const Text &) = delete;
  Text(Text &&);
  Text &operator=(Text &&);

  void draw(const std::string &text, f32 x, f32 y, f32 screen_width, f32 screen_height);
  void draw(const std::string &text, f32 x, f32 y, f32 screen_width, f32 screen_height, f32 text_width,
            f32 text_height);
  glm::vec2 text_dimensions(const std::string &text) const;
};

#endif
