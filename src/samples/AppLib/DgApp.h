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

  virtual void KeyEvent(int, int) {}
  virtual void UpdateScroll(double) {}

  void SetDirty(bool);

  void SetQuitFlag(bool);
  bool ShouldQuit() const;

  //No nextline character is needed.
  void WriteToOutput(std::string const &, unsigned long color);
  void SetUpOutputWindow(int x, int y, int w, int h, int flags);

protected:

  virtual void BuildUI() {}
  virtual void DoFrame(double dt) {}

  GLFWwindow * GetWindow();
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