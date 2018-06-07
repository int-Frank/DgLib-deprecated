
#include <vector>
#include <exception>

#include "Mod_Renderer_Common.h"

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

  GLuint GetShaderProgram(std::string const & a_vs, std::string const & a_fs)
  {
    //Create shader ID
    GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

    //Set shader source
    const GLchar* pvs_str = a_vs.c_str();
    glShaderSource(vsID, 1, (const GLchar **)&pvs_str, nullptr);

    const GLchar* pfs_str = a_fs.c_str();
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
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vsID);
    glAttachShader(shaderProgram, fsID);
    glLinkProgram(shaderProgram);

    GLint linkStatus = GL_TRUE;
    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &linkStatus);
    if ( linkStatus == GL_FALSE )
    {
      GLint logLen;
      glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLen);
      std::vector<char> msgSrc(logLen + 1);
      GLsizei written;
      glGetProgramInfoLog( shaderProgram, logLen, &written, msgSrc.data());
      std::string message(msgSrc.begin(), msgSrc.end());
      throw ex_LinkFail(message);
    }

    //Detach and delete shaders afer we have successfully linked the program.
    glDetachShader(shaderProgram, vsID);
    glDetachShader(shaderProgram, fsID);
    glDeleteShader(vsID);
    glDeleteShader(fsID);

    return shaderProgram;
  }
}
