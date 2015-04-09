//================================================================================
// @ set.h
// 
// Class: set
//
// An ordered array. Elements are inserted in order. Uses contiguous memory to 
// store m_data, therefore is best used for very small m_data types (ie literals) 
// and types with cheap assignment operators.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef DG_SET_H
#define DG_SET_H

namespace Dg
{
  namespace POD
  {
    //--------------------------------------------------------------------------------
    //	@	set<T>:	T: m_data type
    //--------------------------------------------------------------------------------
    template<class T>
    class set
    {
    public:
      //Constructor / destructor
      set();
      set(unsigned);
      ~set();

      //Copy operations
      set(const set&);
      set& operator= (const set&);

      //Accessors

      //Get item at position
      const T& operator[](unsigned i) const { return m_data[i]; }
      int size()			const			{ return m_currentSize; }
      bool empty()		const			{ return m_currentSize == 0; }
      int max_size()		const			{ return m_arraySize; }

      //Returns false if not found, however, index will be set to
      //the lower index just before where input key would be.
      bool find(const T&, int& index, int lower = 0) const;			//Use binary search
      bool find(const T&, int& index, int lower, int upper) const;	//Use binary search

      //Manipulators
      bool insert(const T&);
      bool insert_unique(const T&);
      void erase(const T&);
      void erase_all(const T&);
      void clear();
      void resize(unsigned);
      void extend();
      void reset();				//Reset m_data array to size 1.

    private:
      //m_data members
      T* m_data;

      int m_arraySize;
      int m_currentSize;

    private:
      void init(const set&);
    };


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::DgOrgeredArray()
    //--------------------------------------------------------------------------------
    //		Constructor
    //--------------------------------------------------------------------------------
    template<class T>
    set<T>::set()
      : m_data(nullptr), m_arraySize(0), m_currentSize(0)
    {
    }	//End: set::set()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::DgOrgeredArray()
    //--------------------------------------------------------------------------------
    //		Constructor
    //--------------------------------------------------------------------------------
    template<class T>
    set<T>::set(unsigned new_size)
      : m_data(nullptr), m_arraySize(0), m_currentSize(0)
    {
      resize(new_size);

    }	//End: set::set()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::~DgOrgeredArray()
    //--------------------------------------------------------------------------------
    //		Destructor
    //--------------------------------------------------------------------------------
    template<class T>
    set<T>::~set()
    {
      //Free memory
      free( m_data );

    }	//End: set::~set()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::DgOrgeredArray()
    //--------------------------------------------------------------------------------
    //		Initialise set to another.
    //--------------------------------------------------------------------------------
    template<class T>
    void set<T>::init(const set& other)
    {
      //Resize lists
      int sze = (other.m_arraySize>0) ? other.m_arraySize : 1;
      resize(sze);

      memcpy(m_data, other.m_data, other.m_currentSize * sizeof(T));

      m_currentSize = other.m_currentSize;

    }	//End: set::init()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::DgOrgeredArray()
    //--------------------------------------------------------------------------------
    //		Copy constructor
    //--------------------------------------------------------------------------------
    template<class T>
    set<T>::set(const set& other) :
      m_data(nullptr), m_arraySize(0), m_currentSize(0)
    {
      init(other);

    }	//End: set::set()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::operator=()
    //--------------------------------------------------------------------------------
    //		Assignment
    //--------------------------------------------------------------------------------
    template<class T>
    set<T>& set<T>::operator=(const set& other)
    {
      if (this == &other)
        return *this;

      init(other);

      return *this;

    }	//End: set::operator=()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::resize()
    //--------------------------------------------------------------------------------
    //		Resize map, erases all m_data before resize.
    //--------------------------------------------------------------------------------
    template<class T>
    void set<T>::resize(unsigned val)
    {
      //Delete old m_data
      free( m_data );
      m_data = (T *)malloc(sizeof(T) * val);

      m_arraySize = val;
      m_currentSize = 0;

    }	//End: set::resize()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::find()
    //--------------------------------------------------------------------------------
    //		Find a value in the list, uses a binary search algorithm
    //--------------------------------------------------------------------------------
    template<class T>
    bool set<T>::find(const T& item, int& index, int lower) const
    {
      return find(item, index, lower, (m_currentSize - 1));

    }	//End: set::find()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::find()
    //--------------------------------------------------------------------------------
    //		Find a value in the list within a range, uses a binary search algorithm
    //--------------------------------------------------------------------------------
    template<class T>
    bool set<T>::find(const T& item, int& index, int lower, int upper) const
    {
      //Check bounds
      lower = (lower>0) ? lower : 0;
      upper = (upper<m_currentSize - 1) ? upper : m_currentSize - 1;

      while (lower <= upper)
      {
        // calculate the midpoint for roughly equal partition
        index = ((upper + lower) >> 1);

        // determine which subarray to search
        if (m_data[index] < item)
          // change min index to search upper subarray
          lower = index + 1;
        else if (m_data[index] > item)
          // change max index to search lower subarray
          upper = index - 1;
        else
          // key found at index index
          return true;
      }

      //Set index closest (but lower) to key
      index = lower - 1;
      return false;

    }	//End: set::find()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::extend()
    //--------------------------------------------------------------------------------
    //		Extend the array by a factor of 1.5, keeps all m_data.
    //--------------------------------------------------------------------------------
    template<class T>
    void set<T>::extend()
    {
      //Calculate new size (1.5 * m_arraySize + 1)
      int new_size = (m_arraySize << 1);

      //Create new arrays
      T * new_data = (T *)malloc(sizeof(T) * new_size);

      memcpy(new_data, m_data, (sizeof(T) * m_currentSize));

      free(m_data);
      m_data = new_data;
      m_arraySize = new_size;

    }	//End: set::extend()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::insert()
    //--------------------------------------------------------------------------------
    //		Insert an element into the list
    //--------------------------------------------------------------------------------
    template<class T>
    bool set<T>::insert(const T& item)
    {
      //Find the index to insert to
      int index;
      find(item, index);

      //Range check
      if (m_currentSize == m_arraySize)
        extend();

      //shift all RHS objects to the right by one.
      memmove(&m_data[index + 2], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));

