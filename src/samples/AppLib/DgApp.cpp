
#include <windows.h>
#include <shlwapi.h>
#include <time.h>
#include <stack>
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


class Event_SaveProject : public Event
{
public:
  Event_SaveProject()
    : Event()
  {}

  ~Event_SaveProject() {}

  Event_SaveProject(Event_SaveProject const & a_other)
    : Event(a_other)
    , m_filePath(a_other.m_filePath)
  {}

  Event_SaveProject & operator=(Event_SaveProject const & a_other)
  {
    Event::operator = (a_other);
    m_filePath = a_other.m_filePath;
  }

  void SetFilePath(const std::string & a_filePath)
  {
    m_filePath = a_filePath;
  }

  void DoEvent()
  {
    DgApp::GetInstance()->SaveProject(m_filePath);
    DgApp::GetInstance()->SetDirty(false);
  }

  Event_SaveProject * Clone() const { return new Event_SaveProject(*this); }

private:
  std::string    m_filePath;
};


class Event_LoadProject : public Event
{
public:
  Event_LoadProject()
    : Event()
  {}

  ~Event_LoadProject() {}

  Event_LoadProject(Event_LoadProject const & a_other)
    : Event(a_other)
    , m_filePath(a_other.m_filePath)
  {}

  Event_LoadProject & operator=(Event_LoadProject const & a_other)
  {
    Event::operator = (a_other);
    m_filePath = a_other.m_filePath;
  }

  void SetFilePath(const std::string & a_filePath)
  {
    m_filePath = a_filePath;
  }

  void DoEvent()
  {
    DgApp::GetInstance()->LoadProject(m_filePath);
  }

  Event_LoadProject * Clone() const { return new Event_LoadProject(*this); }

private:
  std::string    m_filePath;
};


class Event_NewProject : public Event
{
public:
  Event_NewProject()
    : Event()
  {}

  ~Event_NewProject() {}

  Event_NewProject(Event_LoadProject const & a_other)
    : Event(a_other)
  {}

  Event_NewProject & operator=(Event_NewProject const & a_other)
  {
    Event::operator = (a_other);
  }

  void DoEvent()
  {
    DgApp::GetInstance()->NewProject();
  }

  Event_NewProject * Clone() const { return new Event_NewProject(*this); }

};

class Event_DeleteFile : public Event
{
public:
  Event_DeleteFile() : Event() {}
  ~Event_DeleteFile() {}

  Event_DeleteFile(Event_DeleteFile const & a_other)
    : Event(a_other)
    , m_fileName(a_other.m_fileName)
  {}

  Event_DeleteFile & operator=(Event_DeleteFile const & a_other)
  {
    Event::operator = (a_other);
    m_fileName = a_other.m_fileName;
  }

  void DoEvent()
  {
    std::remove(m_fileName.c_str());
  }

  void SetFileName(std::string const & a_file) { m_fileName = a_file; }
  std::string GetFileName() const { return m_fileName; }

  Event_DeleteFile * Clone() const { return new Event_DeleteFile(*this); }

private:
  std::string       m_fileName;
};

class DgApp::PIMPL
{
public:

  enum Modal
  {
    SavePrompt,
    OverwriteWindow,
    SaveAsWindow,
    OpenWindow,
    NewProjectRequest,
    None
  };

  PIMPL()
    : title("New Dg Application")
    , windowWidth(800)
    , windowHeight(600)
    , majorVersion(4)
    , minorVersion(3)
    , samples(0)
    , fullscreen(false)
    , isDirty(false)
    , save_lastItem(-2)
    , save_currentItem(-1)
    , save_buf{}
    , open_currentItem(-1)
    , configFileName("config.ini")
    , projectPath("./files/")
    , projectExtension("dgd")
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
  bool                isDirty;
  int                 save_lastItem;
  int                 save_currentItem;

  std::string const   configFileName;

  std::string         projectPath;
  std::string         projectExtension;
  std::string         currentProject;
  char                save_buf[128];

  int                 open_currentItem;

  EventManager        eventManager;
  std::stack<int>     windowStack;

  GLFWwindow *        window;
  bool                shouldQuit;
};

DgApp * DgApp::PIMPL::s_app(nullptr);

bool FileExists(std::string const & a_name)
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

std::vector<std::string> GetFiles(std::string const & a_dirPath,
                                  std::string a_ext)
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

DgApp * DgApp::GetInstance()
{
  return PIMPL::s_app;
}

