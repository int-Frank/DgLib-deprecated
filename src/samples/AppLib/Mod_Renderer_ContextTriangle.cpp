
#include <vector>
#include <string>
#include <exception>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Renderer_Logger.h"
#include "DgTypes.h"
#include "Mod_Renderer_ContextTriangle.h"
#include "Mod_Renderer_Common.h"

namespace Renderer
{
  class ContextTriangle::PIMPL
  {
  public:

    PIMPL();
    ~PIMPL();

    void LoadData(std::vector<TriangleMesh> const &);
    void Draw(int);
    void Clear();
    void SetMatrix(Dg::R3::Matrix<float> const &);
    void SetColor(Dg::R3::Vector<float> const &);
    void Bind();
    void Unbind();

  private:

    struct GPU_MeshVertex
    {
      float point[4];
      float normal[4];
      float uv[2];
    };

    struct GPU_Data
    {
      std::vector<GPU_MeshVertex> vertexData;
      std::vector<GLuint>         indexData;
    };

    struct MeshData
    {
      GLint   offset;
      GLsizei count;
    };

  private:

    //Also sets m_currentObjects;
    void CollateData(std::vector<TriangleMesh> const &, GPU_Data & out);
    void InitShaderProgram();

  private:

    std::vector<MeshData>     m_currentObjects;

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

  void ContextTriangle::PIMPL::LoadData(std::vector<TriangleMesh> const & a_data)
  {
    Clear();

    if (a_data.empty())
    {
      return;
    }

    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_indexBuffer);

    GPU_Data gpuData;

    CollateData(a_data, gpuData);

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
    GLuint loc_normal   = glGetAttribLocation(m_shaderProgram, "in_normal");
    GLuint loc_uv       = glGetAttribLocation(m_shaderProgram, "in_uv");

    int stride = sizeof(GPU_MeshVertex);

    glVertexAttribPointer(loc_position, 4, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribPointer(loc_normal, 4, GL_FLOAT, GL_FALSE, stride
      , (void*)sizeof(GPU_MeshVertex::point));
    glVertexAttribPointer(loc_uv, 4, GL_FLOAT, GL_FALSE, stride
      , (void*)(sizeof(GPU_MeshVertex::point) + sizeof(GPU_MeshVertex::normal)));

    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_normal);
    glEnableVertexAttribArray(loc_uv);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  //Also sets m_currentObjects;
  void ContextTriangle::PIMPL::CollateData(std::vector<TriangleMesh> const & a_meshList, GPU_Data & a_out)
  {
    GLuint vertexOffset = 0;
    GLuint indexOffset = 0;
    a_out.indexData.clear();
    a_out.vertexData.clear();

    for (auto const & mesh : a_meshList)
    {
      MeshData meshData;
      meshData.offset = a_out.indexData.size() * sizeof(GLuint);
      meshData.count = GLsizei(mesh.triangles.size() * 3);
      m_currentObjects.push_back(meshData);

      for (auto vert : mesh.verts)
      {
        GPU_MeshVertex v;
        v.point[0] = vert.point[0];
        v.point[1] = vert.point[1];
        v.point[2] = vert.point[2];
        v.point[3] = 1.0f;

        v.normal[0] = vert.normal[0];
        v.normal[1] = vert.normal[1];
        v.normal[2] = vert.normal[2];
        v.normal[3] = 0.0f;

        v.uv[0] = vert.uv[0];
        v.uv[1] = vert.uv[1];

        a_out.vertexData.push_back(v);
      }

      for (auto tri : mesh.triangles)
      {
        a_out.indexData.push_back(tri.indices[0] + vertexOffset);
        a_out.indexData.push_back(tri.indices[1] + vertexOffset);
        a_out.indexData.push_back(tri.indices[2] + vertexOffset);
      }

      vertexOffset += GLuint(mesh.verts.size());
    }
  }

  void ContextTriangle::PIMPL::Draw(int a_index)
  {
    glDrawElements(GL_TRIANGLES, 
                   m_currentObjects[a_index].count, 
                   GL_UNSIGNED_INT,
                   (const void *)m_currentObjects[a_index].offset);
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

  void ContextTriangle::PIMPL::SetColor(Dg::R3::Vector<float> const & a_clr)
  {
    GLuint loc_color = glGetUniformLocation(m_shaderProgram, "u_color");
    glUniform4fv(loc_color, 1, a_clr.GetData());
  }

  void ContextTriangle::PIMPL::Bind()
  {
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
  }

  void ContextTriangle::PIMPL::InitShaderProgram()
  {
    std::string vs_str = std::string(
#include "Mod_Renderer_vs_triangle.glsl"
    );

    std::string fs_str = std::string(
#include "Mod_Renderer_fs_triangle.glsl"
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

  void ContextTriangle::LoadData(std::vector<TriangleMesh> const & a_data)
  {
    m_pimpl->LoadData(a_data);
  }

  void ContextTriangle::Draw(int a_ref)
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

  void ContextTriangle::SetColor(Dg::R3::Vector<float> const & a_clr)
  {
    m_pimpl->SetColor(a_clr);
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