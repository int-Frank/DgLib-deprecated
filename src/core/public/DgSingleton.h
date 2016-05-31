//! @file DgSingleton.h
//!
//! @author Frank Hart
//! @date 4/10/2015
//!
//! Class declaration: DgSingleton

#ifndef SINGLETON_H
#define SINGLETON_H

namespace Dg
{
  //! @ingroup utility_classes
  //!
  //! @class Singleton
  //!
  //! @brief Singleton
  //!
  //! @author Frank Hart
  //! @date 4/10/2015
  template<typename T>
  class Singleton
  {
  public:

    static T * Instance() 
    {
      static T s_instance;
      return &s_instance;
    }
   
  protected:

    Singleton(){} // Prevent construction
    Singleton(Singleton const &); // Prevent construction by copying
    Singleton & operator=(Singleton const &); // Prevent assignment
    virtual ~Singleton(){} // Prevent unwanted destruction

  };


}

#endif