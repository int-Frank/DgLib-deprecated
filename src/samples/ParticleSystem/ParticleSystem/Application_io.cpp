
#include <fstream>
#include <iostream>

#include "Application.h"
#include "./json/json.h"

bool Application::LoadProject(std::string a_file)
{
  std::ifstream fs;
  fs.open("./projects/test_json_file.dgp");
  if (fs.good())
  {
    Json::Value root;
    fs >> root;
    std::cout << root;
  }

  return true;
}

bool Application::SaveProject(std::string a_file)
{
  printf("%s saved!\n", a_file.c_str());

  Json::Value root, sub;
  sub["One"] = 7;
  root["Emitter"] = 1;
  root["Attractor"] = 2;
  root["Attractor1"] = 3;
  root["Attractor2"] = sub;

  //root["Emitter"]["Item"] = 3;
  //attr["Item"] = "3";

  std::ofstream fs;
  fs.open("./projects/test_json_file.dgp");
  if (fs.good())
  {
    fs << root;
  }

  return true;
}