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

#define DG_MAP_DEFAULT_SIZE 1024

namespace Dg
{
  namespace POD
  {
    //--------------------------------------------------------------------------------
    //	@	map<U, T>:	U: key
    //						T: m_data
    //--------------------------------------------------------------------------------
    template<class U, class T>
    class map
    {
      //Internal container which stores the m_data
      struct Container
      {
        U key;
        T item;
      };

    public:
      //Constructor / destructor
      map();
      map(unsigned int);
      ~map();

      //Copy operations
      map(const map&);
      map& operator= (const map&);

      //Accessors

      T&       operator[](unsigned int i)				{ return m_data[i].item; }
      const T& operator[](unsigned int i) const { return m_data[i].item; }
      int      size()			                const	{ return m_currentSize; }
      bool     empty()		                const	{ return m_currentSize == 0; }
      int      max_size()		              const	{ return m_arraySize; }
      U        ID(int i)			            const { return m_data[i].key; }

      //Returns false if not found, however, index will be set to
      //the lower index just before where input key would be.
      bool find(U key, int& index, int lower = 0) const;			//Use binary search
      bool find(U key, int& index, int lower, int upper) const;	//Use binary search

      //Manipulators
      bool insert(const T&, U key);
      bool set(U key, const T&);
      bool erase(U key);
      void clear();
      void resize(unsigned int);
      void extend();
      void reset();

    private:
      //Data members
      Container* m_data;

      int m_arraySize;
      int m_currentSize;

