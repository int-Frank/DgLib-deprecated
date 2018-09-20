
#include <vector>
#include <string>
#include <exception>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Context_Logger.h"
#include "DgTypes.h"
#include "Mod_Context_Triangle.h"
#include "Mod_Context_Common.h"

namespace Context
{
  class excptTypeError: public std::exception
  {
  public:

    virtual const char* what() const throw()
    {
      return "Object handle is the wrong type for this context";
    }
  };

  class ContextTriangle::PIMPL
  {
  public:

    PIMPL();
    ~PIMPL();

    void ClearLoadList();
    ObjectHandle AddObject(TriangleMesh const &);
    void CommitLoadList();

    void Bind();
    void Draw(ObjectHandle);
    void Unbind();

    void Clear();
    void SetMatrix(Dg::R3::Matrix<float> const &);
    void SetColor(ObjectHandle, Dg::R3::Vector<float> const &);

  private:

    struct GPU_MeshVertex
    {
      float point[4];
    };

    struct GPU_Data
    {
      std::vector<GPU_MeshVertex> vertexData;
      std::vector<GLuint>         indexData;
    };

    struct MeshData
    {
      GLint                 offset;
      GLsizei               count;
      Dg::R3::Vector<float> color;
      bool                  depthEnabled;
    };

  private:

    //Also sets m_currentObjects;
    GPU_Data CollateData();
    void InitShaderProgram();
    void LoadData();

  private:

    GL_State m_savedState;

    std::vector<MeshData>     m_currentObjects;
    std::vector<TriangleMesh> m_loadList;

    GLuint m_shaderProgram;
    GLuint m_vao;
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
  };

  ContextTriangle::PIMPL::PIMPL()
    : m_shaderProgram(0)
    , m_vao(0)
    , m_vertexBuffer(0)
    , m_indexBuffer(0)
  {
    InitShaderProgram();
  }

  ContextTriangle::PIMPL::~PIMPL()
  {
    Clear();
    glDeleteProgram(m_shaderProgram);
  }

  void ContextTriangle::PIMPL::ClearLoadList()
  {
    m_loadList.clear();
  }

  void ContextTriangle::PIMPL::CommitLoadList()
  {
    Clear();
    LoadData();
    ClearLoadList();
  }

  ObjectHandle ContextTriangle::PIMPL::AddObject(TriangleMesh const & a_obj)
  {
    ObjectHandle handle = GetObjectHandle(gContextTriangleID, (uint32_t)m_loadList.size());
    m_loadList.push_back(a_obj);
    return handle;
  }

  void ContextTriangle::PIMPL::LoadData()
  {
    Clear();

    if (m_loadList.empty())
    {
      return;
    }

    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_indexBuffer);

    GPU_Data gpuData = CollateData();

