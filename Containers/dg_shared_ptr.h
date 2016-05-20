#ifndef DG_SHARED_PTR
#define DG_SHARED_PTR

namespace Dg
{
  template < typename T > 
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
        delete m_pData;
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

    shared_ptr<T>& operator = (shared_ptr<T> const & sp)
    {
      if (this != &sp) // Avoid self assignment
      {
        // Decrement the old reference count
        // if reference become zero delete the old data
        if (--(*m_pRefCount) == 0)
        {
          delete m_pData;
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
    T *          m_pData;       // pointer
    size_t *     m_pRefCount;
  };
}
#endif