#ifndef EVENTS_H
#define EVENTS_H

#include <string>
#include "DgObject.h"

class Event : public Dg::Object<Event>
{
public:

  Event(){}

  virtual ~Event() {}

  Event(Event const & a_other){}

  Event & operator=(Event const & a_other)
  {
    return *this;
  }

  virtual void DoEvent() {}

  Event * Clone() const { return new Event(*this); }
};

class Event_LoadProject : public Event
{
public:
  Event_LoadProject() : Event() {}
  ~Event_LoadProject() {}

  Event_LoadProject(Event_LoadProject const & a_other)
    : Event(a_other)
    , m_fileName(a_other.m_fileName)
  {}

  Event_LoadProject & operator=(Event_LoadProject const & a_other)
  {
    Event::operator = (a_other);
    m_fileName = a_other.m_fileName;
  }

  void DoEvent();
  void SetFileName(std::string const & a_file) { m_fileName = a_file; }
  std::string GetFileName() const { return m_fileName; }
  
  Event_LoadProject * Clone() const { return new Event_LoadProject(*this); }

private:
  std::string       m_fileName;
};


class Event_SaveProject : public Event
{
public:
  Event_SaveProject() : Event() {}
  ~Event_SaveProject() {}

  Event_SaveProject(Event_SaveProject const & a_other)
    : Event(a_other)
    , m_fileName(a_other.m_fileName)
  {}

  Event_SaveProject & operator=(Event_SaveProject const & a_other)
  {
    Event::operator = (a_other);
    m_fileName = a_other.m_fileName;
  }

  void DoEvent();
  void SetFileName(std::string const & a_file) { m_fileName = a_file; }
  std::string GetFileName() const { return m_fileName; }

  Event_SaveProject * Clone() const { return new Event_SaveProject(*this); }

private:
  std::string       m_fileName;
};

class Event_NewProject : public Event
{
public:
  Event_NewProject() : Event() {}
  ~Event_NewProject() {}

  Event_NewProject(Event_NewProject const & a_other)
    : Event(a_other)
  {}

  Event_NewProject & operator=(Event_NewProject const & a_other)
  {
    Event::operator = (a_other);
  }

  void DoEvent();
  Event_NewProject * Clone() const { return new Event_NewProject(*this); }

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

  void DoEvent();
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

  void DoEvent();
  void SetKey(int a_key) { m_key = a_key; }
  void SetAction(int a_action) { m_action = a_action; }
  int GetKey() const { return m_key; }
  int GetAction() const { return m_action; }

  Event_KeyEvent * Clone() const { return new Event_KeyEvent(*this); }

private:
  int       m_key;
  int       m_action;
};

#endif