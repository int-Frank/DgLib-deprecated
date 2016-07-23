//! @file DgIDManager.h
//!
//! @author: Frank B. Hart
//! @date 22/07/2016
//!
//! Class declaration: IDManager

#ifndef DGIDSERVER_H
#define DGIDSERVER_H

#include "Dg_list_pod.h"

//TODO Documentation, tests
namespace Dg
{
  //! @ingroup DgUtility
  //!
  //! @class IDManager
  //!
  //! The IDManager serves unique ID's with a certain range.
  //!
  //! @author Frank Hart
  //! @date 23/07/2016
  template<typename T>
  class IDManager
  {
  public:

    IDManager();
    IDManager(T lower, T upper);
    ~IDManager() {}
    IDManager(IDManager<T> const &);
    IDManager & operator=(IDManager<T> const &);

    void Init(T lower, T upper);

    //! @return 0 if no more IDs are available.
    T GetID();
    void ReturnID(T);

    //! @return false if id already in use.
    bool MarkAsUsed(T);
    bool IsUsed(T) const;

  private:

    class Interval
    {
    public:
      Interval(T a_lower, T a_upper)
        : m_lower(a_lower)
        , m_upper(a_upper)
      {}

      Interval()
        : m_lower(static_cast<T>(1))
        , m_upper(static_cast<T>(1))
      {}

      Interval(Interval const & a_other)
        : m_lower(a_other.m_lower)
        , m_upper(a_other.m_upper)
      {}

      Interval & operator=(Interval const & a_other)
      {
        m_lower = a_other.m_lower;
        m_upper = a_other.m_upper;
        return *this;
      }

      bool IsOne() const { return m_lower == m_upper; }

      bool operator<(Interval const &) const;
      bool operator>(Interval const &) const;

    public:
      T m_lower;
      T m_upper;
    };
    
    list_pod<Interval>     m_intervals;
    Interval               m_bounds;
  };

  template<typename T>
  bool IDManager<T>::Interval::operator<(typename IDManager<T>::Interval const & a_other) const
  {
    return m_upper < a_other.m_lower;
  }

  template<typename T>
  bool IDManager<T>::Interval::operator>(typename IDManager<T>::Interval const & a_other) const
  {
    return m_upper > a_other.m_lower;
  }

  template<typename T>
  IDManager<T>::IDManager()
    : m_bounds(static_cast<T>(1), static_cast<T>(1))
  {
    m_intervals.push_back(m_bounds);
  }

  template<typename T>
  IDManager<T>::IDManager(T a_lower, T a_upper)
    : m_bounds(a_lower, a_upper)
  {
    Init(a_lower, a_upper);
  }

  template<typename T>
  IDManager<T>::IDManager(IDManager<T> const & a_other)
    : m_intervals(a_other.m_intervals)
    , m_bounds(a_other.m_bounds)
  {
  }

  template<typename T>
  IDManager<T> & IDManager<T>::operator=(IDManager<T> const & a_other)
  {
    if (this != &a_other)
    {
      m_intervals = a_other.m_intervals;
      m_bounds = a_other.m_bounds;
    }
    return *this;
  }

  template<typename T>
  void IDManager<T>::Init(T a_lower, T a_upper)
  {
    m_intervals.clear();
    if (a_lower > a_upper) a_lower = a_upper;
    m_bounds.m_lower = a_lower;
    m_bounds.m_upper = a_upper;
    m_intervals.push_front(m_bounds);
  }

  template<typename T>
  T IDManager<T>::GetID()
  {
    if (m_intervals.empty())
    {
      return static_cast<T>(0);
    }
 
    list_pod<Interval>::iterator it = m_intervals.begin();
    T result = it->m_lower;
    it->m_lower++;
    if (it->m_lower > it->m_upper)
    {
      m_intervals.erase(it);
    }

    return result;
  }

  template<typename T>
  void IDManager<T>::ReturnID(T a_val)
  {
    //Check bounds
    if (a_val < m_bounds.m_lower || a_val > m_bounds.m_upper)
    {
      return;
    }

    list_pod<Interval>::iterator it = m_intervals.begin();
    bool found = false;
    for (it; it != m_intervals.end(); ++it)
    {
      if (a_val <= it->m_upper)
      {
        found = true;
        if (it == m_intervals.begin())
        {
          if (a_val == (it->m_lower - static_cast<T>(1)))
          {
            it->m_lower--;
          }
          else
          {
            m_intervals.insert(it, Interval(a_val, a_val));
          }
        }
        else
        {
          list_pod<Interval>::iterator itp(it); --itp;
          if (a_val == (itp->m_upper + static_cast<T>(1)))
          {
            itp->m_upper++;
            if (itp->m_upper == it->m_lower - static_cast<T>(1))
            {
              it->m_lower = itp->m_lower;
              m_intervals.erase(itp);
            }
          }
          else if (a_val == (it->m_lower - static_cast<T>(1)))
          {
            it->m_lower--;
            if (itp->m_upper == it->m_lower - static_cast<T>(1))
            {
              it->m_lower = itp->m_lower;
              m_intervals.erase(itp);
            }
          }
          else
          {
            m_intervals.insert(it, Interval(a_val, a_val));
          }
        }
        break;
      }
    }

    //We're at the end
    if (!found)
    {
      if (m_intervals.empty() || a_val != (m_intervals.back().m_upper + static_cast<T>(1)))
      {
        m_intervals.push_back(Interval(a_val, a_val));
      }
      else
      {
        m_intervals.back().m_upper++;
      }
    }
  }

  template<typename T>
  bool IDManager<T>::MarkAsUsed(T a_val)
  {
    list_pod<Interval>::iterator it = m_intervals.begin();
    bool good = false;
    for (it; it != m_intervals.end(); ++it)
    {
      if (a_val <= it->m_upper)
      {
        good = true;
        if (a_val == it->m_lower)
        {
          if (it->IsOne()) it = m_intervals.erase(it);
          else it->m_lower++;
        }
        else if (a_val == it->m_upper)
        {
          if (it->IsOne()) it = m_intervals.erase(it);
          else it->m_upper--;
        }
        else if (a_val > it->m_lower)
        {
          m_intervals.insert(it, Interval(it->m_lower, a_val - static_cast<T>(1) ));
          it->m_lower = a_val + static_cast<T>(1);
        }
        else //Already marked as used
        {
          good = false;
        }
        break;
      }
    }
    return good;
  }

  template<typename T>
  bool IDManager<T>::IsUsed(T a_val) const
  {
    //Check bounds
    if (a_val < m_bounds.m_lower || a_val > m_bounds.m_upper)
    {
      return false;
    }

    list_pod<Interval>::const_iterator it = m_intervals.cbegin();
    for (it; it != m_intervals.cend(); ++it)
    {
      if (a_val <= it->m_upper)
      {
        return a_val < it->m_lower;
      }
    }
    return true;
  }
}


#endif