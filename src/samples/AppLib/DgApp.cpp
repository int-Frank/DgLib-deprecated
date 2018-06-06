
#include <windows.h>
#include <shlwapi.h>
#include <time.h>
#include <stack>
#include <atomic>
#include <mutex>
#include <fstream>
#include <sstream>
#include <list>

#pragma comment(lib,"shlwapi.lib")

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "DgApp.h"
#include "DgParser_INI.h"
#include "DgStringFunctions.h"
#include "DgTypes.h"
#include "EventManager.h"
#include "Event.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

struct TextItem
{
  std::string text;
  int logLevel;
};

class TextList
{
public:

  typedef std::list<TextItem>::const_iterator const_iterator;
  typedef std::list<TextItem>::iterator iterator;

public:

  TextList()
    : m_maxSize(2000)
  {}

  void push_back(TextItem const & a_item) 
  { 
    m_items.push_back(a_item); 
    Trim();
  }

  iterator begin() { return m_items.begin(); }
  iterator end() { return m_items.end(); }

  const_iterator cbegin() const { return m_items.cbegin(); }
  const_iterator cend() const { return m_items.cend(); }

  void SetMaxCharacters(size_t a_size)
  {
    m_maxSize = a_size;
    Trim();
  }

private:

  void Trim()
  {
    size_t totalSize = 0;
    for (auto it = m_items.end(); it != m_items.begin();)
    {
      it--;
      totalSize += it->text.size();
      if (totalSize > m_maxSize)
      {
        it = m_items.erase(it);
      }
    }
  }

private:

  size_t m_maxSize;
  std::list<TextItem> m_items;
};

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
    DgApp::GetInstance()->SetDirty(false);
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

static void window_size_callback(GLFWwindow* a_window, int a_width, int a_height)
{
  DgApp::GetInstance()->WindowSizeCallback(a_width, a_height);
}

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
    , showOutputWindow(true)
    , save_lastItem(-2)
    , save_currentItem(-1)
    , save_buf{}
    , open_currentItem(-1)
    , configFileName("config.ini")
    , projectPath("./files/")
    , projectExtension("dgd")
    , window(nullptr)
    , shouldQuit(false)
    , logLevel(Dg::LL_Debug)
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
  std::atomic<int>    logLevel;

  bool                showOutputWindow;
  TextList            outputText;

  std::ofstream       logFile;
  std::mutex          mutexLogFile;
  std::mutex          mutexOutputText;

  std::string const   configFileName;
  map_str_str         configItems;

  std::string         projectPath;
  std::string         projectExtension;
  std::string         currentProject;
  char                save_buf[128];

  int                 open_currentItem;

  EventManager        eventManager;
  std::stack<int>     windowStack;

  GLFWwindow *        window;
  std::atomic<bool>   shouldQuit;
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

void DgApp::SetQuitFlag(bool a_val)
{
  m_pimpl->shouldQuit = a_val;
}

bool DgApp::ShouldQuit() const
{
  return m_pimpl->shouldQuit;
}

void DgApp::ToggleOutputWindow(bool a_val)
{
  m_pimpl->showOutputWindow = a_val;
}

void DgApp::LogToFile(std::string const & a_message, int a_lvl)
{
  if (a_lvl < m_pimpl->logLevel)
  {
    return;
  }

  std::lock_guard<std::mutex> lock(m_pimpl->mutexLogFile);

  switch (a_lvl)
  {
  case Dg::LL_Warning:
  {
    m_pimpl->logFile << "WARNING: ";
    break;
  }
  case Dg::LL_Error:
  {
    m_pimpl->logFile << "ERROR: ";
    break;
  }
  case Dg::LL_Info:
  {
    m_pimpl->logFile << "INFO: ";
    break;
  }
  case Dg::LL_Debug:
  {
    m_pimpl->logFile << "DEBUG: ";
    break;
  }
  case Dg::LL_Critical:
  {
    m_pimpl->logFile << "CRITICAL: ";
    break;
  }
  default:
  {
    break;
  }
  }

  m_pimpl->logFile << a_message << std::endl;
}

