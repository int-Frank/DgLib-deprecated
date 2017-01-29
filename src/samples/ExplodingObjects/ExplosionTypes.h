#ifndef EXPLOSIONTYPES_H
#define EXPLOSIONTYPES_H

#include <vector>
#include "Types.h"

//----------------------------------------------------------------------------------
//  ExplosionTypeBase...
//----------------------------------------------------------------------------------

class ExplosionTypeBase
{
public:

  virtual ~ExplosionTypeBase() {}
  void Init(vec4 const & a_source);
  virtual TransformData GetData(vec4 const & a_centroid);

protected:

  virtual void PostInit() {}

protected:

  vec4 m_source;
};

//----------------------------------------------------------------------------------
//  ExplosionType_Complete...
//----------------------------------------------------------------------------------

class ExplosionType_Complete : public ExplosionTypeBase
{
public:

  TransformData GetData(vec4 const & a_centroid);
};

//----------------------------------------------------------------------------------
//  ExplosionType_Chunked...
//----------------------------------------------------------------------------------

class ExplosionType_Chunked : public ExplosionTypeBase
{
public:

  TransformData GetData(vec4 const & a_centroid);

private:

  void PostInit();

private:

  std::vector<vec4>  m_axes;
};

#endif