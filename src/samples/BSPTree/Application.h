#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <map>
#include <stack>

#include "imgui.h"
#include "game_tools/DgGTBSPTree.h"
#include "Types.h"


class Application
{
public:
  Application();

  ~Application() {}

  Application(const Application&);
  Application& operator= (const Application&);

  void Run(Application*);

  bool LoadProject(std::string const &);
  void ClearProject();

  static Application * GetInstance() { return s_app; }
private:

  static Application* s_app;

private:

  //UI only touches this data
  AppState                  m_appState;

  int const                 m_canvasWidth = 1024;
  int const                 m_canvasHeight = 768;
  int const                 m_editorWidth = 200;
  int const                 m_editorHeight = m_canvasHeight;
  int const                 m_bufferWidth = 3;
  int const                 m_topMenuBuffer = 5;
  int const                 m_windowWidth = m_editorWidth + m_canvasWidth + m_bufferWidth * 3;
  int const                 m_windowHeight = m_canvasHeight + m_topMenuBuffer + m_bufferWidth * 2;

  AABB                      m_canvasBounds;

  std::string const         m_configFileName = "config.ini";
  std::string const         m_projectPath = "./maps/";
  std::string const         m_fileExt = "cp";

  AppInfo		                m_info;
  GLFWwindow*               m_window;

  double                    m_mouseScroll;

  BSPTree                   m_bspTree;
  std::map<int, Polygon>    m_polygons;
  Matrix                    m_T_model_screen;

  bool                      m_shouldQuit;

private:
  //Main initializer function. All others are called through here.
  bool Init();

  void GetConfiguration();
  bool InitGL();
  void CreateFileList(std::vector<std::string> const &
                    , char const *
                    , int *);
  void UpdateProjectTitle(std::string const &);

  bool ParseFile(std::string const &,
                 BSPTree::DataInput &);
  void SavePolygons(BSPTree::DataInput const &);
  void SetModelToScreenTransform(BSPTree::DataInput const &);

  void GetCanvasBounds();
  void DrawScene();
  void DrawPolygonEdges();
  void DrawPolygons();

  void DrawSegment(Segment const &, ImColor const &);
  void DrawFilledPolygon(Polygon const &, ImColor const &);
  void DrawAABB(AABB const &, ImColor const &);

  std::vector<std::string> GetProjects();

  void Path_GoChildLess();
  void Path_GoChildGreater();
  void Path_GoParent();
  void Path_GoRoot();

  void Shutdown();

private:

  //The windows stack allows us to work around ImGui's issue of stacking modals
  //called from menu items.
  enum Modal
  {
    SavePrompt,
    OpenWindow,
    NewProjectRequest,
    ViewHelp,
    ViewAbout,
    None
  };

  std::stack<int>          m_windowStack;

  void InitGUI();
  void ShowGUI();
  void ShowGUI_Editor();
  void ShowGUI_Canvas();
};

#endif