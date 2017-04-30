
#include <windows.h>
#include <shlwapi.h>
#include <time.h>
#pragma comment(lib,"shlwapi.lib")

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "DgApp.h"
#include "DgParser_INI.h"
#include "DgStringFunctions.h"
#include "EventManager.h"
#include "Event.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

class Event_MouseScroll : public Event
{
public:
  Event_MouseScroll()
    : Event()
    , m_val(0.0)
  {}

  ~Event_MouseScroll() {}

  Event_MouseScroll(Event_MouseScroll const & a_other)
    : Event(a_other)
    , m_val(a_other.m_val)
  {}

  Event_MouseScroll & operator=(Event_MouseScroll const & a_other)
  {
    Event::operator = (a_other);
    m_val = a_other.m_val;
  }

  void DoEvent()
  {
    DgApp::GetInstance()->UpdateScroll(m_val);
  }

  void SetOffset(double a_val) { m_val = a_val; }
  double GetOffset() const { return m_val; }

  Event_MouseScroll * Clone() const { return new Event_MouseScroll(*this); }

private:
  double    m_val;
};


class Event_KeyEvent : public Event
{
public:
  Event_KeyEvent()
    : Event()
    , m_key(-1)
    , m_action(-1)
  {}

  ~Event_KeyEvent() {}

  Event_KeyEvent(Event_KeyEvent const & a_other)
    : Event(a_other)
    , m_key(a_other.m_key)
    , m_action(a_other.m_action)
  {}

  Event_KeyEvent & operator=(Event_KeyEvent const & a_other)
  {
    Event::operator = (a_other);
    m_key = a_other.m_key;
    m_action = a_other.m_action;
  }

  void DoEvent()
  {
    DgApp::GetInstance()->KeyEvent(m_key, m_action);
  }

  void SetKey(int a_key) { m_key = a_key; }
  void SetAction(int a_action) { m_action = a_action; }
  int GetKey() const { return m_key; }
  int GetAction() const { return m_action; }

  Event_KeyEvent * Clone() const { return new Event_KeyEvent(*this); }

private:
  int       m_key;
  int       m_action;
};

class DgApp::PIMPL
{
public:

  PIMPL()
    : title("New Dg Application")
    , windowWidth(800)
    , windowHeight(600)
    , majorVersion(4)
    , minorVersion(3)
    , samples(0)
    , fullscreen(false)
    , configFileName("config.ini")
    , window(nullptr)
    , shouldQuit(false)
  {}


  static DgApp *      s_app;

  char                title[128];
  int                 windowWidth;
  int                 windowHeight;
  int                 majorVersion;
  int                 minorVersion;
  int                 samples;
  bool                fullscreen;

  std::string const   configFileName;

  EventManager        eventManager;

  GLFWwindow *        window;
  bool                shouldQuit;
};

DgApp * DgApp::PIMPL::s_app(nullptr);

DgApp::DgApp()
  : m_pimpl(new DgApp::PIMPL())
{
  m_pimpl->s_app = this;
}

DgApp::~DgApp()
{
  Shutdown();
  delete m_pimpl;
}

void DgApp::PushEvent(Event const & a_event)
{
  m_pimpl->eventManager.PushEvent(a_event);
}

void AppOnKeyEvent(int a_key, int a_action)
{
  Event_KeyEvent e;
  e.SetKey(a_key);
  e.SetAction(a_action);
  DgApp::GetInstance()->PushEvent(e);
}

void AppOnMouseScroll(double yOffset)
{
  Event_MouseScroll e;
  e.SetOffset(yOffset);
  DgApp::GetInstance()->PushEvent(e);
}

bool DgApp::FileExists(std::string const & a_name) const
{
  if (FILE *file = fopen(a_name.c_str(), "r"))
  {
    fclose(file);
    return true;
  }
  else
  {
    return false;
  }
}

std::vector<std::string> DgApp::GetFiles(std::string const & a_dirPath,
                                         std::string a_ext) const
{
  std::vector<std::string> result;
  WIN32_FIND_DATA ffd;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  std::string szDir = a_dirPath + "*";

  hFind = FindFirstFile(szDir.c_str(), &ffd);

  if (INVALID_HANDLE_VALUE == hFind)
  {
    return result;
  }

  do
  {
    if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
      size_t pos = PathFindExtension(ffd.cFileName) - &ffd.cFileName[0];
      std::string ext = (std::string(ffd.cFileName).substr(pos + 1));
      if (ext == a_ext || a_ext == "*")
      {
        result.push_back(ffd.cFileName);
      }
    }
  } while (FindNextFile(hFind, &ffd) != 0);

  FindClose(hFind);
  return result;
}


