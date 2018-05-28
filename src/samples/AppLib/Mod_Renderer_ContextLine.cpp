
#include <vector>
#include <string>
#include <exception>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Renderer_Logger.h"
#include "DgTypes.h"
#include "Mod_Renderer_ContextLine.h"

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

  class ex_vsCompile: public excptBase
  {
  public:

    ex_vsCompile(std::string a_meta = "")
      : excptBase(a_meta)
    {}

    virtual const char* what() const throw()
    {
      std::string meta = GetMeta();
      std::string msg = "Vertex shader failed to compile for ContextLine";
      if (!meta.empty())
      {
        msg = msg + ": " + meta;
      }
      return msg.c_str();
    }
  };

  class ex_fsCompile: public excptBase
  {
  public:

    ex_fsCompile(std::string a_meta = "")
      : excptBase(a_meta)
    {}

    virtual const char* what() const throw()
    {
      std::string meta = GetMeta();
      std::string msg = "Fragment shader failed to compile for ContextLine";
      if (!meta.empty())
      {
        msg = msg + ": " + meta;
      }
      return msg.c_str();
    }
  };

  class ex_LinkFail: public excptBase
  {
  public:

    ex_LinkFail(std::string a_meta = "")
      : excptBase(a_meta)
    {}

    virtual const char* what() const throw()
    {
      std::string meta = GetMeta();
      std::string msg = "Shader program failed to link";
      if (!meta.empty())
      {
        msg = msg + ": " + meta;
      }
      return msg.c_str();
    }
  };

  class ContextLine::PIMPL
  {
  public:

    PIMPL();
    ~PIMPL();

    void LoadData(std::vector<LineMesh> const &);
    void Draw(int);
    void Clear();
    void SetMatrix(Dg::R3::Matrix<float> const &);
    void TurnOnContext();
    void TurnOffContext();

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
    GLuint vPosition = glGetAttribLocation(m_shaderProgram, "position");

    //TODO Fix 5th argument; stride
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
  }

  //Also sets m_currentLines;
  void ContextLine::PIMPL::CollateData(std::vector<LineMesh> const & a_lineMeshs, GPU_Data & a_out)
  {
    GLuint vertexOffset = 0;
    a_out.indexData.clear();
    a_out.vertexData.clear();

    for (auto const & lineMesh : a_lineMeshs)
    {
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

      LineData lineData;
      lineData.offset = vertexOffset;
      lineData.count = GLsizei(lineMesh.lines.size() * 2);
      m_currentLines.push_back(lineData);

      vertexOffset += GLuint(lineMesh.verts.size());
    }
  }

  void ContextLine::PIMPL::Draw(int a_index)
  {
    glDrawArrays(GL_LINES, m_currentLines[a_index].offset, m_currentLines[a_index].count);
  }

  void ContextLine::PIMPL::Clear()
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    GLuint mv_matrix = glGetAttribLocation(m_shaderProgram, "mv_matrix");
    if (mv_matrix == 0)
    {
      Logger::Log("Failed to find mv_matrix in shader program", Dg::LL_Error);
    }
    glUniformMatrix3fv(mv_matrix, 1, GL_FALSE, a_mat.GetData());
  }

  void ContextLine::PIMPL::TurnOnContext()
  {
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
  }

  void ContextLine::PIMPL::TurnOffContext()
  {
    glUseProgram(0);
    glBindVertexArray(0);
  }

  void ContextLine::PIMPL::InitShaderProgram()
  {
    //Create shader ID
    GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vs_str = std::string(
#include "Mod_Renderer_vs_line.glsl"
    );

    std::string fs_str = std::string(
#include "Mod_Renderer_fs_line.glsl"
    );

    //Set shader source
    const GLchar* pvs_str = vs_str.c_str();
    glShaderSource(vsID, 1, (const GLchar **)&pvs_str, nullptr);

    const GLchar* pfs_str = fs_str.c_str();
    glShaderSource(fsID, 1, (const GLchar **)&pfs_str, nullptr);

    //Compile shader source
    glCompileShader(vsID);
    glCompileShader(fsID);

    //Check shader for errors
    GLint vsCompiled = GL_FALSE;
    GLint fsCompiled = GL_FALSE;
    glGetShaderiv(vsID, GL_COMPILE_STATUS, &vsCompiled);
    glGetShaderiv(fsID, GL_COMPILE_STATUS, &fsCompiled);
    if (vsCompiled != GL_TRUE)
    {
      throw ex_vsCompile();
    }
    if (fsCompiled != GL_TRUE)
    {
      throw ex_fsCompile();
    }

    //Create program, attach shaders to it and link it.
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vsID);
    glAttachShader(m_shaderProgram, fsID);
    glLinkProgram(m_shaderProgram);

    GLint linkStatus = GL_TRUE;
    glGetProgramiv( m_shaderProgram, GL_LINK_STATUS, &linkStatus);
    if ( linkStatus == GL_FALSE )
    {
      GLint logLen;
      glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &logLen);
      std::vector<char> msgSrc(logLen + 1);
      GLsizei written;
      glGetProgramInfoLog( m_shaderProgram, logLen, &written, msgSrc.data());
      std::string message(msgSrc.begin(), msgSrc.end());
      throw ex_LinkFail(message);
    }

    //Detach and delete shaders afer we have successfully linked the program.
    glDetachShader(m_shaderProgram, vsID);
    glDetachShader(m_shaderProgram, fsID);
    glDeleteShader(vsID);
    glDeleteShader(fsID);
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

  void ContextLine::TurnOnContext()
  {
    m_pimpl->TurnOnContext();
  }

  void ContextLine::TurnOffContext()
  {
    m_pimpl->TurnOffContext();
  }
}