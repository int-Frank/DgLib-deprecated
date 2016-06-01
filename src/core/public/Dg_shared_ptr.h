//! @file Dg_shared_ptr.h
//!
//! @author: Frank B. Hart
//! @date 22/05/2016
//!
//! Class declaration: shared_ptr

#ifndef DG_SHARED_PTR_H
#define DG_SHARED_PTR_H

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class DefaultDeleter
  //!
  //! Default deleter struct for the shared_ptr class.
  //!
  //! @author Frank B. Hart
  //! @date 22/05/2016
  template<typename T>
  struct DefaultDeleter
  {
    //! Deleting operator
    void operator()(T * a_obj) const { delete a_obj; }
  };

  //! @ingroup DgContainers
  //!
  //! @class shared_ptr
  //!
  //! Reference counted pointer
  //!
  //! @author Frank B. Hart
  //! @date 22/05/2016
  template < typename T, typename Deleter = DefaultDeleter<T> > 
  class shared_ptr
  {
  public:

    //! Default constructor
    shared_ptr() : m_pData(nullptr), m_pRefCount(nullptr)
    {
      m_pRefCount = new size_t(1);
    }

    //! Construct from pointer to object. The shared_point then assumes ownership.
    //!
    //! @param[in] pValue Pointer to object.
    shared_ptr(T* pValue) : m_pData(pValue), m_pRefCount(nullptr)
    {
      m_pRefCount = new size_t(1);
    }

    //! Copy constructor
    shared_ptr(shared_ptr<T> const & sp) : m_pData(sp.m_pData), m_pRefCount(sp.m_pRefCount)
    {
      ++(*m_pRefCount);
    }

    //! Destructor. If no other shared_ptr's are pointing to the object,
    //! the object is destroyed.
    ~shared_ptr()
    {
      if (--(*m_pRefCount) == 0)
      {
        s_deleter(m_pData);
        delete m_pRefCount;
      }
    }

    //! Conversion
    //!
    //! @return Reference to object
    T& operator* ()
    {
      return *m_pData;
    }

    //! Conversion
    //!
    //! @return Pointer to object
    T* operator-> ()
    {
      return m_pData;
    }

    //! Conversion
    //!
    //! @return const reference to object
    T const & operator* () const
    {
      return *m_pData;
    }

    //! Conversion
    //!
    //! @return const pointer to object
    T const * operator-> () const
    {
      return m_pData;
    }

    //! Assignment
    shared_ptr& operator = (shared_ptr const & sp)
    {
      if (this != &sp)
      {
        if (--(*m_pRefCount) == 0)
        {
          s_deleter(m_pData);
          delete m_pRefCount;
        }

        m_pData = sp.m_pData;
        m_pRefCount = sp.m_pRefCount;
        ++(*m_pRefCount);
      }
      return *this;
    }

  private:

    static Deleter const s_deleter;

  private:
    T *          m_pData;       // pointer
    size_t *     m_pRefCount;
  };


  template <typename T, typename Deleter>
  Deleter const shared_ptr<T, Deleter>::s_deleter;

}
#endif