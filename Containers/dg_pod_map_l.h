//! @file dg_pod_map_l.h
//!
//! @author: Frank B. Hart
//! @date 2/5/2015
//!
//! Class declaration: map_l_l

#ifndef DG_POD_map_l_L_H
#define DG_POD_map_l_L_H

#include <exception>

#include "common.h"

namespace Dg
{
  namespace POD
  {
    //! @ingroup Containers
    //!
    //! @class map_l
    //!
    //! Ordered map_lped list. 
    //!
    //! Assumed types are POD, so no construction / assignment operators called
    //!
    //! @author Frank B. Hart
    //! @date 2/5/2015
    template<typename U, typename T>
    class map_l
    {
      //Internal container which stores the m_data
      struct DataContainer
      {
        DataContainer() : next(nullptr), prev(nullptr){}

        DataContainer* next;
        DataContainer* prev;
        T item;
      };

      //Internal container which stores key and pointer to m_data
      struct KeyContainer
      {
        KeyContainer() : item_ptr(nullptr) {}

        U key;
        DataContainer* item_ptr;
      };

    public:

      //! Constructor 
      //! If the constructor fails to allocate the map_l, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      map_l();

      //! Initialize map_l with to a capacity. 
      //! If the constructor fails to allocate the map_l, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      map_l(unsigned int);
      ~map_l();

      //! Copy constructor.
      map_l(map_l const &);

      //! Assigns new contents to the container, replacing its current content.
      map_l& operator= (map_l const &);

      //! Returns a reference to the \a i<SUP>th</SUP> element in the map_l. 
      //! This function does not perform a range check.
      T&       operator[](unsigned int i)				{ return m_data[i].item; }

      //! Returns a const reference to the \a i<SUP>th</SUP> element in the map_l. 
      //! This function does not perform a range check.
      const T& operator[](unsigned int i) const { return m_data[i].item; }

      //! Return number of elements in the map_l.
      int      size()			                const	{ return m_currentSize; }

      //! Returns whether the map_l is empty.
      bool     empty()		                const	{ return m_currentSize == 0; }

      //! Returns number of elements the map_l can hold before resizing.
      int      max_size()		              const	{ return m_arraySize; }

      //! Returns the key of the ith element in the map_l.
      //! This function does not perform a range check.
      U        QueryKey(int i)		          const { return m_data[i].key; }

      //! Searches the map_l for an element with a key equivalent to \a k.
      //! \return True if the element was found with \a index being set to the 
      //!         index of the element inthe map_l. False if not found with \a index
      //!         set to one lower to where \a k would be.
      //! \param lower Set a low bound to the search sublist.
      bool find(U k, int& index, int lower = 0) const;			 //Use binary search

      //! Searches the map_l for an element with a key equivalent to \a k.
      //! \return True if the element was found with \a index being set to the 
      //!         index of the element inthe map_l. False if not found with \a index
      //!         set to one lower to where \a k would be.
      //! \param lower Set a low bound to the search sublist.
      //! \param upper Set an upper bound to the search sublist.
      bool find(U k, int& index, int lower, int upper) const;	//Use binary search

      //! Extends the container by inserting new elements, effectively increasing 
      //! the container size by the number of elements inserted.
      //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      //! \return False if key already exists in the map_l.
      bool insert(U k, T t);

      //! Set element with key \a k, with value \a t.
      //! \return True if key found.
      bool set(U k, T t);

      //! Removes the item in the map_l with key \a k.
      void erase(U k);

      //! Clear all items from the map_l, retains allocated memory.
      void clear();

      //! Resize the map. The content of the map is preserved up 
      //! to the lesser of the new and old sizes.
      //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      void resize(unsigned int);

      //! Doubles the memory allocated to the map_l. Retains all m_data.
      //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      void extend();

      //! Clears the map_l, reallocates memory to the map_l.
      void reset();

    private:
      //Data members
      DataContainer* m_data;
      KeyContainer* m_keys;

      int m_arraySize;
      int m_currentSize;

      //Next free container to fill
      DataContainer* m_nextPtr;