GLFWwindow * DgApp::GetWindow()
{
  return m_pimpl->window;
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

std::string DgApp::CurrentFile() const
{
  return m_pimpl->currentProject;
}

void DgApp::SetDirty(bool a_isDirty)
{
  m_pimpl->isDirty = a_isDirty;
}

void DgApp::AddFileHandlingMenuItems()
{
  if (ImGui::MenuItem("New"))
  {
    m_pimpl->windowStack.push(PIMPL::Modal::NewProjectRequest);
    if (m_pimpl->isDirty)
    {
      m_pimpl->windowStack.push(PIMPL::Modal::SavePrompt);
    }
  }
  if (ImGui::MenuItem("Open"))
  {
    m_pimpl->windowStack.push(PIMPL::Modal::OpenWindow);
    if (m_pimpl->isDirty)
    {
      m_pimpl->windowStack.push(PIMPL::Modal::SavePrompt);
    }
  }
  if (ImGui::MenuItem("Save"))
  {
    if (m_pimpl->currentProject == "")
    {
      m_pimpl->windowStack.push(PIMPL::Modal::SaveAsWindow);
    }
    else
    {
      Event_SaveProject e;
      e.SetFilePath(m_pimpl->currentProject);
      m_pimpl->eventManager.PushEvent(e);
    }
  }
  if (ImGui::MenuItem("Save As.."))
  {
    m_pimpl->windowStack.push(PIMPL::Modal::SaveAsWindow);
  }
}

static std::vector<std::string> GetProjects(std::string const & a_projectPath)
{
  std::vector<std::string> result;
  WIN32_FIND_DATA ffd;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  std::string szDir = a_projectPath + "*";

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
      if (ext == "dgd")
      {
        result.push_back(ffd.cFileName);
      }
    }
  } while (FindNextFile(hFind, &ffd) != 0);

  FindClose(hFind);
  return result;
}

static void CreateFileList(std::vector<std::string> const & a_items,
                           std::string const & a_projectPath,
                           char const * a_name,
                           int * a_currentItem)
{
  if (a_items.size() == 0)
  {
    ImGui::ListBox(a_name, a_currentItem, nullptr, 0, 5);
    *a_currentItem = -1;
  }
  else
  {
    char * * pItems = new char*[a_items.size()];
    for (int i = 0; i < a_items.size(); ++i)
    {
      pItems[i] = new char[a_items[i].size() + 1]();
      memcpy(pItems[i], a_items[i].data(), a_items[i].size() * sizeof(char));
    }
    if (a_items.size() == 0)
    {
      *a_currentItem = -1;
    }
    ImGui::ListBox(a_name, a_currentItem, (char const **)pItems, (int)a_items.size(), 5);

    if (ImGui::BeginPopupContextItem("item context menu"))
    {
      if (ImGui::Selectable("Delete selected"))
      {
        if (*a_currentItem >= 0)
        {
          Event_DeleteFile e;
          e.SetFileName(a_projectPath + a_items[*a_currentItem]);
          DgApp::GetInstance()->PushEvent(e);
        }
      }
      ImGui::EndPopup();
    }
    for (int i = 0; i < a_items.size(); ++i)
    {
      delete[] pItems[i];
    }
    delete[] pItems;
  }
}

