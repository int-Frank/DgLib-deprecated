#ifndef DG_SIMPLEPAGE_H
#define DG_SIMPLEPAGE_H

#include "impl\DgContainerBase.h"

namespace Dg
{
  namespace impl
  {
    template<typename I, typename T>
    class SimplePage : public ContainerBase
    {
      //! Container to hold the object and pointers
      class Node
      {
      public:

        //! Default constructor
        Node() : pNext(nullptr){}


        //! Destruct the data is a destructor exists for the type.
        inline void DestructData()
        {
          if (!std::is_trivially_destructible<T>::value)
          {
            data.~T();
          }
        }

      private:

        Node* pNext;
        T     data;
      };

    public:

      SimplePage();
      ~SimplePage();

      SimplePage(SimplePage const &);
      SimplePage & operator=(SimplePage const &);

      I GetNewObject();
      void Return(I);
      
      T & operator[I];
      T const & operator[I] const;

      void clear();
      void resize();

    private:

      size_t m_currentUsed;
      Node * m_pData;

    };
  }
}

#endif