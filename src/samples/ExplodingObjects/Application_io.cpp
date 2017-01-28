
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>

#include "Application.h"
#include "DgStringFunctions.h"
#include "Mesh.h"

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

  Mesh mesh;
  if (!mesh.LoadOBJ(a_file))
  {
    printf("Failed to open file '%s'!\n", a_file.c_str());
    return false;
  }

  printf("'%s' loaded!\n", a_file.c_str());
  UpdateProjectTitle(a_file);

  m_renderer.SetMesh(mesh);

  size_t id = 0;
  for (auto it = mesh.Faces().cbegin();
       it != mesh.Faces().cend();
       ++it, ++id)
  {
    triangle tri(mesh.Points()[(*it)[0]], 
                 mesh.Points()[(*it)[1]], 
                 mesh.Points()[(*it)[2]]);

    m_centroids.push_back(tri.Centroid());
  }

  return true;
}