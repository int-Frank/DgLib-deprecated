#include "Mod_Renderer_Logger.h"
#include "Mod_Renderer_Context.h"
#include "DgTypes.h"

namespace Renderer
{
  void Context::LoadData(std::vector<LineMesh> const & a_data)
  {
    Logger::Log("You have tried to load data into a context which does not support this data type", Dg::LL_Warning);
  }

  void Context::LoadData(std::vector<TriangleMesh> const & a_data)
  {
    Logger::Log("You have tried to load data into a context which does not support this data type", Dg::LL_Warning);
  }
}