DgApp::DgApp()
  : m_pimpl(new DgApp::PIMPL())
{
  m_pimpl->logFile.open("log.txt");
  if (!m_pimpl->logFile.good())
  {
    std::cout << "Failed to open log file\n";
    throw AppInitFailed;
  }

  //Maybe add session infor in the log file...

  //TODO divert cout and cerr to Logging functions

  if (PIMPL::s_app != nullptr)
  {
    LogToFile("Attempt to create more than one instance of DgApp.", Dg::LL_Error);
    throw AppInitFailed;
  }

  PIMPL::s_app = this;

  //Read configuration
  do
  {
    Dg::Parser_INI parser;
    Dg::ErrorCode result = parser.Parse(m_pimpl->configFileName);

    if (result == Dg::Err_FailedToOpenFile)
    {
      LogToFile(std::string("Failed to open config file: ") + m_pimpl->configFileName, Dg::LL_Warning);
      break;
    }
    else if (result != Dg::Err_None)
    {
      LogToFile(std::string("Failed to parse config file: ") + m_pimpl->configFileName, Dg::LL_Warning);
      break;
    }

    m_pimpl->configItems = parser.GetItems();
    for (auto kv : m_pimpl->configItems)
    {
      if (kv.first == "windowWidth")
      {
        int val = 0;
        if (Dg::StringToNumber(val, kv.second, std::dec))
        {
          m_pimpl->windowWidth = val;
        }
      }
      else if (kv.first == "windowHeight")
      {
        int val = 0;
        if (Dg::StringToNumber(val, kv.second, std::dec))
        {
          m_pimpl->windowHeight = val;
        }
      }
      else if (kv.first == "glMajorVersion")
      {
        int val = 0;
        if (Dg::StringToNumber(val, kv.second, std::dec))
        {
          m_pimpl->majorVersion = val;
        }
      }
      else if (kv.first == "glMinorVersion")
      {
        int val = 0;
        if (Dg::StringToNumber(val, kv.second, std::dec))
        {
          m_pimpl->minorVersion = val;
        }
      }
      else if (kv.first == "glSamples")
      {
        int val = 0;
        if (Dg::StringToNumber(val, kv.second, std::dec))
        {
          m_pimpl->samples = val;
        }
      }
      else if (kv.first == "fullscreen")
      {
        int val = 0;
        if (Dg::StringToNumber(val, kv.second, std::dec))
        {
          m_pimpl->fullscreen = (val != 0);
        }
      }
    }
  } while (false);

  //Set up GL
  {
    if (!glfwInit())
    {
      LogToFile("Failed to initialize GLFW.", Dg::LL_Error);
      throw AppInitFailed;
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
      LogToFile("Failed to open glfw window.", Dg::LL_Error);
      glfwTerminate();
      throw AppInitFailed;
    }

    glfwMakeContextCurrent(m_pimpl->window);
    glfwSetWindowSizeCallback(m_pimpl->window, window_size_callback);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      LogToFile(std::string("glewInit() failed: ") + std::string((char*)glewGetErrorString(err)), Dg::LL_Error);
      glfwTerminate();
      throw AppInitFailed;
    }
  }

  //Init ImGui
  if (!ImGui_ImplGlfwGL3_Init(m_pimpl->window, true))
  {
    LogToFile(std::string("ImGui_ImplGlfwGL3_Init() returned false."), Dg::LL_Error);
    glfwDestroyWindow(m_pimpl->window);
    m_pimpl->window = nullptr;
    glfwTerminate();
    throw AppInitFailed;
  }

  //We need this for any constructors which use ImGui
  ImGui_ImplGlfwGL3_NewFrame();

  LogToOutputWindow(std::string("Renderer: " + std::string((char*)glGetString(GL_RENDERER))), Dg::LL_Info);
  LogToOutputWindow(std::string("OpenGL version supported " + std::string((char*)glGetString(GL_VERSION))), Dg::LL_Info);
}

void DgApp::SetLogLevel(int a_lvl)
{
  m_pimpl->logLevel = a_lvl;
}

void DgApp::LogToOutputWindow(std::string const & a_message, int a_lvl)
{
  TextItem ti;
  ti.text = a_message;
  ti.logLevel = a_lvl;

  std::lock_guard<std::mutex> lock(m_pimpl->mutexOutputText);
  m_pimpl->outputText.push_back(ti);
}

DgApp::~DgApp()
{
  ImGui_ImplGlfwGL3_Shutdown();
  glfwDestroyWindow(m_pimpl->window);
  glfwTerminate();

  delete m_pimpl;
  m_pimpl = nullptr;
  PIMPL::s_app = nullptr;
}

DgApp * DgApp::GetInstance()
{
  return PIMPL::s_app;
}

GLFWwindow * DgApp::GetWindow()
{
  return m_pimpl->window;
}

void DgApp::GetWindowDimensions(int & w, int & h)
{
  glfwGetWindowSize(m_pimpl->window, &w, &h);
}

map_str_str const & DgApp::GetConfigItems() const
{
  return m_pimpl->configItems;
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
  ImGui::Separator();
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

void DgApp::Run()
{
  double lastTick = glfwGetTime();
  do
  {
    double thisTick = glfwGetTime();
    double dt = static_cast<float>(thisTick - lastTick);
    lastTick = thisTick;

    glfwPollEvents();
    ImGui_ImplGlfwGL3_NewFrame();

    //Build log window
    m_pimpl->mutexOutputText.lock();
    auto outputText = m_pimpl->outputText;
    m_pimpl->mutexOutputText.unlock();

    if (m_pimpl->showOutputWindow)
    {
      ImGui::Begin("Output", &m_pimpl->showOutputWindow);
      for (auto const & ti : outputText)
      {
        switch (ti.logLevel)
        {
        case Dg::LL_Debug:
        {
          ImGui::TextColored(ImVec4(1.0, 0.0, 1.0, 1.0), "DEBUG:");
          break;
        }
        case Dg::LL_Info:
        {
          ImGui::TextColored(ImVec4(0.5, 0.5, 1.0, 1.0), "INFO: ");
          break;
        }
        case Dg::LL_Warning:
        {
          ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "WARN: ");
          break;
        }
        case Dg::LL_Error:
        {
          ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "ERROR:");
          break;
        }
        case Dg::LL_Critical:
        {
          ImGui::TextColored(ImVec4(0.0, 1.0, 1.0, 1.0), "CRIT: ");
          break;
        }
        default:
        {
          std::stringstream ss;
          ss << "Unrecognised log level: " << ti.logLevel;
          LogToOutputWindow(ss.str(), Dg::LL_Warning);
          ImGui::TextColored(ImVec4(1.0, 0.0, 1.0, 1.0), "???:    ");
          break;
        }
        }
        ImGui::SameLine();
        ImGui::TextWrapped(ti.text.c_str());
      }
      ImGui::End();
    }

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
}
