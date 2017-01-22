#ifndef HANDLEBASE_H
#define HANDLEBASE_H

namespace HandleEnum
{
  enum
  {
    INVALID_HANDLE  = -1,
    UNBOUNDED       = -2
  };
}

template<typename T, int ID = 0>
class HandleBase
{
public:

  HandleBase(): m_val(T(HandleEnum::INVALID_HANDLE)) {}
  HandleBase(T a_val): m_val(a_val) {}
  ~HandleBase(){}

  HandleBase(HandleBase const & h): m_val(h.m_val) {}
  HandleBase & operator=(HandleBase const & h)
  {
    m_val = h.m_val;
    return *this;
  }

  bool IsValid() const {return m_val != HandleEnum::INVALID_HANDLE;}

  bool operator==(HandleBase const & h) const {return h.m_val == m_val;}
  bool operator!=(HandleBase const & h) const {return h.m_val != m_val;}
  bool operator< (HandleBase const & h) const {return h.m_val < m_val;}
  bool operator> (HandleBase const & h) const {return h.m_val > m_val;}
  bool operator<=(HandleBase const & h) const {return h.m_val <= m_val;}
  bool operator>=(HandleBase const & h) const {return h.m_val >= m_val;}

  operator T() const {return m_val;}

private:
  T m_val;
};

#endif // HANDLEBASE_H
