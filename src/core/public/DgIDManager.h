#ifndef DGIDSERVER_H
#define DGIDSERVER_H

#include "Dg_list_pod.h"
#include <stdio.h>

namespace Dg
{
  template<typename t_int>
  class IDManager
  {
  public:

    IDManager(t_int lower, t_int upper);
    IDManager() : m_bounds(static_cast<t_int>(1), static_cast<t_int>(1)) {}
    ~IDManager() {}
    IDManager(IDManager<t_int> const &);
    IDManager & operator=(IDManager<t_int> const &);

    void Init(t_int lower, t_int upper);

    //! @return 0 if no more IDs are available.
    t_int GetID();
    void ReturnID(t_int);

    //! @return false if id already in use.
    bool MarkAsUsed(t_int);
    bool IsUsed(t_int) const;

    //----------------------------------------------------------------------
    // DEBUGGING...
  public:

    void PrintState()
    {
      printf("State: < ");
      list_pod<Interval>::iterator it = m_intervals.begin();
      for (it; it != m_intervals.end(); ++it)
      {
        printf("[%i...%i] ", it->m_lower, it->m_upper);
      }
      printf(" >\n");
    }
    //----------------------------------------------------------------------

  private:

    class Interval
    {
    public:
      Interval(t_int a_lower, t_int a_upper)
        : m_lower(a_lower)
        , m_upper(a_upper)
      {}

      Interval()
        : m_lower(static_cast<t_int>(1))
        , m_upper(static_cast<t_int>(1))
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
      t_int m_lower;
      t_int m_upper;
    };
    
    list_pod<Interval>     m_intervals;
    Interval               m_bounds;
  };

  template<typename t_int>
  bool IDManager<t_int>::Interval::operator<(typename IDManager<t_int>::Interval const & a_other) const
  {
    return m_upper < a_other.m_lower;
  }

  template<typename t_int>
  bool IDManager<t_int>::Interval::operator>(typename IDManager<t_int>::Interval const & a_other) const
  {
    return m_upper > a_other.m_lower;
  }

  template<typename t_int>
  IDManager<t_int>::IDManager(IDManager<t_int> const & a_other)
    : m_intervals(a_other.m_intervals)
    , m_bounds(a_other.m_bounds)
  {
  }

  template<typename t_int>
  IDManager<t_int> & IDManager<t_int>::operator=(IDManager<t_int> const & a_other)
  {
    if (this != &a_other)
    {
      m_intervals = a_other.m_intervals;
      m_bounds = a_other.m_bounds;
    }
    return *this;
  }

  template<typename t_int>
  void IDManager<t_int>::Init(t_int a_lower, t_int a_upper)
  {
    m_intervals.clear();
    if (a_lower > a_upper) a_lower = a_upper;
    m_intervals.push_front(Interval(a_lower, a_upper));
  }

  template<typename t_int>
  IDManager<t_int>::IDManager(t_int a_lower, t_int a_upper)
    : m_bounds(a_lower, a_upper )
  {
    Init(a_lower, a_upper);
  }

  template<typename t_int>
  t_int IDManager<t_int>::GetID()
  {
    printf("\n\n");
    printf("GetID() - Before:\n");
    PrintState();

    if (m_intervals.empty())
    {
      printf("GetID() - EMPTY:\n");
      return static_cast<t_int>(0);
    }
 
    list_pod<Interval>::iterator it = m_intervals.begin();
    t_int result = it->m_lower;
    it->m_lower++;
    if (it->m_lower > it->m_upper)
    {
      m_intervals.erase(it);
    }

    printf("GetID() - after: \n");
    PrintState();
    printf("Result = %i\n", result);

    return result;
  }

  template<typename t_int>
  void IDManager<t_int>::ReturnID(t_int a_val)
  {
    printf("\n\n");
    printf("ReturnID(%i) - Before:\n", a_val);
    PrintState();

    //Check bounds
    if (a_val < m_bounds.m_lower
      || a_val > m_bounds.m_upper)
    {
      printf("ReturnID(%i) - ERROR\n", a_val);
      PrintState();

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
          if (a_val == (it->m_lower - static_cast<t_int>(1)))
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
          if (a_val == (itp->m_upper + static_cast<t_int>(1)))
          {
            itp->m_upper++;
            if (itp->m_upper == it->m_lower - static_cast<t_int>(1))
            {
              it->m_lower = itp->m_lower;
              m_intervals.erase(itp);
            }
          }
          else if (a_val == (it->m_lower - static_cast<t_int>(1)))
          {
            it->m_lower--;
            if (itp->m_upper == it->m_lower - static_cast<t_int>(1))
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
      if (m_intervals.empty() || a_val != (m_intervals.back().m_upper + static_cast<t_int>(1)))
      {
        m_intervals.push_back(Interval(a_val, a_val));
      }
      else
      {
        m_intervals.back().m_upper++;
      }
    }

    printf("ReturnID(%i) - After:\n", a_val);
    PrintState();

  }

  template<typename t_int>
  bool IDManager<t_int>::MarkAsUsed(t_int a_val)
  {
    printf("\n\n");
    printf("MarkAsUsed(%i) - Before:\n", a_val);
    PrintState();

    //Check bounds
    if (a_val < m_bounds.m_lower
      || a_val > m_bounds.m_upper
      || m_intervals.empty())
    {

      printf("MarkAsUsed(%i) - ERROR\n", a_val);
      PrintState();

      return false;
    }

    list_pod<Interval>::iterator it = m_intervals.begin();
    bool found = false;
    for (it; it != m_intervals.end(); ++it)
    {
      if (a_val <= it->m_upper)
      {
        found = true;
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
          m_intervals.insert(it, Interval(it->m_lower, a_val - static_cast<t_int>(1) ));
          it->m_lower = a_val + static_cast<t_int>(1);
        }
        else
        {
          found = false;
        }
        break;
      }
    }
    printf("MarkAsUsed(%i) - After:\n", a_val);
    PrintState();

    return found;
  }

  template<typename t_int>
  bool IDManager<t_int>::IsUsed(t_int a_val) const
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