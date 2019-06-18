//! @file DgSingleton.h
//!
//! @author Frank Hart
//! @date 4/10/2015
//!
//! Class declaration: DgSingleton

#ifndef SINGLETON_H
#define SINGLETON_H

//TODO need this?
namespace Dg
{
  //! @ingroup DgUtility_types
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

    //! Get pointer to the static instance of the object
    static T * Instance() 
    {
      static T s_instance;
      return &s_instance;
    }
   
  protected:

    //! Prevent construction
    Singleton(){}

    //! Prevent construction by copying
    Singleton(Singleton const &);

    //! Prevent assignment
    Singleton & operator=(Singleton const &);

    //! Prevent unwanted destruction
    virtual ~Singleton(){}

  };


}

#endif