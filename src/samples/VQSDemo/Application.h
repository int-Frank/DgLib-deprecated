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
#include "Model.h"

class Application
{
public:
  Application();

  ~Application() {}

  Application(const Application&);
  Application& operator= (const Application&);

  void Run(Application*);

  void PushEvent(Event const &);

  void UpdateScroll(double);
  void KeyEvent(int key, int action);

  static Application * GetInstance() { return s_app; }
private:

  static Application* s_app;

private:

  //UI only touches this data
  AppData                   m_appData;

  EventManager              m_eventManager;

  std::string const         m_configFileName = "config.ini";

  AppInfo		                m_info;
  GLFWwindow*               m_window;
  Renderer                  m_renderer;

  double                    m_mouseSpeed;
  double                    m_mouseCurrentX;
  double                    m_mouseCurrentY;
  double                    m_mousePrevX;
  double                    m_mousePrevY;

  double                    m_camRotX;
  double                    m_camRotZ;

  double                    m_camZoom;
  double                    m_camZoomTarget;
  double                    m_maxCamZoomOut;

  bool                      m_camCanRotate;

  ArmSkeleton               m_model;

  bool                      m_shouldQuit;

private:
  //Main initializer function. All others are called through here.
  bool Init();

  void ResetCamera();
  void GetConfiguration();
  bool InitGL();

  void UI_NewFrame();
  void HandleEvents();
  void DoLogic(double dt);
  void Render();

  void Shutdown();

private:

  //The windows stack allows us to work around ImGui's issue of stacking modals
  //called from menu items.
  enum Modal
  {
    ViewHelp,
    ViewAbout,
    None
  };

  std::stack<int>          m_windowStack;

  void ShowMainGUIWindow();
};

#endif