    //Vertices
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 
      gpuData.vertexData.size() * sizeof(GPU_MeshVertex), 
      gpuData.vertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
      gpuData.indexData.size() * sizeof(GLuint),
      gpuData.indexData.data(), GL_STATIC_DRAW);

    GLuint loc_position = glGetAttribLocation(m_shaderProgram, "in_position");

    glVertexAttribPointer(loc_position, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(loc_position);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  //Also sets m_currentObjects;
  ContextTriangle::PIMPL::GPU_Data  ContextTriangle::PIMPL::CollateData()
  {
    GPU_Data out;

    GLuint vertexOffset = 0;
    GLuint indexOffset = 0;
    out.indexData.clear();
    out.vertexData.clear();

    for (auto const & mesh : m_loadList)
    {
      MeshData meshData;
      meshData.offset = (GLuint)out.indexData.size() * sizeof(GLuint);
      meshData.count = GLsizei(mesh.triangles.size() * 3);
      meshData.color = mesh.color;
      meshData.depthEnabled = mesh.depthEnabled;
      m_currentObjects.push_back(meshData);

      for (auto vert : mesh.verts)
      {
        GPU_MeshVertex v;
        v.point[0] = vert.point[0];
        v.point[1] = vert.point[1];
        v.point[2] = vert.point[2];
        v.point[3] = 1.0f;

        out.vertexData.push_back(v);
      }

      for (auto tri : mesh.triangles)
      {
        out.indexData.push_back(tri.indices[0] + vertexOffset);
        out.indexData.push_back(tri.indices[1] + vertexOffset);
        out.indexData.push_back(tri.indices[2] + vertexOffset);
      }

      vertexOffset += GLuint(mesh.verts.size());
    }
    return out;
  }

  void ContextTriangle::PIMPL::Draw(ObjectHandle a_handle)
  {
    uint32_t type = ObjectType(a_handle);
    if (type != gContextTriangleID)
    {
      throw excptTypeError();
    }

    uint32_t id = ObjectID(a_handle);

    GLuint loc_color = glGetUniformLocation(m_shaderProgram, "u_color");
    glUniform4fv(loc_color, 1, m_currentObjects[id].color.GetData());

    if (m_currentObjects[id].depthEnabled)
    {
      glEnable(GL_DEPTH_TEST);
    }
    else
    {
      glDisable(GL_DEPTH_TEST);
    }

    glDrawElements(GL_TRIANGLES, 
                   m_currentObjects[id].count, 
                   GL_UNSIGNED_INT,
                   (const void *)m_currentObjects[id].offset);
  }

  void ContextTriangle::PIMPL::Clear()
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &m_vertexBuffer);
    m_vertexBuffer = 0;
    glDeleteBuffers(1, &m_indexBuffer);
    m_indexBuffer = 0;

    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;

    m_currentObjects.clear();
  }

  void ContextTriangle::PIMPL::SetMatrix(Dg::R3::Matrix<float> const & a_mat)
  {
    GLuint mvp = glGetUniformLocation(m_shaderProgram, "u_mvp");
    if (mvp == -1)
    {
      Logger::Log("Failed to find mv_matrix in shader program", Dg::LL_Error);
    }
    glUniformMatrix4fv(mvp, 1, GL_FALSE, a_mat.GetData());
  }

  void ContextTriangle::PIMPL::SetColor(ObjectHandle a_handle, Dg::R3::Vector<float> const & a_clr)
  {
    uint32_t type = ObjectType(a_handle);
    if (type != gContextTriangleID)
    {
      throw excptTypeError();
    }

    uint32_t id = ObjectID(a_handle);
    m_currentObjects[id].color = a_clr;
  }

  void ContextTriangle::PIMPL::Bind()
  {
    m_savedState = GetGLState();

    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
  }

  void ContextTriangle::PIMPL::Unbind()
  {
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    SetGLState(m_savedState);
  }

  void ContextTriangle::PIMPL::InitShaderProgram()
  {
    std::string vs_str = std::string(
#include "Mod_Context_vs_Triangle.glsl"
    );

    std::string fs_str = std::string(
#include "Mod_Context_fs_Triangle.glsl"
    );

    m_shaderProgram = GetShaderProgram(vs_str, fs_str);

    //Init defaults
    glUseProgram(m_shaderProgram);
    GLuint loc_color = glGetUniformLocation(m_shaderProgram, "u_color");
    GLuint loc_mvp = glGetUniformLocation(m_shaderProgram, "u_mvp");
    Dg::R3::Vector<float> clr(1.0f, 1.0f, 1.0f, 1.0f);
    Dg::R3::Matrix<float> mat;
    glUniform4fv(loc_color, 1, clr.GetData());
    glUniformMatrix4fv(loc_mvp, 1, GL_FALSE, mat.GetData());
    glUseProgram(0);
  }

  ContextTriangle::ContextTriangle()
    : m_pimpl(new PIMPL())
  {

  }

  ContextTriangle::~ContextTriangle()
  {
    delete m_pimpl;
    m_pimpl = nullptr;
  }

  void ContextTriangle::ClearLoadList()
  {
    m_pimpl->ClearLoadList();
  }

  void ContextTriangle::CommitLoadList()
  {
    m_pimpl->CommitLoadList();
  }

  ObjectHandle ContextTriangle::AddObject(TriangleMesh const & a_obj)
  {
    return m_pimpl->AddObject(a_obj);
  }

  void ContextTriangle::Draw(ObjectHandle a_ref)
  {
    m_pimpl->Draw(a_ref);
  }

  void ContextTriangle::Clear()
  {
    m_pimpl->Clear();
  }

  void ContextTriangle::SetMatrix(Dg::R3::Matrix<float> const & a_mat)
  {
    m_pimpl->SetMatrix(a_mat);
  }

  void ContextTriangle::SetColor(ObjectHandle a_handle, Dg::R3::Vector<float> const & a_clr)
  {
    m_pimpl->SetColor(a_handle, a_clr);
  }

  void ContextTriangle::Bind()
  {
    m_pimpl->Bind();
  }

  void ContextTriangle::Unbind()
  {
    m_pimpl->Unbind();
  }
}