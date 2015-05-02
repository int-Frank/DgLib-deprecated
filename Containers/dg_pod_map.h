//================================================================================
// @ map.h
// 
// Class: map
//
// Ordered mapped list. Objects are inserted (in order) with some key. 
// Similar to a DgMap however m_data is stored with the key. Best used for
// small objects and/or objects with cheap assignment operators. Else
// use a DgMap<U,T>. Only unique keys are allowed.
//
// Assumed types are POD, so no construction / assignment operators called
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef DG_POD_MAP_S_H
#define DG_POD_MAP_S_H

#include <exception>

#define DG_MAP_DEFAULT_SIZE 1024

namespace Dg
{
  namespace POD
  {
    //--------------------------------------------------------------------------------
    //	@	map<U, T>:	U: key
    //						T: m_data
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    class map
    {
      //Internal container which stores the m_data
      struct Container
      {
        U key;
        T item;
      };

    public:

      //! Constructor 
      //! If the constructor fails to allocate the map, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      map();

      //! Initialize map with to a capacity. 
      //! If the constructor fails to allocate the map, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      map(unsigned int);
      ~map();

      //! Copy constructor.
      map(map const &);

      //! Assigns new contents to the container, replacing its current content.
      map& operator= (map const &);

      //! Returns a reference to the \a i<SUP>th</SUP> element in the map. 
      //! This function does not perform a range check.
      T&       operator[](unsigned int i)				{ return m_data[i].item; }

      //! Returns a const reference to the \a i<SUP>th</SUP> element in the map. 
      //! This function does not perform a range check.
      const T& operator[](unsigned int i) const { return m_data[i].item; }

      //! Return number of elements in the map.
      int      size()			                const	{ return m_currentSize; }

      //! Returns whether the map is empty.
      bool     empty()		                const	{ return m_currentSize == 0; }

      //! Returns number of elements the map can hold before resizing.
      int      max_size()		              const	{ return m_arraySize; }

      //! Returns the key of the ith element in the map.
      //! This function does not perform a range check.
      U        QueryKey(int i)		          const { return m_data[i].key; }

      //! Searches the map for an element with a key equivalent to \a k.
      //! \return True if the element was found with \a index being set to the 
      //!         index of the element inthe map. False if not found with \a index
      //!         set to one lower to where \a k would be.
      //! \param lower Set a low bound to the search sublist.
      bool find(U k, int& index, int lower = 0) const;			 //Use binary search

      //! Searches the map for an element with a key equivalent to \a k.
      //! \return True if the element was found with \a index being set to the 
      //!         index of the element inthe map. False if not found with \a index
      //!         set to one lower to where \a k would be.
      //! \param lower Set a low bound to the search sublist.
      //! \param upper Set an upper bound to the search sublist.
      bool find(U k, int& index, int lower, int upper) const;	//Use binary search

      //! Extends the container by inserting new elements, effectively increasing 
      //! the container size by the number of elements inserted.
      //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      //! \return False if key already exists in the map.
      bool insert(U k, T t);

      //! Set element with key \a k, with value \a t.
      //! \return True if key found.
      bool set(U k, T t);

      //! Removes the item in the map with key \a k.
      void erase(U k);

      //! Clear all items from the map, retains allocated memory.
      void clear();

      //! Resize the map. All data will be lost.
      //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      void resize(unsigned int);

      //! Doubles the memory allocated to the map. Retains all data.
      //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      void extend();

      //! Clears the map, reallocates memory to the map.
      void reset();

    private:
      //Data members
      Container* m_data;

      int m_arraySize;
      int m_currentSize;

    private:
      void init(map const &);
    };


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::map()
    //--------------------------------------------------------------------------------
    //		Constructor
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    map<U, T>::map()
      : m_data(nullptr), m_arraySize(0), m_currentSize(0)
    {
      resize(DG_MAP_DEFAULT_SIZE);

    }	//End: map::map()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::map()
    //--------------------------------------------------------------------------------
    //		Copy Constructor
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    map<U, T>::map(unsigned int a_size)
      : m_data(nullptr), m_arraySize(0), m_currentSize(0)
    {
      m_data = static_cast<Container *>(malloc(sizeof(Container) * a_size));

      if (m_data == nullptr)
      {
        throw std::bad_alloc;
      }

      m_arraySize = a_size;
      m_currentSize = 0;

    }	//End: map::map()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::~map()
    //--------------------------------------------------------------------------------
    //		Destructor
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    map<U, T>::~map()
    {
      free(m_data);

    }	//End: map::~map()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::init()
    //--------------------------------------------------------------------------------
    //		Initialise map to another.
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map<U, T>::init(map const & a_other)
    {
      resize(a_other.m_arraySize);

      memcpy(m_data, a_other.m_data, a_other.m_currentSize * sizeof(Container));

      m_currentSize = a_other.m_currentSize;

    }	//End: map::init()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::map()
    //--------------------------------------------------------------------------------
    //		Copy constructor
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    map<U, T>::map(map const & a_other) :
      m_data(nullptr), m_arraySize(0), m_currentSize(0)
    {
      init(a_other);

    }	//End: map::map()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::operator=()
    //--------------------------------------------------------------------------------
    //		Assignment
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    map<U, T>& map<U, T>::operator=(map const & a_other)
    {
      if (this == &a_other)
        return *this;

      init(a_other);

      return *this;

    }	//End: map::operator=()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::resize()
    //--------------------------------------------------------------------------------
    //		Resize map, erases all m_data before resize.
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map<U, T>::resize(unsigned int a_newSize)
    {
      free(m_data);
      m_data = static_cast<Container *>(malloc(sizeof(Container) * a_newSize));

      if (m_data == nullptr)
      {
        throw std::bad_alloc;
      }

      m_arraySize = a_newSize;
      m_currentSize = 0;

    }	//End: map::resize()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::find()
    //--------------------------------------------------------------------------------
    //		Find a value in the map, uses a binary search algorithm
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    bool map<U, T>::find(U a_key, int& a_index, int a_lower) const
    {
      return find(a_key, a_index, a_lower, (m_currentSize - 1));

    }	//End: map::find()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::find()
    //--------------------------------------------------------------------------------
    //		Find a value in the map within a range, uses a binary search algorithm
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    bool map<U, T>::find(U a_key, int& a_index, int a_lower, int a_upper) const
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

    }	//End: map::find()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::extend()
    //--------------------------------------------------------------------------------
    //		Extend the map by a factor of 2, keeps all m_data.
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map<U, T>::extend()
    {
      //Calculate new size
      int new_size = (m_arraySize << 1);

      //overflow, map full
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

    }	//End: map::extend()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::insert()
    //--------------------------------------------------------------------------------
    //		Insert an element into the map
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    bool map<U, T>::insert(U a_key, T a_item)
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

    }	//End: map::insert()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::erase()
    //--------------------------------------------------------------------------------
    //		Remove an element from the map
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map<U, T>::erase(U a_key)
    {
      //Find the index
      int index;
      if (!find(a_key, index))
      {
        return;
      }

      memmove(&m_data[index], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(Container));

      m_currentSize--;

    }	//End: map::erase()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::set()
    //--------------------------------------------------------------------------------
    //		Sets an element to a new value
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    bool map<U, T>::set(U a_key, T a_item)
    {
      //Find the index to insert to
      int index;
      if (!find(a_key, index))
      {
        return false;	//element does not exist
      }
        

      memcpy(&m_data[index].item, &a_item, sizeof(a_item));

      return true;

    }	//End: map::set()


    //--------------------------------------------------------------------------------
    //	@	DgMap<U,T>::reset()
    //--------------------------------------------------------------------------------
    //		Reset size to 1
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map<U, T>::reset()
    {
      resize(DG_MAP_DEFAULT_SIZE);

    }	//End: map::reset()


    //--------------------------------------------------------------------------------
    //	@	DgMap<U,T>::clear()
    //--------------------------------------------------------------------------------
    //		Set the number of elements to zero
    //--------------------------------------------------------------------------------
    template<typename U, typename T>
    void map<U, T>::clear()
    {
      m_currentSize = 0;

    }	//End: map::clear()
  }
};

#endif