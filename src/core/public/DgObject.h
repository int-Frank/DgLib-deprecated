#ifndef DGOBJECT_H
#define DGOBJECT_H

template<typename T>
class Object
{
public:

  Object() {}
  virtual ~Object() {}

  /*!
  * @brief Creates a deep copy of a derived object.
  *
  * @return Pointer to the newly created object.
  */
  virtual T* Clone() const = 0;

};

#endif