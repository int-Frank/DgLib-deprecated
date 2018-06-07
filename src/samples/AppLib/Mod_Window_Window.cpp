#include <exception>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Window_Window.h"

namespace Window
{
  typedef Dg::R3::Vector<float> vec4;

  static void ResizeCallbackDefault(Window * a_pWindow, int a_w, int a_h)
  {
    a_pWindow->Resize(a_w, a_h);
  }

  static void(*g_ResizeCallback)(Window *, int, int) = ResizeCallbackDefault;

  void Init(void(*a_ResizeCallback)(Window *, int, int))
  {
    if (a_ResizeCallback)
    {
      g_ResizeCallback = a_ResizeCallback;
    }
  }

  void impl::ResizeCallback(Window * a_pWindow, int a_w, int a_h)
  {
    g_ResizeCallback(a_pWindow, a_w, a_h);
  }

  class excptBase: public std::exception
  {
  public:

    excptBase(std::string const & a_meta)
    {
      m_meta = m_meta; 
    }

    virtual const char* what() const throw()
    {
      return m_meta.c_str();
    }

  protected:

    std::string GetMeta() const
    {
      return m_meta;
    }

  private:

    std::string m_meta;
  };

  class ex_WindowInitFail: public excptBase
  {
  public:

    ex_WindowInitFail(std::string a_meta = "")
      : excptBase(a_meta)
    {}

    virtual const char* what() const throw()
    {
      std::string meta = GetMeta();
      std::string msg = "Window failed to initialise";
      if (!meta.empty())
      {
        msg = msg + ": " + meta;
      }
      return msg.c_str();
    }
  };

  class Window::PIMPL
  {
  public:

    PIMPL(InitData const &);
    ~PIMPL();

    void Resize(unsigned a_w, unsigned a_h);

    void BeginDraw();
    void EndDraw();

    void DeleteBuffers();
    void ClearBuffers();
    void InitBuffers(unsigned w, unsigned h, 
                     bool useDepthTest,
                     unsigned colorFormat,
                     vec4 clearColor);

    GLuint GetTexture() const;
    unsigned GetWidth() const;
    unsigned GetHeight() const;

  private:

    //No copy operations.
    PIMPL(PIMPL const &);
    PIMPL & operator=(PIMPL const &);

  private:

    unsigned    m_width;
    unsigned    m_height;
    bool        m_useDepthTest;
    GLuint      m_colorFormat;
    vec4        m_clearColor;

    GLuint      m_frameBuffer;
    GLuint      m_texture;
    GLuint      m_rbo;
  };

  Window::PIMPL::PIMPL(InitData const & a_data)
    : m_frameBuffer(0)
    , m_texture(0)
    , m_rbo(0)
  {
    GLuint clrFormat = 0;
    switch (a_data.colorFormat)
    {
      case E_RGB: clrFormat = GL_RGB; break;
      case E_RGBA:
      default:    clrFormat = GL_RGBA; break;
    }

    InitBuffers(a_data.width, a_data.height,
                a_data.useDepthTest,
                clrFormat,
                a_data.clearColor);
  }

  Window::PIMPL::~PIMPL()
  {
    DeleteBuffers();
  }

  void Window::PIMPL::Resize(unsigned a_w, unsigned a_h)
  {
    DeleteBuffers();
    InitBuffers(a_w, a_h, m_useDepthTest, m_colorFormat, m_clearColor);
  }

  void Window::PIMPL::BeginDraw()
  {
    ClearBuffers();

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    if (m_useDepthTest)
    {
      glEnable(GL_DEPTH_TEST);
    }
    else
    {
      glDisable(GL_DEPTH_TEST);
    }

    glViewport(0, 0, m_width, m_height);
  }

  void Window::PIMPL::EndDraw()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void Window::PIMPL::DeleteBuffers()
  {
    glDeleteFramebuffers(1, &m_frameBuffer);
    glDeleteTextures(1, &m_texture);
    if (m_useDepthTest)
    {
      glDeleteRenderbuffers(1, &m_rbo);
    }
    m_frameBuffer = 0;
    m_texture = 0;
    m_rbo = 0;
  }

  void Window::PIMPL::ClearBuffers()
  {
    GLuint flag = GL_COLOR_BUFFER_BIT;
    if (m_useDepthTest)
    {
      flag |= GL_DEPTH_BUFFER_BIT;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glClearColor(m_clearColor[0]
      , m_clearColor[1]
      , m_clearColor[2]
      , m_clearColor[3]);
    glClear(flag);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void Window::PIMPL::InitBuffers(unsigned a_w, unsigned a_h, 
                                  bool a_useDepthTest,
                                  unsigned a_colorFormat,
                                  vec4 a_clearColor)
  {
    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

    // generate texture
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, a_colorFormat, a_w, a_h, 0, a_colorFormat, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    if(a_useDepthTest)
    {
      glGenRenderbuffers(1, &m_rbo);
      glBindRenderbuffer(GL_RENDERBUFFER, m_rbo); 
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, a_w, a_h);  
      glBindRenderbuffer(GL_RENDERBUFFER, 0);

      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      throw ex_WindowInitFail();
    }
    else
    {
      m_width = a_w;
      m_height = a_h;
      m_useDepthTest = a_useDepthTest;
      m_colorFormat = a_colorFormat;
      m_clearColor = a_clearColor;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  GLuint Window::PIMPL::GetTexture() const
  {
    return m_texture;
  }

  GLuint Window::PIMPL::GetWidth() const
  {
    return m_width;
  }

  GLuint Window::PIMPL::GetHeight() const
  {
    return m_height;
  }

  Window::Window(InitData const & a_data)
    : m_pimpl(new PIMPL(a_data))
  {

  }

  Window::~Window()
  {
    delete m_pimpl;
  }

  void Window::BeginDraw()
  {
    m_pimpl->BeginDraw();
  }

  void Window::Resize(unsigned a_w, unsigned a_h)
  {
    m_pimpl->Resize(a_w, a_h);
  }

  void Window::EndDraw()
  {
    m_pimpl->EndDraw();
  }

  void Window::DeleteBuffers()
  {
    m_pimpl->DeleteBuffers();
  }

  void Window::ClearBuffers()
  {
    m_pimpl->ClearBuffers();
  }

  void Window::InitBuffers(unsigned a_w, unsigned a_h, 
                           bool a_useDepthTest,
                           unsigned a_colorFormat,
                           vec4 a_clearColor)
  {
    m_pimpl->InitBuffers(a_w, a_h, a_useDepthTest, a_colorFormat, a_clearColor);
  }

  GLuint Window::GetTexture() const
  {
    return m_pimpl->GetTexture();
  }

  GLuint Window::GetWidth() const
  {
    return m_pimpl->GetWidth();
  }

  GLuint Window::GetHeight() const
  {
    return m_pimpl->GetHeight();
  }
}