    private:
      void init(const map&);
    };


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::map()
    //--------------------------------------------------------------------------------
    //		Constructor
    //--------------------------------------------------------------------------------
    template<class U, class T>
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
    template<class U, class T>
    map<U, T>::map(unsigned int new_size)
      : m_data(nullptr), m_arraySize(0), m_currentSize(0)
    {
      resize(new_size);

    }	//End: map::map()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::~map()
    //--------------------------------------------------------------------------------
    //		Destructor
    //--------------------------------------------------------------------------------
    template<class U, class T>
    map<U, T>::~map()
    {
      //Free memory
      clear();
      free(m_data);

    }	//End: map::~map()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::init()
    //--------------------------------------------------------------------------------
    //		Initialise map to another.
    //--------------------------------------------------------------------------------
    template<class U, class T>
    void map<U, T>::init(const map& other)
    {
      //Resize lists
      int sze = (other.m_arraySize>0) ? other.m_arraySize : 1;
      resize(sze);

      memcpy(&m_data[0], &other.m_data[0], other.m_currentSize * sizeof(Container));

      m_currentSize = other.m_currentSize;

    }	//End: map::init()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::map()
    //--------------------------------------------------------------------------------
    //		Copy constructor
    //--------------------------------------------------------------------------------
    template<class U, class T>
    map<U, T>::map(const map& other) :
      m_data(nullptr), m_arraySize(0), m_currentSize(0)
    {
      init(other);

    }	//End: map::map()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::operator=()
    //--------------------------------------------------------------------------------
    //		Assignment
    //--------------------------------------------------------------------------------
    template<class U, class T>
    map<U, T>& map<U, T>::operator=(const map& other)
    {
      if (this == &other)
        return *this;

      init(other);

      return *this;

    }	//End: map::operator=()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::resize()
    //--------------------------------------------------------------------------------
    //		Resize map, erases all m_data before resize.
    //--------------------------------------------------------------------------------
    template<class U, class T>
    void map<U, T>::resize(unsigned int val)
    {
      //Delete old m_data
      clear();
      free(m_data);
      m_data = (Container *)malloc(sizeof(Container) * val);

      m_arraySize = val;
      m_currentSize = 0;

    }	//End: map::resize()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::find()
    //--------------------------------------------------------------------------------
    //		Find a value in the map, uses a binary search algorithm
    //--------------------------------------------------------------------------------
    template<class U, class T>
    bool map<U, T>::find(U key, int& index, int lower) const
    {
      return find(key, index, lower, (m_currentSize - 1));

    }	//End: map::find()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::find()
    //--------------------------------------------------------------------------------
    //		Find a value in the map within a range, uses a binary search algorithm
    //--------------------------------------------------------------------------------
    template<class U, class T>
    bool map<U, T>::find(U key, int& index, int lower, int upper) const
    {
      //Check bounds
      lower = (lower>0) ? lower : 0;
      upper = (upper<m_currentSize - 1) ? upper : m_currentSize - 1;

      while (lower <= upper)
      {
        // calculate the midpoint for roughly equal partition
        index = ((upper + lower) >> 1);

        // determine which subarray to search
        if (m_data[index].key < key)
          // change min index to search upper subarray
          lower = index + 1;
        else if (m_data[index].key > key)
          // change max index to search lower subarray
          upper = index - 1;
        else
          // key found at index index
          return true;
      }

      //Set index closest (but lower) to key
      index = lower - 1;
      return false;

    }	//End: map::find()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::extend()
    //--------------------------------------------------------------------------------
    //		Extend the map by a factor of 2, keeps all m_data.
    //--------------------------------------------------------------------------------
    template<class U, class T>
    void map<U, T>::extend()
    {
      //Calculate new size
      int new_size = (m_arraySize << 1);

      //Create new arrays
      Container * new_data = (Container *)malloc(sizeof(Container) * new_size);

      memcpy(new_data, m_data, (sizeof(Container) * m_currentSize));

      free(m_data);
      m_data = new_data;
      m_arraySize = new_size;

    }	//End: map::extend()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::insert()
    //--------------------------------------------------------------------------------
    //		Insert an element into the map
    //--------------------------------------------------------------------------------
    template<class U, class T>
    bool map<U, T>::insert(const T& val, U key)
    {
      //Find the index to insert to
      int index;
      if (find(key, index))
        return false;	//element already exists

      //Range check
      if (m_currentSize == m_arraySize)
        extend();

      //shift all RHS objects to the right by one.
      memmove(&m_data[index + 2], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(Container));

      index++;

      memcpy(&m_data[index].key, &key, sizeof(key));
      memcpy(&m_data[index].item, &val, sizeof(val));

      m_currentSize++;

      return true;

    }	//End: map::insert()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::erase()
    //--------------------------------------------------------------------------------
    //		Remove an element from the map
    //--------------------------------------------------------------------------------
    template<class U, class T>
    bool map<U, T>::erase(U key)
    {
      //Find the index
      int index;
      if (!find(key, index))
        return false;	//element not found

      memmove(&m_data[index], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(Container));

      m_currentSize--;

      return true;

    }	//End: map::erase()


    //--------------------------------------------------------------------------------
    //	@	map<U,T>::set()
    //--------------------------------------------------------------------------------
    //		Sets an element to a new value
    //--------------------------------------------------------------------------------
    template<class U, class T>
    bool map<U, T>::set(U key, const T& val)
    {
      //Find the index to insert to
      int index;
      if (!find(key, index))
        return false;	//element does not exist

      memcpy(&m_data[index].item, &val, sizeof(val));

      return true;

    }	//End: map::set()


    //--------------------------------------------------------------------------------
    //	@	DgMap<U,T>::reset()
    //--------------------------------------------------------------------------------
    //		Reset size to 1
    //--------------------------------------------------------------------------------
    template<class U, class T>
    void map<U, T>::reset()
    {
      resize(DG_MAP_DEFAULT_SIZE);

    }	//End: map::reset()


    //--------------------------------------------------------------------------------
    //	@	DgMap<U,T>::clear()
    //--------------------------------------------------------------------------------
    //		Set the number of elements to zero
    //--------------------------------------------------------------------------------
    template<class U, class T>
    void map<U, T>::clear()
    {
      m_currentSize = 0;

    }	//End: map::clear()
  }
};

#endif