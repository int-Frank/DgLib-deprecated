
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Window_Window_Viewport.h"
#include "Mod_Renderer_Common.h"

namespace Window
{
  Window_Viewport::Window_Viewport(InitData const & a_data)
    : Window(a_data)
    , m_shaderProgram(0)
    , m_vao(0)
    , m_vbo(0)
  {
    std::string vs(
#include "Mod_Window_vs.glsl"
    );

    std::string fs(
#include "Mod_Window_fs.glsl"
    );

    m_shaderProgram = Renderer::GetShaderProgram(vs, fs);

    float vertData[24] = 
    {
      -1.0f,  1.0f, //position
       0.0f,  1.0f, //uv

       1.0f,  1.0f,
       1.0f,  1.0f,

       1.0f, -1.0f,
       1.0f,  0.0f,

      -1.0f,  1.0f,
       0.0f,  1.0f,

      -1.0f, -1.0f,
       0.0f,  0.0f,

       1.0f, -1.0f,
       1.0f,  0.0f,
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_STATIC_DRAW);

    GLuint loc_in_position = glGetAttribLocation(m_shaderProgram, "in_position");
    GLuint loc_in_texCoords = glGetAttribLocation(m_shaderProgram, "in_texCoords");

    glVertexAttribPointer(loc_in_position, 2, GL_FLOAT, GL_FALSE
      , 4 * sizeof(GL_FLOAT), 0);
    glVertexAttribPointer(loc_in_texCoords, 2, GL_FLOAT, GL_FALSE
      , 4 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));

    glEnableVertexAttribArray(loc_in_position);
    glEnableVertexAttribArray(loc_in_texCoords);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void Window_Viewport::Update()
  {
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, GetTexture());
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(0);
    glBindVertexArray(0);
  }

  Window_Viewport::~Window_Viewport()
  {
    glDeleteProgram(m_shaderProgram);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
  }
}