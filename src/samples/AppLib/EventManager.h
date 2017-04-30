#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <deque>
#include <mutex>
#include "DgObjectWrapper.h"

class Event;

typedef Dg::ObjectWrapper<Event> eObject;

class EventManager
{
public:

  EventManager() {}
  ~EventManager() {}
  EventManager(EventManager const &);
  EventManager & operator=(EventManager const &);

  void PushEvent(Event const &);
  bool PollEvent(eObject &);

private:
  mutable std::mutex      m_mutex;
  std::deque<eObject>     m_eventQueue;
};

#endif