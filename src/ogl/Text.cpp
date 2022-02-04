#include <fstream>
#include <string>

#include <ogl/Shader.h>
#include <ogl/Text.h>
#include <ogl/VertexArray.h>
#include <ogl/VertexBuffer.h>
#include <third_party/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::string read_file(const std::filesystem::path &path) {
  std::ifstream in(path, std::ios::in | std::ios::binary);
  if (!in) {
    throw std::runtime_error("Could open file for reading: " + path.string());
  }
  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(contents.data(), contents.size());
  in.close();
  return contents;
}

Text::Text(const std::filesystem::path &runtime_path) {
  auto font_path = runtime_path / "textures/courier_new_0.png";
  auto font_meta_path = runtime_path / "textures/courier_new.fnt";
  auto font_meta = read_file(font_meta_path);
  i32 w = 0;
  i32 h = 0;
  i32 bpp = 0;
  auto font_content = stbi_load(font_path.c_str(), &w, &h, &bpp, 4);
  if (!font_content) {
    throw std::runtime_error("Could not read .png file: " + font_path.string());
  }

  GL_CALL(glGenTextures(1, &this->tex_id));
  GL_CALL(glBindTexture(GL_TEXTURE_2D, this->tex_id));
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, font_content));

  this->font = cute_font_load_bmfont(this->tex_id, font_meta.c_str(), font_meta.length(), nullptr);
  if (this->font->atlas_w != w) {
    throw std::runtime_error("Error parsing font: incorrect width");
  }
  if (this->font->atlas_h != h) {
    throw std::runtime_error("Error parsing font: incorrect height");
  }

  stbi_image_free(font_content);
}

Text::~Text() {
  if (this->font != nullptr) {
    cute_font_free(this->font);
  }
  if (this->tex_id != 0) {
    GL_CALL(glDeleteTextures(1, &this->tex_id));
  }
}

Text::Text(Text &&other) { *this = std::move(other); }
Text &Text::operator=(Text &&other) {
  if (this != &other) {
    this->font = other.font;
    this->tex_id = other.tex_id;
    other.tex_id = 0;
    other.font = nullptr;
  }

  return *this;
}

void Text::draw(const std::string &text, f32 x, f32 y, f32 screen_width, f32 screen_height) {
  glm::vec2 text_dimensions = this->text_dimensions(text);
  this->draw(text, x, y, screen_width, screen_height, text_dimensions.x, text_dimensions.y);
}

void Text::draw(const std::string &text, f32 x, f32 y, f32 screen_width, f32 screen_height, f32 text_width,
                f32 text_height) {
  VertexArray vao;
  VertexBuffer vbo;
  vao.bind();
  vbo.bind();

  f32 x0 = x + (-text_width) / 2; // This is negative because cute_font uses the DirectX X axis convention
  f32 y0 = y + text_height / 2;

  std::unique_ptr<cute_font_vert_t[]> vertices(new cute_font_vert_t[1024 * 2]);
  i32 n_vertices;
  cute_font_fill_vertex_buffer(this->font, text.c_str(), x0, y0, -1.0f, 2.0f, nullptr, vertices.get(), 1024 * 2,
                               &n_vertices);
  if (!n_vertices) {
    throw std::runtime_error("Could not fill vertex buffer for drawing text");
  }

  vbo.set_data(vertices.get(), sizeof(cute_font_vert_t) * n_vertices);
  VertexBufferLayout layout;
  layout.push({GL_FLOAT, 2});
  layout.push({GL_FLOAT, 2});
  vao.set_layout(layout);

  Shader shader{R"(#version 330
		in vec2 i_pos;
		in vec2 i_uv;
		uniform mat4 u_mvp;

		out vec2 o_uv;

		void main() {
			o_uv = i_uv;
			gl_Position = u_mvp * vec4(i_pos, 0, 1);
		}
	)",
                R"(#version 330

		in vec2 o_uv;

		uniform sampler2D u_tex;

		out vec4 color;

		void main() {
			color = texture(u_tex, o_uv);
		}
	)"};
  shader.bind();
  glm::mat4 projection = glm::ortho(0.0f, screen_width, 0.0f, screen_height);
  shader.set_uniformmat4f("u_mvp", projection);
  shader.set_uniform1i("u_tex", 0);

  GL_CALL(glBindTexture(GL_TEXTURE_2D, this->tex_id));
  GL_CALL(glActiveTexture(GL_TEXTURE0));

  GL_CALL(glDrawArrays(GL_TRIANGLES, 0, n_vertices));
}

glm::vec2 Text::text_dimensions(const std::string &text) const {
  f32 width = cute_font_text_width(this->font, text.c_str());
  f32 height = cute_font_text_height(this->font, text.c_str());

  return glm::vec2{width, height};
}
