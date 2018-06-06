
#include <exception>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Renderer_Window.h"
#include "imgui.h"

namespace Renderer
{
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

    PIMPL(InitData &);
    ~PIMPL();
    PIMPL(PIMPL const &);

    void BeginDraw();
    void EndDraw();

    void Update();

    void ClearBuffers();

  private:

    void Clear();

  private:

    std::string m_name;
    unsigned    m_width;
    unsigned    m_height;
    bool        m_useDepthTest;
    vec4        m_clearColor;

    GLuint      m_frameBuffer;
    GLuint      m_texture;
    GLuint      m_rbo;
  };

  Window::PIMPL::PIMPL(PIMPL const & a_other)
    : m_name(a_other.m_name)
    , m_width(a_other.m_width)
    , m_height(a_other.m_height)
    , m_useDepthTest(a_other.m_useDepthTest)
    , m_clearColor(a_other.m_clearColor)
    , m_frameBuffer(a_other.m_frameBuffer)
    , m_texture(a_other.m_texture)
    , m_rbo(a_other.m_rbo)
  {

  }

  Window::PIMPL::~PIMPL()
  {
    Clear();
  }

  void Window::PIMPL::Clear()
  {
    glDeleteFramebuffers(1, &m_frameBuffer);
    m_frameBuffer = 0;
    m_texture = 0;
    m_rbo = 0;
  }

  Window::PIMPL::PIMPL(InitData & a_data)
    : m_frameBuffer(0)
    , m_texture(0)
    , m_rbo(0)
  {
    float marginX = 9.0f;
    float marginY = 18.0f;
    float width = float(a_data.width) + 2.0f * marginX;
    float height = float(a_data.height) + 2.0f * marginY;

    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::SetNextWindowPos(ImVec2(float(a_data.indentX), float(a_data.indentY)));
    ImGui::Begin(a_data.name.c_str(), nullptr, a_data.ImGuiWindowFlags);
    ImGui::End();

    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

    GLuint clrFormat = 0;
    switch (a_data.colorFormat)
    {
      case E_RGB: clrFormat = GL_RGB; break;
      case E_RGBA:
      default:    clrFormat = GL_RGBA; break;
    }

    // generate texture
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, clrFormat, a_data.width, a_data.height, 0, clrFormat, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    if(a_data.useDepthTest)
    {
      glGenRenderbuffers(1, &m_rbo);
      glBindRenderbuffer(GL_RENDERBUFFER, m_rbo); 
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, a_data.width, a_data.height);  
      glBindRenderbuffer(GL_RENDERBUFFER, 0);

      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      throw ex_WindowInitFail();
    }
    else
    {
      m_name = a_data.name;
      m_width = a_data.width;
      m_height = a_data.height;
      m_useDepthTest = a_data.useDepthTest;
      m_clearColor = a_data.clearColor;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

  void Window::PIMPL::Update()
  {
    ImGui::Begin(m_name.c_str());

    ImGui::GetWindowDrawList()->AddImage(
      (void *)m_texture, 
      ImVec2(ImGui::GetCursorScreenPos()),
      ImVec2(ImGui::GetCursorScreenPos().x + m_width, 
        ImGui::GetCursorScreenPos().y + m_height), 
      ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
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

  Window::Window(InitData & a_data)
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

  void Window::EndDraw()
  {
    m_pimpl->EndDraw();
  }

  void Window::Update()
  {
    m_pimpl->Update();
  }
}