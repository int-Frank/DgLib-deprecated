#ifndef MOD_RENDERER_RENDERERTARGET
#define MOD_RENDERER_RENDERERTARGET

#include <string>

#include "DgR3Vector.h"

namespace Renderer
{
  class Window
  {
  public:

    typedef Dg::R3::Vector<float> vec4;

    enum Flag
    {
      E_Null = 0,

      //Color formats
      E_RGB,
      E_RGBA
    };

    class InitData
    {
    public:

      InitData()
        : name("Window")
        , width(400)
        , height(400)
        , indentX(100)
        , indentY(100)
        , colorFormat(E_RGBA)
        , useDepthTest(true)
        , clearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f))
        , ImGuiWindowFlags(0)
      {}

      std::string name;
      unsigned width;
      unsigned height;
      unsigned indentX;
      unsigned indentY;
      int colorFormat; //E_RGB, E_RGBA
      bool useDepthTest;
      vec4 clearColor;
      unsigned ImGuiWindowFlags;
    };

  public:

    Window(InitData &);
    ~Window();

    void BeginDraw();
    void EndDraw();

    void Update();

  private:

    //No copy operations.
    Window(Window const &);
    Window & operator=(Window const &);

    class PIMPL;
    PIMPL * m_pimpl;
  };
}

#endif