void DgApp::RUN(DgApp* the_app)
{
  m_pimpl->s_app = the_app;

  //Read configuration
  {
    Dg::Parser_INI parser;
    Dg::ErrorCode result = parser.Parse(m_pimpl->configFileName);

    if (result == Dg::ErrorCode::FailedToOpenFile)
    {
      fprintf(stderr, "Failed to open config file '%s'. Using defaults...\n", m_pimpl->configFileName.c_str());
      return;
    }
    else if (result != Dg::ErrorCode::None)
    {
      fprintf(stderr, "Failed trying to parse config file '%s'. Using defaults...\n", m_pimpl->configFileName.c_str());
      return;
    }

    for (int i = 0; i < parser.GetItems().size(); ++i)
    {
      if (parser.GetItems().query_key(i) == "windowWidth")
      {
        int val = 0;
        if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
        {
          m_pimpl->windowWidth = val;
        }
      }
      else if (parser.GetItems().query_key(i) == "windowHeight")
      {
        int val = 0;
        if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
        {
          m_pimpl->windowHeight = val;
        }
      }
      else if (parser.GetItems().query_key(i) == "glMajorVersion")
      {
        int val = 0;
        if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
        {
          m_pimpl->majorVersion = val;
        }
      }
      else if (parser.GetItems().query_key(i) == "glMinorVersion")
      {
        int val = 0;
        if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
        {
          m_pimpl->minorVersion = val;
        }
      }
      else if (parser.GetItems().query_key(i) == "glSamples")
      {
        int val = 0;
        if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
        {
          m_pimpl->samples = val;
        }
      }
      else if (parser.GetItems().query_key(i) == "fullscreen")
      {
        int val = 0;
        if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
        {
          m_pimpl->fullscreen = (val != 0);
        }
      }
    }
  }

  //Set up GL
  {
    if (!glfwInit())
    {
      fprintf(stderr, "Failed to initialize GLFW\n");
      return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_pimpl->majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_pimpl->minorVersion);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, m_pimpl->samples);

    if (m_pimpl->fullscreen)
    {
      if (m_pimpl->windowWidth == 0 || m_pimpl->windowHeight == 0)
      {
        const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        m_pimpl->windowWidth = mode->width;
        m_pimpl->windowHeight = mode->height;
      }
      m_pimpl->window = glfwCreateWindow(m_pimpl->windowWidth,
                                         m_pimpl->windowHeight,
                                         m_pimpl->title,
                                         glfwGetPrimaryMonitor(),
                                         nullptr);
    }
    else
    {
      m_pimpl->window = glfwCreateWindow(m_pimpl->windowWidth,
                                         m_pimpl->windowHeight,
                                         m_pimpl->title,
                                         nullptr,
                                         nullptr);
    }

    if (!m_pimpl->window)
    {
      fprintf(stderr, "Failed to open m_window\n");
      glfwTerminate();
      return;
    }

    glfwMakeContextCurrent(m_pimpl->window);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // Display version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
  }

  //Init ImGui
  ImGui_ImplGlfwGL3_Init(m_pimpl->window, true);

  //User Init method
  if (!Init())
  {
    printf("Failed to Init()\n");
    return;
  }

  double lastTick = glfwGetTime();

  do
  {
    double thisTick = glfwGetTime();
    double dt = static_cast<float>(thisTick - lastTick);
    lastTick = thisTick;

    glfwPollEvents();
    ImGui_ImplGlfwGL3_NewFrame();

    BuildUI();

    eObject e(nullptr);
    while (m_pimpl->eventManager.PollEvent(e))
    {
      e->DoEvent();
    }

    DoFrame(dt);

    ImGui::Render();
    glfwSwapBuffers(m_pimpl->window);

  } while (!glfwWindowShouldClose(m_pimpl->window) && !m_pimpl->shouldQuit);

  ImGui_ImplGlfwGL3_Shutdown();
  glfwDestroyWindow(m_pimpl->window);
  glfwTerminate();
}
