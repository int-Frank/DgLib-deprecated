
#include <cmath>
#include <string>
#include <sstream>

#include "StopwatchApp.h"
#include "imgui.h"

StopwatchApp::StopwatchApp()
{
  ToggleOutputWindow(false);
}

void AmmendTimeString(std::string & a_out, int a_val)
{
  if (a_out.size() != 0)
  {
    a_out += ":";
  }
  std::stringstream ss;
  ss << a_val;
  std::string str = ss.str();
  if (str.size() < 2)
  {
    str = "0" + str;
  }
  a_out += str;
}

void StopwatchApp::BuildUI()
{
  ImGui::Begin("Stopwatch", nullptr, ImGuiWindowFlags_NoTitleBar
                                   | ImGuiWindowFlags_NoResize
                                   | ImGuiWindowFlags_NoMove
                                   | ImGuiWindowFlags_NoScrollbar
                                   | ImGuiWindowFlags_NoScrollWithMouse
                                   | ImGuiWindowFlags_NoCollapse
                                   | ImGuiWindowFlags_NoSavedSettings);
  double time = m_timer.GetTime();
  int hours = int(time / 3600.0) % 100;
  int minutes = int(time / 60.0) % 60;
  int seconds = int(time) % 60;
  int centiseconds = int(time * 100) % 100;

  std::string timeString;
  AmmendTimeString(timeString, hours);
  AmmendTimeString(timeString, minutes);
  AmmendTimeString(timeString, seconds);
  AmmendTimeString(timeString, centiseconds);

  ImGui::Text(timeString.c_str());

  if (ImGui::Button("Start")) m_timer.Start();
  if (ImGui::Button("Stop")) m_timer.Stop();
  if (ImGui::Button("Pause")) m_timer.Pause();
  if (ImGui::Button("Resume")) m_timer.Resume();

  ImGui::End();
}

void StopwatchApp::DoFrame()
{

}