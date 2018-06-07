#ifndef MOD_WINDOW_WINDOW_VIEWPORT
#define MOD_WINDOW_WINDOW_VIEWPORT

#include <string>

#include <GL/glew.h>

#include "DgR3Vector.h"
#include "Mod_Window_Window.h"

namespace Window
{
  class Window_Viewport : public Window
  {
    typedef Dg::R3::Vector<float> vec4;

  public:

    Window_Viewport(InitData const &);
    ~Window_Viewport();

    void Update();

  private:

    GLuint m_shaderProgram;
    GLuint m_vao;
    GLuint m_vbo;
  };
}

#endif