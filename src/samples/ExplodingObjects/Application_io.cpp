
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>

#include "Application.h"
#include "DgStringFunctions.h"

#define ARRAY_SIZE(arr) (int)((sizeof(arr))/(sizeof(*arr)))

void Application::UpdateProjectTitle(std::string const & a_name)
{
  char pFile[32];
  _splitpath_s(
               a_name.c_str()
               , nullptr
               , 0
               , nullptr
               , 0
               , pFile
               , 32
               , nullptr
               , 0
  );

  m_appData.projName = std::string(pFile);
}

bool Application::LoadProject(std::string const & a_file)
{
  ClearProject();

  Mesh newModel;
  if (!newModel.Load(a_file))
  {
    printf("Failed to open file '%s'!\n", a_file.c_str());
    return false;
  }

  printf("'%s' loaded!\n", a_file.c_str());
  UpdateProjectTitle(a_file);

  newModel.NormalizeData(vec4::Origin(), 0.5f);

  m_models.push_back(newModel);
  m_renderer.SetModels(m_models);

  SceneObject so;
  so.SetModelReference(0);
  m_sceneObjects.push_back(so);

  return true;
}

template<typename T>
static std::string ToString(T const * a_data, int a_size)
{
  std::stringstream ss;
  for (int i = 0; i < a_size; ++i)
  {
    ss << a_data[i];
    if (i != a_size - 1) ss << ',';
  }
  return ss.str();
}
