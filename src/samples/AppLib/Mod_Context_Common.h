#ifndef MOD_RENDERER_COMMON_H
#define MOD_RENDERER_COMMON_H

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Context.h"

namespace Context
{
  GLuint GetShaderProgram(std::string const & a_fs, std::string const & a_vs);

  //Object Handle makeup: 
  //
  // |----------ID------------| |-type-|
  // 00000000 00000000 00000000 00000000
  //
  uint32_t ObjectType(ObjectHandle);
  uint32_t ObjectID(ObjectHandle);
  ObjectHandle GetObjectHandle(uint32_t type, uint32_t id);

  uint32_t const gContextLineID = 1;
  uint32_t const gContextTriangleID = 1;

  struct GL_State
  {
    GLint program;
    GLint texture;
    GLint active_texture;
    GLint array_buffer;
    GLint element_array_buffer;
    GLint vertex_array;
    GLint blend_src;
    GLint blend_dst;
    GLint blend_equation_rgb;
    GLint blend_equation_alpha;
    GLint viewport[4];
    GLboolean enable_blend;
    GLboolean enable_cull_face;
    GLboolean enable_depth_test;
    GLboolean enable_scissor_test;
  };

  GL_State GetGLState();
  void SetGLState(GL_State const &);
}

#endif