
#include <cstdio>
#include <fstream>

#include "Events.h"
#include "Application.h"

void Event_MouseScroll::DoEvent()
{
  Application::GetInstance()->UpdateScroll(m_val);
}

void Event_KeyEvent::DoEvent()
{
  Application::GetInstance()->KeyEvent(m_key, m_action);
}
