#ifndef MOD_WINDOW_WINDOW
#define MOD_WINDOW_WINDOW

#include <string>

#include <GL/glew.h>

#include "DgR3Vector.h"

namespace Window
{
  class Window;

  //We init with a Resize callback in case you want to resize
  //the window at a different time.
  void Init(void(*ResizeCallback)(Window *, int, int));

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
      , width(600)
      , height(500)
      , indentX(300)
      , indentY(100)
      , colorFormat(E_RGBA)
      , useDepthTest(true)
      , isImGuiWindow(true)
      , clearColor(Dg::R3::Vector<float>(0.0f, 0.2f, 0.2f, 1.0f))
      , ImGuiWindowFlags(0)
    {}

    std::string name;
    unsigned width;
    unsigned height;
    unsigned indentX;
    unsigned indentY;
    int colorFormat; //E_RGB, E_RGBA
    bool useDepthTest;
    bool isImGuiWindow;
    Dg::R3::Vector<float> clearColor;
    unsigned ImGuiWindowFlags;
  };

  namespace impl
  {
    void ResizeCallback(Window *, int, int);
  }

  //Use WindowFactory below to create windows. This is just the base class.
  class Window
  {
  public:

    Window(InitData const &);
    virtual ~Window();

    void Resize(unsigned w, unsigned h);

    void BeginDraw();
    void EndDraw();
    virtual void Update()    {}

  protected:

    void DeleteBuffers();
    void ClearBuffers();
    void InitBuffers(unsigned w, unsigned h, 
                     bool useDepthTest,
                     unsigned colorFormat,
                     Dg::R3::Vector<float> clearColor);

    GLuint GetTexture() const;
    unsigned GetWidth() const;
    unsigned GetHeight() const;

  private:

    //No copy operations.
    Window(Window const &);
    Window & operator=(Window const &);

  private:

    class PIMPL;
    PIMPL * m_pimpl;
  };

  class WindowFactory
  {
  public:

    Window * GetNewWindow(InitData const &);
  };
}

#endif