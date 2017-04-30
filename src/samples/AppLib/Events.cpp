
#include <cstdio>
#include <fstream>

#include "Events.h"
#include "DgApp.h"

void Event_MouseScroll::DoEvent()
{
  DgApp::GetInstance()->UpdateScroll(m_val);
}

void Event_KeyEvent::DoEvent()
{
  DgApp::GetInstance()->KeyEvent(m_key, m_action);
}