#ifndef DGIDSERVER_H
#define DGIDSERVER_H

#include "Dg_set_pod.h"

namespace Dg
{
  template<typename T>
  class IDServer
  {
    class impl_Data
    {
    public:

      T   item;
      int count;
      int countMax;

      friend bool operator==(impl_Data const & p0, impl_Data const & p1) { return p0.item == p1.item; }
      friend bool operator!=(impl_Data const & p0, impl_Data const & p1) { return p0.item != p1.item; }
      friend bool operator<=(impl_Data const & p0, impl_Data const & p1) { return p0.item <= p1.item; }
      friend bool operator>=(impl_Data const & p0, impl_Data const & p1) { return p0.item >= p1.item; }
      friend bool operator< (impl_Data const & p0, impl_Data const & p1) { return p0.item < p1.item; }
      friend bool operator> (impl_Data const & p0, impl_Data const & p1) { return p0.item > p1.item; }
    };

  public:

    IDServer(int nIDs, T const * data);
    IDServer() {}
    ~IDServer() {}
    IDServer(IDServer<T> const & a_other) : m_IDs(a_other.m_IDs) {}
    IDServer & operator=(IDServer<T> const & a_other);

    void Init(int nIDs, T const * data);
    bool Get(T &);
    void Return(T);

  private:

    set_pod<impl_Data>  m_IDs;

  };

  template<typename T>
  void IDServer<T>::Init(int a_nIDs, T const * a_data)
  {
    m_IDs.clear();
    for (int i = 0; i < a_nIDs; ++i)
    {
      impl_Data elem;
      elem.item = a_data[i];
      elem.count = 0;
      elem.countMax = 1;

      int index(0);
      if (m_IDs.find(elem, index))
      {
        m_IDs[index].count++;
        m_IDs[index].countMax++;
      }
      else
      {
        m_IDs.insert_unique(elem);
      }
    }
  }

  template<typename T>
  IDServer<T>::IDServer(int a_nIDs, T const * a_data)
  {
    Init(a_nIDs, a_data);
  }

  template<typename T>
  IDServer<T> & IDServer<T>::operator=(IDServer<T> const & a_other)
  {
    if (this != &a_other)
    {
      m_IDs = a_other.m_IDs;
    }
    return *this;
  }

  template<typename T>
  bool IDServer<T>::Get(T & a_out)
  {
    for (int i = 0; i < m_IDs.size(); ++i)
    {
      if (m_IDs[i].count < m_IDs[i].countMax)
      {
        a_out = m_IDs[i].item;
        m_IDs[i].count++;
        return true;
      }
    }
    return false;
  }

  template<typename T>
  void IDServer<T>::Return(T a_val)
  {
    impl_Data elem;
    elem.item = a_val;
    elem.count = 0;
    elem.countMax = 1;

    int index(0);
    if (m_IDs.find(p, index))
    {
      if (m_IDs[index].count < m_IDs[index].countMax)
      {
        m_IDs[index].count++;
      }
    }
  }

}


#endif