      index++;

      memcpy(&m_data[index], &item, sizeof(T));

      m_currentSize++;

      return true;

    }	//End: set::insert()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::insert_unique()
    //--------------------------------------------------------------------------------
    //		Insert an element into the list, only if it does not yet exist
    //--------------------------------------------------------------------------------
    template<class T>
    bool set<T>::insert_unique(const T& item)
    {
      //Find the index to insert to
      int index;
      if (find(item, index))
        return false;

      //Range check
      if (m_currentSize == m_arraySize)
        extend();

      //shift all RHS objects to the right by one.
      memmove(&m_data[index + 2], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));

      index++;

      memcpy(&m_data[index], &item, sizeof(T));

      m_currentSize++;

      return true;

    }	//End: set::insert_unique()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::erase()
    //--------------------------------------------------------------------------------
    //		Find and removes one of this element from the list.
    //--------------------------------------------------------------------------------
    template<class T>
    void set<T>::erase(const T& item)
    {
      //Find the index
      int index;
      if (!find(item, index))
        return;	//element not found

      //Remove elements
      memmove(&m_data[index], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));

      //Adjust m_currentSize
      m_currentSize--;

      return true;

    }	//End: set::erase()
    

    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::erase_all()
    //--------------------------------------------------------------------------------
    //		Find and removes all of this element from the list.
    //--------------------------------------------------------------------------------
    template<class T>
    bool set<T>::erase_all(const T& item)
    {
      //Find the index
      int lower, upper;
      if (!find(item, lower))
        return false;	//element not found

      //Initial upper bounds
      upper = lower + 1;

      //Find lower bounds
      while (lower >= 0 && m_data[lower] == item)
        --lower;

      //Find upper bounds
      while (upper <= m_currentSize && m_data[upper] == item)
        ++upper;

      //Number of elements to remove
      lower++;
      int num = upper - lower;

      //Remove elements
      memmove(&m_data[lower], &m_data[lower + num], (m_currentSize - index - 1 - num) * sizeof(T));

      //Adjust m_currentSize
      m_currentSize -= num;

      return true;

    }	//End: set::erase_all()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::reset()
    //--------------------------------------------------------------------------------
    //		Reset size to 1
    //--------------------------------------------------------------------------------
    template<class T>
    void set<T>::reset()
    {
      //Delete old m_data
      free(m_data);
      m_data = nullptr;

      m_arraySize = 0;
      m_currentSize = 0;

    }	//End: set::reset()


    //--------------------------------------------------------------------------------
    //	@	DgOrgeredArray<T>::clear()
    //--------------------------------------------------------------------------------
    //		Set the number of elements to zero
    //--------------------------------------------------------------------------------
    template<class T>
    void set<T>::clear()
    {
      m_currentSize = 0;

    }	//End: set::clear()
  }
};

#endif