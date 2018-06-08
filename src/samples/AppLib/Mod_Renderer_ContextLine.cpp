
#include <vector>
#include <string>
#include <exception>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Renderer_Logger.h"
#include "DgTypes.h"
#include "Mod_Renderer_ContextLine.h"
#include "Mod_Renderer_Common.h"

namespace Renderer
{
  class ContextLine::PIMPL
  {
  public:

    PIMPL();
    ~PIMPL();

    void LoadData(std::vector<LineMesh> const &);
    void Draw(int);
    void Clear();
    void SetMatrix(Dg::R3::Matrix<float> const &);
    void SetColor(Dg::R3::Vector<float> const &);
    void Bind();
    void Unbind();

  private:

    struct GPU_LineVertex
    {
      float vec4[4];
    };

    struct GPU_Data
    {
      std::vector<GPU_LineVertex> vertexData;
      std::vector<GLuint>         indexData;
    };

    struct LineData
    {
      GLint   offset;
      GLsizei count;
    };

  private:

    //Also sets m_currentLines;
    void CollateData(std::vector<LineMesh> const &, GPU_Data & out);
    void InitShaderProgram();

  private:

    std::vector<LineData>     m_currentLines;

    GLuint m_shaderProgram;
    GLuint m_vao;
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
  };

  ContextLine::PIMPL::PIMPL()
    : m_shaderProgram(0)
    , m_vao(0)
    , m_vertexBuffer(0)
    , m_indexBuffer(0)
  {
    InitShaderProgram();
  }

  ContextLine::PIMPL::~PIMPL()
  {
    Clear();
    glDeleteProgram(m_shaderProgram);
  }

  void ContextLine::PIMPL::LoadData(std::vector<LineMesh> const & a_data)
  {
    Clear();

    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_indexBuffer);

    GPU_Data gpuData;
    
    CollateData(a_data, gpuData);
    
    //Vertices
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 
      gpuData.vertexData.size() * sizeof(GPU_LineVertex), 
      gpuData.vertexData.data(), GL_STATIC_DRAW);
    
    int stride = sizeof(GPU_LineVertex);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
      gpuData.indexData.size() * sizeof(GLuint),
      gpuData.indexData.data(), GL_STATIC_DRAW);
    GLuint loc_position = glGetAttribLocation(m_shaderProgram, "in_position");

    //TODO Fix 5th argument; stride
    glVertexAttribPointer(loc_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(loc_position);
  }

  //Also sets m_currentLines;
  void ContextLine::PIMPL::CollateData(std::vector<LineMesh> const & a_lineMeshs, GPU_Data & a_out)
  {
    GLuint vertexOffset = 0;
    GLuint indexOffset = 0;
    a_out.indexData.clear();
    a_out.vertexData.clear();

    for (auto const & lineMesh : a_lineMeshs)
    {
      LineData lineData;
      lineData.offset = a_out.indexData.size() * sizeof(GLuint);
      lineData.count = GLsizei(lineMesh.lines.size() * 2);
      m_currentLines.push_back(lineData);

      for (auto vert : lineMesh.verts)
      {
        GPU_LineVertex v;
        v.vec4[0] = vert.point[0];
        v.vec4[1] = vert.point[1];
        v.vec4[2] = vert.point[2];
        v.vec4[3] = 1.0f;

        a_out.vertexData.push_back(v);
      }

      for (auto line : lineMesh.lines)
      {
        a_out.indexData.push_back(line.indices[0] + vertexOffset);
        a_out.indexData.push_back(line.indices[1] + vertexOffset);
      }

      vertexOffset += GLuint(lineMesh.verts.size());
    }
  }

  void ContextLine::PIMPL::Draw(int a_index)
  {
    glDrawElements(GL_LINES, 
                   m_currentLines[a_index].count, 
                   GL_UNSIGNED_INT,
                   (const void *)m_currentLines[a_index].offset);
  }

  void ContextLine::PIMPL::Clear()
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

    m_currentLines.clear();
  }

  void ContextLine::PIMPL::SetMatrix(Dg::R3::Matrix<float> const & a_mat)
  {
    GLuint mvp = glGetUniformLocation(m_shaderProgram, "u_mvp");
    if (mvp == -1)
    {
      Logger::Log("Failed to find mv_matrix in shader program", Dg::LL_Error);
    }
    glUniformMatrix4fv(mvp, 1, GL_FALSE, a_mat.GetData());
  }

  void ContextLine::PIMPL::SetColor(Dg::R3::Vector<float> const & a_clr)
  {
      GLuint loc_color = glGetUniformLocation(m_shaderProgram, "u_color");
      glUniform4fv(loc_color, 1, a_clr.GetData());
  }

  void ContextLine::PIMPL::Bind()
  {
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
  }

  void ContextLine::PIMPL::Unbind()
  {
    glUseProgram(0);
    glBindVertexArray(0);
  }

  void ContextLine::PIMPL::InitShaderProgram()
  {
    std::string vs_str = std::string(
#include "Mod_Renderer_vs_line.glsl"
    );

    std::string fs_str = std::string(
#include "Mod_Renderer_fs_line.glsl"
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

  ContextLine::ContextLine()
    : m_pimpl(new PIMPL())
  {

  }

  ContextLine::~ContextLine()
  {
    delete m_pimpl;
    m_pimpl = nullptr;
  }

  void ContextLine::LoadData(std::vector<LineMesh> const & a_data)
  {
    m_pimpl->LoadData(a_data);
  }

  void ContextLine::Draw(int a_ref)
  {
    m_pimpl->Draw(a_ref);
  }

  void ContextLine::Clear()
  {
    m_pimpl->Clear();
  }

  void ContextLine::SetMatrix(Dg::R3::Matrix<float> const & a_mat)
  {
    m_pimpl->SetMatrix(a_mat);
  }

  void ContextLine::SetColor(Dg::R3::Vector<float> const & a_clr)
  {
    m_pimpl->SetColor(a_clr);
  }

  void ContextLine::Bind()
  {
    m_pimpl->Bind();
  }

  void ContextLine::Unbind()
  {
    m_pimpl->Unbind();
  }
}