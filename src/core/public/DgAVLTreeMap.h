#ifndef DGAVLTREEMAP_H
#define DGAVLTREEMAP_H

#include "impl/DgContainerBase.h"

#ifdef STD_COMPATIBLE
#include <iterator>
#define BASE_ITERATOR : public std::iterator<std::bidirectional_iterator_tag, T>
#else
#define BASE_ITERATOR
#endif

namespace Dg
{
  template<typename K, typename V>
  class AVLTreeMap : public ContainerBase
  {
    typedef size_t sizeType;
    static sizeType const s_nullValue = -1;

  private:

    struct KeyNode
    {
      K         key;
      sizeType  left;
      sizeType  right;
      sizeType  dataOffset;
    };

  public:

    class const_iterator BASE_ITERATOR
    {

    };

    class iterator BASE_ITERATOR
    {

    };

  public:

    AVLTreeMap();
    AVLTreeMap(sizeType);
    ~AVLTreeMap();

    AVLTreeMap(AVLTreeMap const &);
    AVLTreeMap & operator=(AVLTreeMap const &);

    AVLTreeMap(AVLTreeMap &&);
    AVLTreeMap & operator=(AVLTreeMap &&);

    sizeType size() const;
    bool empty() const;

    //Iterating will not be in order, but simply moving through the key/value pairs
    //as they appear in the underlying array.
    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

    void insert(K const &, V const &);
    void erase(K const &);
    void erase(const_iterator);
    bool exists(K const &) const;

    V & operator[](K const &);
    V const & operator[](K const &) const;

    V & at(K const &);
    V const & at(K const &) const;

  private:

    KeyNode *   m_pKeys;
    V *         m_pData;
    sizeType    m_nEntries;
  };
}

#endif