    private:
      void init(map_l const &);
    };


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::map_l()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    map_l<U, T>::map_l()
      : m_data(nullptr)
      , m_keys(nullptr)
      , m_nextPtr(nullptr)
      , m_arraySize(0)
      , m_currentSize(0)
    {
      resize(DG_MAP_DEFAULT_SIZE);

    }	//End: map_l::map_l()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::map_l()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    map_l<U, T>::map_l(unsigned int a_size)
      : m_data(nullptr)
      , m_keys(nullptr)
      , m_nextPtr(nullptr)
      , m_arraySize(0)
      , m_currentSize(0)
    {
      resize(a_size);

    }	//End: map_l::map_l()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::~map_l()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    map_l<U, T>::~map_l()
    {
      free(m_keys);
      free(m_data);

    }	//End: map_l::~map_l()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::init()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map_l<U, T>::init(map_l const & a_other)
    {
      resize(a_other.m_arraySize);

      for (int i = 0; i < a_other.m_currentSize; ++i)
      {
        insert(a_other.m_keys[i].item_ptr->item, a_other.m_keys[i].key);
      }

    }	//End: map_l::init()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::map_l()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    map_l<U, T>::map_l(map_l const & a_other)
      : m_data(nullptr)
      , m_keys(nullptr)
      , m_nextPtr(nullptr)
      , m_arraySize(0)
      , m_currentSize(0)
    {
      init(a_other);

    }	//End: map_l::map_l()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::operator=()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    map_l<U, T>& map_l<U, T>::operator=(map_l const & a_other)
    {
      if (this == &a_other)
        return *this;

      init(a_other);

      return *this;

    }	//End: map_l::operator=()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::resize()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map_l<U, T>::resize(unsigned int a_newSize)
    {
      //Delete old data
      free(m_keys);
      free(m_data);

      //Adjust sizes
      m_arraySize = a_newSize;
      m_currentSize = 0;

      //Create new arrays
      m_data = static_cast<DataContainer *>(malloc((a_newSize + 2) * sizeof(DataContainer)));
      if (m_data == nullptr)
      {
        throw std::bad_alloc;
      }

      m_keys = static_cast<DataContainer *>(malloc((a_newSize) * sizeof(KeyContainer)));
      if (m_keys == nullptr)
      {
        throw std::bad_alloc;
      }

      //Set pointers
      m_nextPtr = &m_data[1];
      for (int i = 1; i < array_size; ++i)
        data[i].next = &data[i + 1];

      data[0].next = &data[array_size + 1];
      data[array_size + 1].prev = &data[0];

    }	//End: map_l::resize()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::find()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    bool map_l<U, T>::find(U a_key, int& a_index, int a_lower) const
    {
      return find(a_key, a_index, a_lower, (m_currentSize - 1));

    }	//End: map_l::find()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::find()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    bool map_l<U, T>::find(U a_key, int& a_index, int a_lower, int a_upper) const
    {
      //Check bounds
      a_lower = (a_lower > 0) ? a_lower : 0;
      a_upper = (a_upper < m_currentSize - 1) ? a_upper : m_currentSize - 1;

      while (a_lower <= a_upper)
      {
        // calculate the midpoint for roughly equal partition
        a_index = ((a_upper + a_lower) >> 1);

        // determine which subarray to search
        if (m_data[a_index].a_key < a_key)
          // change min index to search upper subarray
          a_lower = a_index + 1;
        else if (m_data[a_index].a_key > a_key)
          // change max index to search lower subarray
          a_upper = a_index - 1;
        else
          // key found at index index
          return true;
      }

      //Set index closest (but lower) to key
      a_index = a_lower - 1;
      return false;

    }	//End: map_l::find()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::extend()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map_l<U, T>::extend()
    {
      //Calculate new size
      int new_size = (m_arraySize << 1);

      //overflow, map_l full
      if (new_size <= m_arraySize)
      {
        throw std::overflow_error;
      }

      m_data = static_cast<Container*>(realloc(m_data, sizeof(Container) * new_size));
      if (m_data == nullptr)
      {
        throw std::bad_alloc;
      }

      m_arraySize = new_size;

    }	//End: map_l::extend()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::insert()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    bool map_l<U, T>::insert(U a_key, T a_item)
    {
      //Find the index to insert to
      int index;
      if (find(a_key, index))
        return false;	//element already exists

      //Range check
      if (m_currentSize == m_arraySize)
        extend();

      //shift all RHS objects to the right by one.
      memmove(&m_data[index + 2], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(Container));

      index++;

      memcpy(&m_data[index].key, &a_key, sizeof(a_key));
      memcpy(&m_data[index].item, &a_item, sizeof(a_item));

      m_currentSize++;

      return true;

    }	//End: map_l::insert()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::erase()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map_l<U, T>::erase(U a_key)
    {
      //Find the index
      int index;
      if (!find(a_key, index))
      {
        return;
      }

      memmove(&m_data[index], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(Container));

      m_currentSize--;

    }	//End: map_l::erase()


    //--------------------------------------------------------------------------------
    //	@	map_l<U,T>::set()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    bool map_l<U, T>::set(U a_key, T a_item)
    {
      //Find the index to insert to
      int index;
      if (!find(a_key, index))
      {
        return false;	//element does not exist
      }


      memcpy(&m_data[index].item, &a_item, sizeof(a_item));

      return true;

    }	//End: map_l::set()


    //--------------------------------------------------------------------------------
    //	@	Dgmap_l<U,T>::reset()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map_l<U, T>::reset()
    {
      resize(DG_map_l_DEFAULT_SIZE);

    }	//End: map_l::reset()


    //--------------------------------------------------------------------------------
    //	@	Dgmap_l<U,T>::clear()
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map_l<U, T>::clear()
    {
      m_currentSize = 0;

    }	//End: map_l::clear()
  }
};

#endif