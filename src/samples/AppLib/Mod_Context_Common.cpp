
#include <vector>
#include <exception>

#include "Mod_Context_Common.h"

namespace Context
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
      GLint maxLength = 0;
      glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &maxLength);
      std::vector<GLchar> errorLog(maxLength);
      glGetShaderInfoLog(vsID, maxLength, &maxLength, &errorLog[0]);

      std::string reason(errorLog.begin(), errorLog.end());
      throw ex_vsCompile(reason);
    }
    if (fsCompiled != GL_TRUE)
    {
      GLint maxLength = 0;
      glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &maxLength);
      std::vector<GLchar> errorLog(maxLength);
      glGetShaderInfoLog(fsID, maxLength, &maxLength, &errorLog[0]);

      std::string reason(errorLog.begin(), errorLog.end());
      throw ex_fsCompile(reason);
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

  uint32_t ObjectType(ObjectHandle a_handle)
  {
    return a_handle & 0xFF;
  }

  uint32_t ObjectID(ObjectHandle a_handle)
  {
    return a_handle >> 8;
  }

  ObjectHandle GetObjectHandle(uint32_t type, uint32_t id)
  {
    return (id << 8) | (type & 0xFF);
  }

  GL_State GetGLState()
  {
    GL_State data;

    glGetIntegerv(GL_CURRENT_PROGRAM, &data.program);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &data.texture);
    glGetIntegerv(GL_ACTIVE_TEXTURE, &data.active_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &data.array_buffer);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &data.element_array_buffer);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &data.vertex_array);
    glGetIntegerv(GL_BLEND_SRC, &data.blend_src);
    glGetIntegerv(GL_BLEND_DST, &data.blend_dst);
    glGetIntegerv(GL_BLEND_EQUATION_RGB, &data.blend_equation_rgb);
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &data.blend_equation_alpha);
    glGetIntegerv(GL_VIEWPORT, data.viewport);
    data.enable_blend = glIsEnabled(GL_BLEND);
    data.enable_cull_face = glIsEnabled(GL_CULL_FACE);
    data.enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    data.enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

    return data;
  }

  void SetGLState(GL_State const & a_data)
  {
    glUseProgram(a_data.program);
    glActiveTexture(a_data.active_texture);
    glBindTexture(GL_TEXTURE_2D, a_data.texture);
    glBindVertexArray(a_data.vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, a_data.array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_data.element_array_buffer);
    glBlendEquationSeparate(a_data.blend_equation_rgb, a_data.blend_equation_alpha);
    glBlendFunc(a_data.blend_src, a_data.blend_dst);
    if (a_data.enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    if (a_data.enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (a_data.enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (a_data.enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    glViewport(a_data.viewport[0], a_data.viewport[1], (GLsizei)a_data.viewport[2], (GLsizei)a_data.viewport[3]);
  }
}
