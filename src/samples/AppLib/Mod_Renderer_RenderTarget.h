#ifndef MOD_RENDERER_RENDERERTARGET
#define MOD_RENDERER_RENDERERTARGET

#include <stdint.h>

#include "DgR3Vector.h"

namespace Renderer
{
  class RenderTarget
  {
    friend class Renderer;

    typedef Dg::R3::Vector<float> vec4;

    enum Flag
    {
      E_Null = 0,

      //Color formats
      E_RGB,
      E_RGBA
    };
  public:

    static unsigned const s_InvalidID = -1;

    struct InitData
    {
      //In
      unsigned width;
      unsigned height;
      int colorFormat; //E_RGB, E_RGBA
      bool useDepthTest;

      //Out
      unsigned textureID;
    };

  public:

    //Sets InitData.textureID
    RenderTarget(InitData &);
    ~RenderTarget();

    RenderTarget(RenderTarget const &);
    RenderTarget & operator=(RenderTarget const &);

    InitData GetInfo() const;
    unsigned GetID() const;

    void Bind();
    void Unbind();

    void ResetBuffers(vec4 = vec4(0.0f, 0.0f, 0.0f, 1.0f));

  private:

    class PIMPL;
    PIMPL * m_pimpl;
  };
}

#endif