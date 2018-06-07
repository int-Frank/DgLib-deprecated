#ifndef MOD_RENDERER_COMMON_H
#define MOD_RENDERER_COMMON_H

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Renderer
{
  GLuint GetShaderProgram(std::string const & a_fs, std::string const & a_vs);
}

#endif