
#include <vector>
#include <string>
#include <exception>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Context_Logger.h"
#include "DgTypes.h"
#include "Mod_Context_Line.h"
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

  class ContextLine::PIMPL
  {
  public:

    PIMPL();
    ~PIMPL();

    void ClearLoadList();
    ObjectHandle AddObject(LineMesh const &);
    void CommitLoadList();

    void Bind();
    void Draw(ObjectHandle);
    void Unbind();

    void Clear();
    void SetMatrix(Dg::R3::Matrix<float> const &);
    void SetColor(ObjectHandle, Dg::R3::Vector<float> const &);
    void SetThickness(ObjectHandle, float);

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
      GLint                 offset;
      GLsizei               count;
      Dg::R3::Vector<float> color;
      float                 thickness;
      bool                  depthEnabled;
    };

  private:

    //Also sets m_currentLines;
    GPU_Data CollateData();
    void InitShaderProgram();
    void LoadData();

  private:

    GL_State m_savedState;

    std::vector<LineData>     m_currentLines;
    std::vector<LineMesh>     m_loadList;

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

  void ContextLine::PIMPL::ClearLoadList()
  {
    m_loadList.clear();
  }

  void ContextLine::PIMPL::CommitLoadList()
  {
    Clear();
    LoadData();
    ClearLoadList();
  }

  ObjectHandle ContextLine::PIMPL::AddObject(LineMesh const & a_obj)
  {
    ObjectHandle handle = GetObjectHandle(gContextLineID, (uint32_t)m_loadList.size());
    m_loadList.push_back(a_obj);
    return handle;
  }

  void ContextLine::PIMPL::LoadData()
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

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  //Also sets m_currentLines;
  ContextLine::PIMPL::GPU_Data ContextLine::PIMPL::CollateData()
  {
    GPU_Data out;

    GLuint vertexOffset = 0;
    GLuint indexOffset = 0;
    out.indexData.clear();
    out.vertexData.clear();

    for (auto const & lineMesh : m_loadList)
    {
      LineData lineData;
      lineData.offset = (GLuint)out.indexData.size() * sizeof(GLuint);
      lineData.count = GLsizei(lineMesh.lines.size() * 2);
      lineData.depthEnabled = lineMesh.depthEnabled;
      lineData.color = lineMesh.color;
      lineData.thickness = lineMesh.thickness;

      m_currentLines.push_back(lineData);

      for (auto vert : lineMesh.verts)
      {
        GPU_LineVertex v;
        v.vec4[0] = vert.point[0];
        v.vec4[1] = vert.point[1];
        v.vec4[2] = vert.point[2];
        v.vec4[3] = 1.0f;

        out.vertexData.push_back(v);
      }

      for (auto line : lineMesh.lines)
      {
        out.indexData.push_back(line.indices[0] + vertexOffset);
        out.indexData.push_back(line.indices[1] + vertexOffset);
      }

      vertexOffset += GLuint(lineMesh.verts.size());
    }
    return out;
  }

  void ContextLine::PIMPL::Draw(ObjectHandle a_handle)
  {
    uint32_t type = ObjectType(a_handle);
    if (type != gContextLineID)
    {
      throw excptTypeError();
    }

    uint32_t id = ObjectID(a_handle);

    GLuint loc_color = glGetUniformLocation(m_shaderProgram, "u_color");
    glUniform4fv(loc_color, 1, m_currentLines[id].color.GetData());

    glLineWidth(m_currentLines[id].thickness);

    if (m_currentLines[id].depthEnabled)
    {
      glEnable(GL_DEPTH_TEST);
    }
    else
    {
      glDisable(GL_DEPTH_TEST);
    }

    glDrawElements(GL_LINES, 
                   m_currentLines[id].count, 
                   GL_UNSIGNED_INT,
                   (const void *)m_currentLines[id].offset);
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
    GLint prog;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    glUseProgram(m_shaderProgram);
    GLuint mvp = glGetUniformLocation(m_shaderProgram, "u_mvp");
    if (mvp == -1)
    {
      Logger::Log("Failed to find mv_matrix in shader program", Dg::LL_Error);
    }
    glUniformMatrix4fv(mvp, 1, GL_FALSE, a_mat.GetData());
    glUseProgram(prog);
  }

  void ContextLine::PIMPL::SetColor(ObjectHandle a_handle, Dg::R3::Vector<float> const & a_clr)
  {
    uint32_t type = ObjectType(a_handle);
    if (type != gContextLineID)
    {
      throw excptTypeError();
    }

    uint32_t id = ObjectID(a_handle);
    m_currentLines[id].color = a_clr;
  }

  void ContextLine::PIMPL::SetThickness(ObjectHandle a_handle, float a_val)
  {
    uint32_t type = ObjectType(a_handle);
    if (type != gContextLineID)
    {
      throw excptTypeError();
    }

    uint32_t id = ObjectID(a_handle);
    m_currentLines[id].thickness = a_val;
  }

  void ContextLine::PIMPL::Bind()
  {
    m_savedState = GetGLState();

    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
  }

  void ContextLine::PIMPL::Unbind()
  {
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    SetGLState(m_savedState);
  }

  void ContextLine::PIMPL::InitShaderProgram()
  {
    std::string vs_str = std::string(
#include "Mod_Context_vs_Line.glsl"
    );

    std::string fs_str = std::string(
#include "Mod_Context_fs_Line.glsl"
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

  void ContextLine::ClearLoadList()
  {
    m_pimpl->ClearLoadList();
  }

  void ContextLine::CommitLoadList()
  {
    m_pimpl->CommitLoadList();
  }

  ObjectHandle ContextLine::AddObject(LineMesh const & a_obj)
  {
    return m_pimpl->AddObject(a_obj);
  }

  void ContextLine::Draw(ObjectHandle a_ref)
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

  void ContextLine::SetColor(ObjectHandle a_handle, Dg::R3::Vector<float> const & a_clr)
  {
    m_pimpl->SetColor(a_handle, a_clr);
  }

  void ContextLine::SetThickness(ObjectHandle a_handle, float a_val)
  {
    m_pimpl->SetThickness(a_handle, a_val);
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