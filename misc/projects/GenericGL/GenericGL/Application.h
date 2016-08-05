#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <stack>

#include "Renderer.h"
#include "DgIDManager.h"
#include "EventManager.h"

class Application
{
public:
  Application();

  ~Application() {}

  Application(const Application&);
  Application& operator= (const Application&);

  void Run(Application*);

  void PushEvent(Event const &);

  bool LoadProject(std::string const &);
  bool SaveProject(std::string const &);
  void UpdateScroll(double);
  void KeyEvent(int key, int action);
  void ClearProject();
  void NewProject();

  static Application * GetInstance() { return s_app; }
private:

  static Application* s_app;

private:

  //UI only touches this data
  AppData                   m_appData;

  EventManager              m_eventManager;

  std::string const         m_configFileName = "config.ini";
  std::string const         m_projectPath = "./projects/";
  std::string const         m_fileExt = "txt";

  AppInfo		                m_info;
  GLFWwindow*               m_window;
  Renderer                  m_renderer;

  double                    m_mouseScroll;

  Dg::IDManager<int>        m_IDManager;

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

  void UI_NewFrame();
  void HandleEvents();
  void DoLogic(double dt);
  void Render();

  std::vector<std::string> GetProjects();

  void Shutdown();

private:

  //The windows stack allows us to work around ImGui's issue of stacking modals
  //called from menu items.
  enum Modal
  {
    SavePrompt,
    OverwriteWindow,
    SaveAsWindow,
    OpenWindow,
    NewProjectRequest,
    ViewHelp,
    ViewAbout,
    None
  };

  std::stack<int>          m_windowStack;

  void ShowMainGUIWindow();
};

#endif