#ifndef MOD_RENDERER_WINDOW_VIEWPORT
#define MOD_RENDERER_WINDOW_VIEWPORT

#include <string>

#include <GL/glew.h>

#include "DgR3Vector.h"
#include "Mod_Renderer_Window.h"

namespace Renderer
{
  class Window_Viewport : public Window
  {
    typedef Dg::R3::Vector<float> vec4;

  public:

    Window_Viewport(InitData const &);
    ~Window_Viewport();

    void BeginDraw();  //Implement in Window
    void EndDraw();  //Implement in Window

    void Update();

  private:

    void DeleteBuffers();  //Implement in Window
    void ClearBuffers();  //Implement in Window
    void InitBuffers(InitData const &);  //Implement in Window
    GLuint GetTexture() const;  //Implement in Window

  private:

    unsigned    m_width;
    unsigned    m_height;
    bool        m_useDepthTest;
    vec4        m_clearColor;

    GLuint      m_shaderProgram;
    GLuint      m_vao;

    GLuint      m_frameBuffer;
    GLuint      m_texture;
    GLuint      m_rbo;
  };
}

#endif