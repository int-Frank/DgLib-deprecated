#ifndef UI_H
#define UI_H

namespace UI
{
  extern bool showAlphaBlendingWindow;
}

class UIData
{
public:
  void Update();

  int emitterType;
  int prev_emitterType;

  bool onOff;
  bool prev_onOff;

  int posGenMethod;
  int prev_posGenMethod;

  float posX;
  float prev_posX;

  float posY;
  float prev_posY;

  float posZ;
  float prev_posZ;

  float rx;
  float prev_rx;

  float rz;
  float prev_rz;

  float spread;
  float prev_spread;

  float boxL;
  float prevBoxL;

  float boxW;
  float prevBoxW;

  float boxH;
  float prevBoxH;

  bool repelFromCenter;
  bool prev_repelFromCenter;

  float Velrx;
  float prev_Velxr;

  float Velrz;
  float prev_Velrz;

  float sphereRadius;
  float prev_sphereRadius;

  float startColor[4] = {};
  float prev_startColor[4] = {};

  float endColor[4] = {};
  float prev_endColor[4] = {};

  float rate;
  float prev_rate;

  float velocity;
  float prev_velocity;

  float life;
  float prev_life;

  float force;
  float prev_force;

  float startSize;
  float prev_startSize;

  float endSize;
  float prev_endSize;
};

#endif