
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Renderer_RenderTarget.h"

namespace Renderer
{
  class RenderTarget::PIMPL
  {
  public:

    PIMPL(InitData &);
    ~PIMPL();
    PIMPL(PIMPL const &);

    void Bind();
    void Unbind();

    InitData GetInfo() const;

    void ResetBuffers(vec4);

  private:

    void Clear();

  private:

    InitData  m_initData;

    GLuint    m_frameBuffer;
    GLuint    m_texColorBuffer;
    GLuint    m_rbo;
  };

  RenderTarget::PIMPL::PIMPL(PIMPL const & a_other)
    : m_initData(a_other.m_initData)
    , m_frameBuffer(a_other.m_frameBuffer)
    , m_texColorBuffer(a_other.m_texColorBuffer)
    , m_rbo(a_other.m_rbo)
  {

  }

  RenderTarget::PIMPL::~PIMPL()
  {
    Clear();
  }

  void RenderTarget::PIMPL::Clear()
  {
    glDeleteFramebuffers(1, &m_frameBuffer);
    m_frameBuffer = 0;
    m_texColorBuffer = 0;
    m_rbo = 0;
    m_initData.textureID = s_InvalidID;
  }

  RenderTarget::PIMPL::PIMPL(InitData & a_data)
    : m_frameBuffer(0)
    , m_texColorBuffer(0)
    , m_rbo(0)
  {
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
    glGenTextures(1, &m_texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, clrFormat, a_data.width, a_data.height, 0, clrFormat, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColorBuffer, 0);

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
      a_data.textureID = s_InvalidID;
      Clear();
    }
    else
    {
      m_initData = a_data;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void RenderTarget::PIMPL::Bind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    if (m_initData.useDepthTest)
    {
      glEnable(GL_DEPTH_TEST);
    }
    else
    {
      glDisable(GL_DEPTH_TEST);
    }
  }

  void RenderTarget::PIMPL::Unbind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void RenderTarget::PIMPL::ResetBuffers(vec4 a_clr)
  {
    GLuint flag = GL_COLOR_BUFFER_BIT;
    if (m_initData.useDepthTest)
    {
      flag |= GL_DEPTH_BUFFER_BIT;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glClearColor(a_clr[0], a_clr[1], a_clr[2], a_clr[3]);
    glClear(flag);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  RenderTarget::InitData RenderTarget::PIMPL::GetInfo() const
  {
    return m_initData;
  }

  RenderTarget::RenderTarget(InitData & a_data)
    : m_pimpl(new PIMPL(a_data))
  {

  }

  RenderTarget::RenderTarget(RenderTarget const & a_other)
    : m_pimpl(new PIMPL(*a_other.m_pimpl))
  {

  }

  RenderTarget & RenderTarget::operator=(RenderTarget const & a_other)
  {
    if (this != &a_other)
    {
      delete m_pimpl;
      m_pimpl = new PIMPL(*a_other.m_pimpl);
    }
    return *this;
  }

  RenderTarget::~RenderTarget()
  {
    delete m_pimpl;
  }

  void RenderTarget::Bind()
  {
    m_pimpl->Bind();
  }

  void RenderTarget::Unbind()
  {
    m_pimpl->Unbind();
  }

  RenderTarget::InitData RenderTarget::GetInfo() const
  {
    return m_pimpl->GetInfo();
  }

  void RenderTarget::ResetBuffers(vec4 a_clr)
  {
    m_pimpl->ResetBuffers(a_clr);
  }
}