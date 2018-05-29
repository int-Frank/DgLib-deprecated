
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Renderer_Main.h"
#include "Mod_Renderer_ContextLine.h"
#include "Mod_Renderer_ContextTriangle.h"

#include "Mod_Renderer_TriangleMesh.h"
#include "Mod_Renderer_LineMesh.h"
#include "DgR3Matrix.h"
#include "Mod_Renderer_Logger.h"
#include "DgTypes.h"

namespace Renderer
{
  void Init(void(*Log)(std::string const &, int))
  {
    Logger::Init(Log);
  }

  class Renderer::PIMPL
  {
  public:

    PIMPL();
    ~PIMPL();

    void Clear();

    void ClearLoadList();
    ObjectHandle  AddObject(TriangleMesh const &);
    ObjectHandle  AddObject(LineMesh const &);
    void CommitLoadList();

    void SetTransform(Dg::R3::Matrix<float> const &);
    void SetColor(Dg::R3::Vector<float> const &);
    void SetRenderTarget();

    void SetContext(Contexts);
    void BeginDraw();
    void Draw(ObjectHandle);
    void EndDraw();

  private:

    ObjectHandle GetHandle(int type, int id) const;
    int GetType(ObjectHandle) const;
    int GetID(ObjectHandle) const;

  private:

    Context *                   m_pCurrentContext;
    Context *                   m_contexts[E_NoContext]; //Lines, Polygons

    std::vector<LineMesh>       m_loadListLines;
    std::vector<TriangleMesh>   m_loadListTriangles;
  };

  Renderer::PIMPL::PIMPL()
    : m_contexts{nullptr, nullptr}
    , m_pCurrentContext(nullptr)
  {
    m_contexts[E_Lines] = new ContextLine();
    m_contexts[E_Triangles] = new ContextTriangle();
  }

  Renderer::PIMPL::~PIMPL()
  {
    m_pCurrentContext = nullptr;
    for (int i = 0; i < E_NoContext; i++)
    {
      delete m_contexts[i];
      m_contexts[i] = nullptr;
    }
  }

  void Renderer::PIMPL::Clear()
  {
    ClearLoadList();

    for (int i = 0; i < E_NoContext; i++)
    {
      m_contexts[i]->Clear();
    }
  }

  void Renderer::PIMPL::ClearLoadList()
  {
    m_loadListLines.clear();
    m_loadListTriangles.clear();
  }

  ObjectHandle Renderer::PIMPL::AddObject(TriangleMesh const & a_mesh)
  {
    m_loadListTriangles.push_back(a_mesh);
    return GetHandle(E_Triangles, static_cast<int>(m_loadListTriangles.size()) - 1);
  }

  ObjectHandle Renderer::PIMPL::AddObject(LineMesh const & a_mesh)
  {
    m_loadListLines.push_back(a_mesh);
    return GetHandle(E_Lines, static_cast<int>(m_loadListLines.size()) - 1);
  }

  void Renderer::PIMPL::CommitLoadList()
  {
    m_contexts[E_Lines]->LoadData(m_loadListLines);
    m_contexts[E_Triangles]->LoadData(m_loadListTriangles);
    ClearLoadList();
  }

  void Renderer::PIMPL::SetTransform(Dg::R3::Matrix<float> const & a_mat)
  {
    if (m_pCurrentContext)
    {
      m_pCurrentContext->SetMatrix(a_mat);
    }
  }

  void Renderer::PIMPL::SetColor(Dg::R3::Vector<float> const & a_clr)
  {
    if (m_pCurrentContext)
    {
      m_pCurrentContext->SetColor(a_clr);
    }
  }

  void Renderer::PIMPL::SetRenderTarget()
  {

  }

  void Renderer::PIMPL::SetContext(Contexts a_context)
  {
    if (m_pCurrentContext)
    {
      m_pCurrentContext->DeactivateContext();
    }

    switch (a_context)
    {
      case E_Lines:
      case E_Triangles:
      {
        m_pCurrentContext = m_contexts[a_context];
        m_pCurrentContext->ActivateContext();
        break;
      }
      case E_NoContext:
      {
        m_pCurrentContext = nullptr;
        break;
      }
      default:
      {
        m_pCurrentContext = nullptr;
        Logger::Log("Unrecognised context sent to SetContext()", Dg::LL_Warning);
      }
    }
  }

  void Renderer::PIMPL::BeginDraw()
  {
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void Renderer::PIMPL::Draw(ObjectHandle a_h)
  {
    if (m_pCurrentContext)
    {
      m_pCurrentContext->Draw(GetID(a_h));
    }
  }

  ObjectHandle Renderer::PIMPL::GetHandle(int a_type, int a_id) const
  {
    int halfSize = sizeof(ObjectHandle) * 4;
    ObjectHandle mask = (ObjectHandle(1) << halfSize) - 1;

    return (a_type << halfSize) | (a_id & mask);
  }

  int Renderer::PIMPL::GetType(ObjectHandle a_handle) const
  {
    return a_handle >> (sizeof(ObjectHandle) * 4);
  }

  int Renderer::PIMPL::GetID(ObjectHandle a_handle) const
  {
    int halfSize = sizeof(ObjectHandle) * 4;
    ObjectHandle mask = (ObjectHandle(1) << halfSize) - 1;
    return a_handle & mask;
  }

  void Renderer::PIMPL::EndDraw()
  {

  }

  Renderer::Renderer()
    : m_pimpl(new PIMPL())
  {
  }

  Renderer::~Renderer()
  {
    delete m_pimpl;
    m_pimpl = nullptr;
  }

  void Renderer::Clear()
  {
    m_pimpl->Clear();
  }

  void Renderer::ClearLoadList()
  {
    m_pimpl->ClearLoadList();
  }

  ObjectHandle Renderer::AddObject(TriangleMesh const & a_mesh)
  {
    return m_pimpl->AddObject(a_mesh);
  }

  ObjectHandle Renderer::AddObject(LineMesh const & a_mesh)
  {
    return m_pimpl->AddObject(a_mesh);
  }

  void Renderer::CommitLoadList()
  {
    m_pimpl->CommitLoadList();
  }

  void Renderer::SetTransform(Dg::R3::Matrix<float> const & a_matrix)
  {
    m_pimpl->SetTransform(a_matrix);
  }

  void Renderer::SetRenderTarget()
  {
    m_pimpl->SetRenderTarget();
  }

  void Renderer::BeginDraw()
  {
    m_pimpl->BeginDraw();
  }

  void Renderer::SetContext(Contexts a_context)
  {
    m_pimpl->SetContext(a_context);
  }

  void Renderer::Draw(ObjectHandle a_handle)
  {
    m_pimpl->Draw(a_handle);
  }

  void Renderer::EndDraw()
  {
    m_pimpl->EndDraw();
  }

  void Renderer::SetColor(Dg::R3::Vector<float> const & a_clr)
  {
    m_pimpl->SetColor(a_clr);
  }
}
