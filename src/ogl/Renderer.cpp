#include <ogl/Core.h>
#include <ogl/Renderer.h>

Renderer::Renderer(u32 width, u32 height) : render_width{width}, render_height{height} {}

void Renderer::clear() const { GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); }

void Renderer::draw(const VertexArray &va, const ElementBuffer &eb, const Shader &shader) const {
  va.bind();
  eb.bind();
  shader.bind();

  GL_CALL(glDrawElements(GL_TRIANGLES, eb.get_count(), GL_UNSIGNED_INT, nullptr));
}
