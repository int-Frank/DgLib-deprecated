#ifndef CIRCULATOR_CONST_H
#define CIRCULATOR_CONST_H

#include <iterator>

template<typename Iterator, typename T>
class Circulator_const : public std::iterator<std::bidirectional_iterator_tag, T>
{
public:

  Circulator_const(Iterator a_first, Iterator a_last)
    : m_it(a_first)
    , m_first(a_first)
    , m_last(a_last)
  {

  }

  Circulator_const(){}
  ~Circulator_const(){}

  Circulator_const(Circulator_const const & a_other)
    : m_it(a_other.m_it)
    , m_first(a_other.m_first)
    , m_last(a_other.m_last)
  {

  }

  Circulator_const& operator= (Circulator_const const & a_other)
  {
    if (this != &a_other)
    {
      m_it = a_other.m_it;
      m_first = a_other.m_first;
      m_last = a_other.m_last;
    }
    return *this;
  }

  bool operator==(Circulator_const const & a_other) const
  {
    return a_other.m_it == m_it
        && a_other.m_first == m_first
        && a_other.m_last == a_other.m_last;
  }

  bool operator!=(Circulator_const const & a_other) const
  {
    return a_other.m_it != m_it
        || a_other.m_first != m_first
        || a_other.m_last != a_other.m_last;
  }

  Circulator_const& operator++()
  {
    if (m_it == m_last)
    {
      m_it = m_first;
    }
    else
    {
      ++m_it;
    }
    return *this;
  }

  Circulator_const operator++(int)
  {
   Circulator_const result(*this);
   ++(*this);
   return result;
  }

  Circulator_const& operator--()
  {
    if (m_it == m_first)
    {
      m_it = m_last;
    }
    else
    {
      --m_it;
    }
    return *this;
  }

  Circulator_const operator--(int)
  {
    Circulator_const result(*this);
    ++(*this);
    return result;
  }

  T const * operator->() const
  {
    return m_it.operator->();
  }

  T const & operator*() const
  {
    return m_it.operator*();
  }

private:

  Iterator            m_it;
  Iterator const      m_first;
  Iterator const      m_last;
};

#endif // CIRCULATOR_CONST_H
