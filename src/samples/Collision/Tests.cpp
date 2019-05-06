#include "DirectionMask.h"

#define CHECK(val) if(!val){return __LINE__;}

int TestDirectionMask()
{
  DirMask dm;
  vec3 v(0.0f, 1.0f, 0.0f);

  //------------------------------------------------------------------------------------------
  // DM_None
  //------------------------------------------------------------------------------------------
  CHECK(dm.InMask(v) == false);


  //------------------------------------------------------------------------------------------
  // DM_D
  //------------------------------------------------------------------------------------------
  dm.Add(vec3(0.0f, 1.0f, 0.0f));

  CHECK(dm.InMask(v) == true);
  CHECK(dm.InMask(vec3(1.0f, 0.0f, 0.0f)) == false);

  v.Set(0.001f, 1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) == false);

  //------------------------------------------------------------------------------------------
  // DM_LDR
  //------------------------------------------------------------------------------------------
  dm.Add(vec3(-1.0, 0.0, 0.0));
  CHECK(dm.InMask(vec3(-1.0, 0.0, 0.0)) == true);
  CHECK(dm.InMask(vec3(0.0, 1.0, 0.0)) == true);
  v.Set(-1.0f, 1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) == true);

  CHECK(dm.InMask(vec3(0.0f, -1.0f, 0.0f)) == false);
  v.Set(-1.0f, -0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) == false);
  v.Set(0.001f, 1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) == false);

  v.Set(1.0f, 1.0f, 0.0f);
  v.Normalize();
  dm.Add(v);
  CHECK(dm.InMask(v) == true);
  CHECK(dm.InMask(vec3(0.0f, -1.0f, 0.0f)) == false);
  CHECK(dm.InMask(vec3(1.0f, 0.0f, 0.0f)) == false);

  v.Set(1.0f, 1.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) == true);

  v.Set(1.0f, 0.999f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  false);

  dm.Add(vec3(1.0f, 0.0f, 0.0f));
  CHECK(dm.InMask(vec3(1.0f, 0.0f, 0.0f)) ==  true);
  CHECK(dm.InMask(vec3(-1.0f, 0.0f, 0.0f)) == true);
  CHECK(dm.InMask(vec3(0.0f, 1.0f, 0.0f)) == true);

  v.Set(1.0f, 0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) == true);

  v.Set(1.0f, -0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) == false);

  v.Set(-1.0f, 0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) == true);

  v.Set(-1.0f, -0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) == false);

  //------------------------------------------------------------------------------------------
  // DM_S
  //------------------------------------------------------------------------------------------
  v.Set(1.0f, -0.001f, 0.0f);
  v.Normalize();
  dm.Add(v);

  v.Set(1.0f, -0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  v.Set(-1.0f, -0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  v.Set(2.0f, 1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  v.Set(-2.0f, -1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  v.Set(-2.0f, 1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  //------------------------------------------------------------------------------------------
  // DM_LR
  //------------------------------------------------------------------------------------------

  dm.Reset();
  dm.Add(vec3(-1.0f, 0.0f, 0.0f));
  dm.Add(vec3(1.0f, 0.0f, 0.0f));

  CHECK(dm.InMask(vec3(1.0f, 0.0f, 0.0f)) ==  true);
  CHECK(dm.InMask(vec3(-1.0f, 0.0f, 0.0f)) ==  true);

  v.Set(1.0f, -0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  false);

  v.Set(-1.0f, -0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  false);

  v.Set(2.0f, 1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  false);

  v.Set(-2.0f, -1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  false);

  v.Set(-2.0f, 1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  false);

  //------------------------------------------------------------------------------------------
  // DM_LRD
  //------------------------------------------------------------------------------------------

  dm.Add(vec3(0.0f, -1.0f, 0.0f));

  CHECK(dm.InMask(vec3(1.0f, 0.0f, 0.0f)) ==  true);
  CHECK(dm.InMask(vec3(-1.0f, 0.0f, 0.0f)) ==  true);

  v.Set(1.0f, -0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  v.Set(-1.0f, -0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  v.Set(2.0f, 1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  false);

  v.Set(-2.0f, -1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  v.Set(-2.0f, 1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  false);

  v.Set(2.0f, -1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  //------------------------------------------------------------------------------------------
  // DM_S again
  //------------------------------------------------------------------------------------------

  dm.Add(vec3(1.0f, 0.001f, 0.0f));

  v.Set(1.0f, -0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  v.Set(-1.0f, -0.001f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  v.Set(2.0f, 1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  v.Set(-2.0f, -1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  v.Set(-2.0f, 1.0f, 0.0f);
  v.Normalize();
  CHECK(dm.InMask(v) ==  true);

  //------------------------------------------------------------------------------------------
  // All good :)
  //------------------------------------------------------------------------------------------
  return 0;
}