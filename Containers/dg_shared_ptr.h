//! @file dg_shared_ptr.h
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

    shared_ptr() : m_pData(nullptr), m_pRefCount(nullptr)
    {
      m_pRefCount = new size_t(1);
    }

    shared_ptr(T* pValue) : m_pData(pValue), m_pRefCount(nullptr)
    {
      m_pRefCount = new size_t(1);
    }

    shared_ptr(shared_ptr<T> const & sp) : m_pData(sp.m_pData), m_pRefCount(sp.m_pRefCount)
    {
      ++(*m_pRefCount);
    }

    ~shared_ptr()
    {
      if (--(*m_pRefCount) == 0)
      {
        s_deleter(m_pData);
        delete m_pRefCount;
      }
    }

    T& operator* ()
    {
      return *m_pData;
    }

    T* operator-> ()
    {
      return m_pData;
    }

    T const & operator* () const
    {
      return *m_pData;
    }

    T const * operator-> () const
    {
      return m_pData;
    }

    shared_ptr& operator = (shared_ptr const & sp)
    {
      if (this != &sp) // Avoid self assignment
      {
        // Decrement the old reference count
        // if reference become zero delete the old data
        if (--(*m_pRefCount) == 0)
        {
          s_deleter(m_pData);
          delete m_pRefCount;
        }

        // Copy the data and reference pointer
        // and increment the reference count
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