void DgApp::AddFileHandlingWindows()
{
  //----------------------------------------------------------------------------------
  //  New Project created info box
  //----------------------------------------------------------------------------------
  if (!m_pimpl->windowStack.empty() && m_pimpl->windowStack.top() == PIMPL::Modal::NewProjectRequest)
  {
    Event_NewProject e;
    m_pimpl->eventManager.PushEvent(e);
    m_pimpl->windowStack.pop();
  }


  //----------------------------------------------------------------------------------
  //  Save Prompt
  //----------------------------------------------------------------------------------
  if (!m_pimpl->windowStack.empty() && m_pimpl->windowStack.top() == PIMPL::Modal::SavePrompt)
  {
    ImGui::OpenPopup("Save current Project?");
  }
  if (ImGui::BeginPopupModal("Save current Project?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    bool finished = false;
    if (ImGui::Button("Yes", ImVec2(80, 0)))
    {
      m_pimpl->windowStack.pop();
      m_pimpl->windowStack.push(PIMPL::Modal::SaveAsWindow);
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("No", ImVec2(80, 0)))
    {
      m_pimpl->windowStack.pop();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }


  //----------------------------------------------------------------------------------
  //  Overwrite Prompt
  //----------------------------------------------------------------------------------
  if (!m_pimpl->windowStack.empty() && m_pimpl->windowStack.top() == PIMPL::Modal::OverwriteWindow)
  {
    ImGui::OpenPopup("Overwrite?");
  }
  if (ImGui::BeginPopupModal("Overwrite?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("File already exists. Overwrite?");
    if (ImGui::Button("Yes", ImVec2(80, 0)))
    {
      //Save...
      m_pimpl->currentProject = std::string(m_pimpl->save_buf);
      Event_SaveProject e;
      e.SetFilePath(m_pimpl->projectPath + m_pimpl->currentProject);
      m_pimpl->eventManager.PushEvent(e);

      //Reset
      memset(m_pimpl->save_buf, 0, sizeof(m_pimpl->save_buf) / sizeof(char));
      m_pimpl->save_lastItem = -2;
      m_pimpl->save_currentItem = -1;
      m_pimpl->windowStack.pop();
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("No", ImVec2(80, 0)))
    {
      m_pimpl->windowStack.pop();
      m_pimpl->windowStack.push(PIMPL::Modal::SaveAsWindow);
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }


  //----------------------------------------------------------------------------------
  //  Save As Window
  //----------------------------------------------------------------------------------
  if (!m_pimpl->windowStack.empty() && m_pimpl->windowStack.top() == PIMPL::Modal::SaveAsWindow)
  {
    ImGui::OpenPopup("Save As..");
  }
  if (ImGui::BeginPopupModal("Save As..", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    std::vector<std::string> files = GetProjects(m_pimpl->projectPath);

    CreateFileList(files, m_pimpl->projectPath, "Files", &m_pimpl->save_currentItem);

    if (m_pimpl->save_currentItem != m_pimpl->save_lastItem && m_pimpl->save_currentItem >= 0)
    {
      strcpy_s(m_pimpl->save_buf, 128, files[m_pimpl->save_currentItem].data());
      m_pimpl->save_lastItem = m_pimpl->save_currentItem;
    }
    ImGui::InputText("File name", m_pimpl->save_buf, 128);

    bool shouldClose = false;
    bool shouldReset = true;
    if (ImGui::Button("Save", ImVec2(120, 0)))
    {
      std::string save_finalFile = m_pimpl->projectPath + std::string(m_pimpl->save_buf);
      std::string dotExt = std::string(".") + m_pimpl->projectExtension;
      if (save_finalFile.rfind(dotExt) != save_finalFile.size() - 4)
      {
        save_finalFile += dotExt;
      }
      m_pimpl->windowStack.pop();
      shouldClose = true;
      if (FileExists(save_finalFile))
      {
        m_pimpl->windowStack.push(PIMPL::Modal::OverwriteWindow);
        shouldReset = false;
      }
      else
      {
        //Save...
        m_pimpl->currentProject = std::string(m_pimpl->save_buf);
        Event_SaveProject e;
        e.SetFilePath(save_finalFile);
        m_pimpl->eventManager.PushEvent(e);
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      m_pimpl->windowStack.pop();
      shouldClose = true;
    }

    if (shouldClose)
    {
      if (shouldReset)
      {
        memset(m_pimpl->save_buf, 0, sizeof(m_pimpl->save_buf) / sizeof(m_pimpl->save_buf[0]));
        m_pimpl->save_lastItem = -2;
        m_pimpl->save_currentItem = -1;
      }

      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }


  //----------------------------------------------------------------------------------
  //  Open Project Window
  //----------------------------------------------------------------------------------
  if (!m_pimpl->windowStack.empty() && m_pimpl->windowStack.top() == PIMPL::Modal::OpenWindow)
  {
    ImGui::OpenPopup("Open file");
  }
  if (ImGui::BeginPopupModal("Open file", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    std::vector<std::string> files = GetProjects(m_pimpl->projectPath);
    CreateFileList(files, m_pimpl->projectPath, "Files", &m_pimpl->open_currentItem);

    bool finished = false;
    if (ImGui::Button("Open", ImVec2(120, 0)))
    {
      if (m_pimpl->open_currentItem != -1)
      {
        m_pimpl->currentProject = files[m_pimpl->open_currentItem];
        Event_LoadProject e;
        e.SetFilePath(m_pimpl->projectPath + files[m_pimpl->open_currentItem]);
        m_pimpl->eventManager.PushEvent(e);
      }
      finished = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      finished = true;
    }
    if (finished)
    {
      m_pimpl->open_currentItem = -1;
      m_pimpl->windowStack.pop();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void DgApp::Run(DgApp* the_app)
{
  PIMPL::s_app = the_app;

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
