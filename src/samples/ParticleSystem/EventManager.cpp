#include "EventManager.h"
#include "Events.h"

EventManager::EventManager(EventManager const & a_other)
{
  std::lock_guard<std::mutex> lock(a_other.m_mutex);
  m_eventQueue = a_other.m_eventQueue;
}

EventManager & EventManager::operator=(EventManager const & a_other)
{
  if (this != &a_other)
  {
    std::lock(m_mutex, a_other.m_mutex);
    std::lock_guard<std::mutex> lhs_lk(m_mutex, std::adopt_lock);
    std::lock_guard<std::mutex> rhs_lk(a_other.m_mutex, std::adopt_lock);

    m_eventQueue = a_other.m_eventQueue;
  }
  return *this;
}

void EventManager::PushEvent(Event const & a_pEvent)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_eventQueue.push_back(eObject(a_pEvent));
}

bool EventManager::PollEvent(eObject & a_out)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if (!m_eventQueue.empty())
  {
    a_out = m_eventQueue.front();
    m_eventQueue.pop_front();
    return true;
  }
  return false;
}