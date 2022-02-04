#ifndef OGL_EXAMPLE_H
#define OGL_EXAMPLE_H

#include <ogl/Core.h>

#include <iostream>

class Scene {
public:
  Scene() {}
  virtual ~Scene() {}

  virtual void on_update(f32 delta) {}
  virtual void on_render() {}
};

#endif
