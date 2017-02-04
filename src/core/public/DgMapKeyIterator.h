#ifndef MAPKEYITERATOR_H
#define MAPKEYITERATOR_H

#include <map>

namespace Dg
{
  template<typename Key, typename MappedType>
  class key_iterator : public std::map<Key, MappedType>::iterator
  {
  public:
    key_iterator() : std::map<Key, MappedType>::iterator() {}

    key_iterator(typename std::map<Key, MappedType>::iterator s)
      : std::map<Key, MappedType>::iterator(s)
    {}

    key_iterator & operator= (typename std::map<Key, MappedType>::iterator s)
    {
      std::map<Key, MappedType>::iterator::operator=(s);
      return *this;
    }

    Key * operator->() { return (Key * const)&(std::map<Key, MappedType>::iterator::operator->()->first); }
    Key operator*() { return std::map<Key, MappedType>::iterator::operator*().first; }
  };


  template<typename Key, typename MappedType>
  class const_key_iterator : public std::map<Key, MappedType>::const_iterator
  {
  public:
    const_key_iterator() : std::map<Key, MappedType>::const_iterator() {}

    const_key_iterator(typename std::map<Key, MappedType>::const_iterator s)
      : std::map<Key, MappedType>::const_iterator(s)
    {}

    const_key_iterator & operator= (typename std::map<Key, MappedType>::const_iterator s)
    {
      std::map<Key, MappedType>::const_iterator::operator=(s);
      return *this;
    }

    Key * operator->() { return (Key * const)&(std::map<Key, MappedType>::const_iterator::operator->()->first); }
    Key operator*() { return std::map<Key, MappedType>::const_iterator::operator*().first; }
  };

}

#endif // MAPKEYITERATOR_H
