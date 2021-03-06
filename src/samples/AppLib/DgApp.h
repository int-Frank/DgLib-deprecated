#ifndef DGAPP_H
#define DGAPP_H

#include <map>
#include <string>

class Event;
struct GLFWwindow;

typedef std::map<std::string, std::string> map_str_str;

class DgApp
{
public:

  enum
  {
    AppInitFailed
  };

public:

  DgApp();

  virtual ~DgApp();

  void Run();
  static DgApp * GetInstance();

  void PushEvent(Event const &);

  std::string CurrentFile() const;
  virtual void NewProject() {}
  virtual bool LoadProject(std::string const & a_fileName) { return true; }
  virtual bool SaveProject(std::string const & a_filePath) { return true; }

  virtual void KeyEvent(int key, int action) {}
  void GetMousePosition(double &x, double &y);
  virtual void UpdateScroll(double) {}
  virtual void WindowSizeCallback(int w, int h) {}
  virtual void MouseMoveCallback(double x, double y) {}

  void SetDirty(bool = true);

  void SetQuitFlag(bool);
  bool ShouldQuit() const;

  //Logging
  //Log levels have the same values as the DgTypes.h -> LogLevel enum
  void SetLogLevel(int);
  void LogToOutputWindow(std::string const &, int);
  void LogToFile(std::string const &, int);
  void ToggleOutputWindow(bool);

protected:

  virtual void BuildUI() {}
  virtual void DoFrame(double dt) {}

  GLFWwindow * GetWindow();
  void GetWindowDimensions(int & w, int & h);
  map_str_str const & GetConfigItems() const;

  void AddFileHandlingMenuItems();
  void AddFileHandlingWindows();

private:

  class PIMPL;
  PIMPL * m_pimpl;

  //Prevent copy or assignment
  DgApp(const DgApp&);
  DgApp& operator= (const DgApp&);
};

#endif