#ifndef DGHYPERARRAY_H
#define DGHYPERARRAY_H

#include <array>
#include <sstream>

namespace Dg
{
  namespace impl
  {
    /// Checks that all the template arguments are integral types using `std::is_integral`
    template <bool... > struct bool_pack { };

    template <bool... b>
    using all_true = std::is_same<bool_pack<true, b...>, bool_pack<b..., true>>;

    template <typename... T>
    using are_all_integral = all_true<std::is_integral<T>::value...>;

    /// Compile-time sum
    template <typename T>
    constexpr T ct_plus(T const x, T const y)
    {
      return x + y;
    }

    /// Compile-time product
    template <typename T>
    constexpr T ct_prod(T const x, T const y)
    {
      return x * y;
    }

    /// Compile-time equivalent to `std::accumulate()`
    template
    <
      typename    T,  ///< result type
      std::size_t N,  ///< length of the array
      typename    O   ///< type of the binary operation
    >
    constexpr T ct_accumulate(::std::array<T, N> const & arr,  ///< accumulate from this array
                              size_t const first,             ///< starting from this position
                              size_t const length,            ///< accumulate this number of elements
                              T const initialValue,      ///< let this be the accumulator's initial value
                              O const & op)                ///< use this binary operation
    {
      // https://stackoverflow.com/a/33158265/865719
      return (first < (first + length))
            ? op(arr[first],
                 ct_accumulate(arr,
                 first + 1,
                 length - 1,
                 initialValue,
                 op))
            : initialValue;
    }

    /// Compile-time equivalent to `std::inner_product()`
    template
    <
      typename T,      ///< the result type
      typename T_1,    ///< first array's type
      size_t   N_1,    ///< length of the first array
      typename T_2,    ///< second array's type
      size_t   N_2,    ///< length of the second array
      typename O_SUM,  ///< summation operation's type
      typename O_PROD  ///< multiplication operation's type
    >
    constexpr T ct_inner_product(::std::array<T_1, N_1> const & arr_1,  ///< perform the inner product of this array
                                 size_t const first_1,                ///< from this position
                                 ::std::array<T_2, N_2> const & arr_2,  ///< with this array
                                 size_t const first_2,                ///< from this position
                                 size_t const length,                 ///< using this many elements from both arrays
                                 T const initialValue,           ///< let this be the summation's initial value
                                 O_SUM const & op_sum,                 ///< use this as the summation operator
                                 O_PROD const & op_prod                 ///< use this as the multiplication operator
      )
    {
      // same logic as `ct_accumulate()`
      return (first_1 < (first_1 + length))
            ? op_sum(op_prod(arr_1[first_1], arr_2[first_2]),
                     ct_inner_product(arr_1, first_1 + 1,
                     arr_2, first_2 + 1,
                     length - 1,
                     initialValue,
                     op_sum, op_prod))
            : initialValue;
    }
  }

  template<typename T, size_t Dimensions>
  class HyperArray
  {
  public:
    using SizeType  = size_t;  ///< used for measuring sizes and lengths
    using IndexType = size_t;  ///< used for indices

  public:

    /// It doesn't make sense to create an array without specifying the dimension lengths
    HyperArray() = delete;

    HyperArray(std::array<size_t, Dimensions> const & a_dimensions)
      : m_dimensionLengths(a_dimensions)
      , m_dataLength{impl::ct_accumulate(m_dimensionLengths,
                                         0,
                                         Dimensions,
                                         static_cast<SizeType>(1),
                                         impl::ct_prod<SizeType>)}
      , m_indexCoeffs([this] {
                               std::array<SizeType, Dimensions> coeffs;
                               coeffs[Dimensions - 1] = 1;
                               for (SizeType i = 0; i < (Dimensions - 1); ++i)
                               {
                                 coeffs[i] = impl::ct_accumulate(m_dimensionLengths,
                                                                 i + 1,
                                                                 Dimensions - i - 1,
                                                                 static_cast<SizeType>(1),
                                                                 impl::ct_prod<SizeType>);
                               }
                               return coeffs;
                             }())
      , m_pData(nullptr)
    {
      m_pData = new T[m_dataLength];
    }
    
    HyperArray(HyperArray const & a_other)
      : m_dimensionLengths(a_other.m_dimensionLengths)
      , m_dataLength(a_other.m_dataLength)
      , m_indexCoeffs(a_other.m_indexCoeffs)
    {
      m_pData = new T[m_dataLength];
      for (size_t i = 0; i < m_dataLength; i++)
      {
        m_pData[i] = a_other.m_pData[i];
      }
    }
    
    HyperArray & operator=(HyperArray const & a_other)
    {
      if (this == & a_other)
      {
        m_dimensionLengths = a_other.m_dimensionLengths;
        m_dataLength = a_other.m_dataLength;
        m_indexCoeffs = a_other.m_indexCoeffs;
    
        delete[] m_pData;
        m_pData = new T[m_dataLength];
        for (size_t i = 0; i < m_dataLength; i++)
        {
          m_pData[i] = a_other.m_pData[i];
        }
      }
    
      return *this;
    }

    ~HyperArray()
    {
      delete[] m_pData;
    }

    /// Returns the length of a given dimension at run-time
    SizeType length(const size_t a_dimensionIndex) const
    {
      if (a_dimensionIndex >= Dimensions)
      {
        throw std::out_of_range("The dimension index must be within [0, Dimensions-1]");
      }
      return m_dimensionLengths[a_dimensionIndex];
    }

    /// Returns the length of a given dimension at run-time
    SizeType length_noChecks(const size_t a_dimensionIndex) const
    {
      return m_dimensionLengths[a_dimensionIndex];
    }

    /// Compile-time version of [length()](@ref length())
    template <size_t DimensionIndex>
    SizeType length() const
    {
      static_assert(DimensionIndex < Dimensions,
        "The dimension index must be within [0, Dimensions-1]");

      return m_dimensionLengths[DimensionIndex];
    }

    template <typename... Indices,
              typename = std::enable_if_t<impl::are_all_integral<Indices...>::value && sizeof...(Indices) == Dimensions>
             >
    T & operator()(Indices... a_indices) 
    {
      // nothrow implementation
      return m_pData[rawIndex_noChecks(a_indices...)];
    }

    template <typename... Indices,
              typename = std::enable_if_t<impl::are_all_integral<Indices...>::value && sizeof...(Indices) == Dimensions>
             >
    T const & operator()(Indices... a_indices) const
    {
      // nothrow implementation
      return m_pData[rawIndex_noChecks(a_indices...)];
    }

    template <typename... Indices,
              typename = std::enable_if_t<impl::are_all_integral<Indices...>::value && sizeof...(Indices) == Dimensions>
             >
    T & at(Indices... a_indices)
    {
      rangeCheck(a_indices...);
      return (*this)(a_indices...);
    }

    template <typename... Indices,
              typename = std::enable_if_t<impl::are_all_integral<Indices...>::value && sizeof...(Indices) == Dimensions>
             >
    T const & at(Indices... a_indices) const
    {
      rangeCheck(a_indices...);
      return (*this)(a_indices...);
    }

    /// Returns the actual index of the element in the [data](@ref data) array
    /// Usage:
    /// @code
    ///     hyper_array::array<int, 3> arr(4, 5, 6);
    ///     assert(&arr.at(3, 1, 4) == &arr.data[arr.rawIndex(3, 1, 4)]);
    /// @endcode
    template <typename... Indices,
              typename = std::enable_if_t<impl::are_all_integral<Indices...>::value && sizeof...(Indices) == Dimensions>
             >
    IndexType rawIndex(Indices... a_indices) const
    {
      rangeCheck(a_indices...);
      return rawIndex_noChecks(a_indices...);
    }

    template <typename... Indices,
              typename = std::enable_if_t<impl::are_all_integral<Indices...>::value && sizeof...(Indices) == Dimensions>
             >
    IndexType rawIndex_noChecks(Indices... a_indices) const
    {
      std::array<IndexType, Dimensions> indexArray = {{static_cast<IndexType>(a_indices)...}};

      // I_{actual} = \sum_{i=0}^{N-1} {C_i \cdot I_i}
      //
      // where I_{actual} : actual index of the data in the data array
      //       N          : Dimensions
      //       C_i        : indexCoeffs[i]
      //       I_i        : indexArray[i]
      return impl::ct_inner_product(m_indexCoeffs, 0,
                                    indexArray, 0,
                                    Dimensions,
                                    static_cast<IndexType>(0),
                                    impl::ct_plus<IndexType>,
                                    impl::ct_prod<IndexType>);
    }

  private:

    template <typename... Indices,
              typename = std::enable_if_t<impl::are_all_integral<Indices...>::value && sizeof...(Indices) == Dimensions>
             >
    void rangeCheck(Indices... indices) const
    {
      // runtime input validation
      std::array<IndexType, Dimensions> indexArray = {{static_cast<IndexType>(indices)...}};

      // check all indices and prepare an exhaustive report (in oss)
      // if some of them are out of bounds
      std::ostringstream oss;
      for (size_t i = 0; i < Dimensions; ++i)
      {
        if ((indexArray[i] >= m_dimensionLengths[i]) || (indexArray[i] < 0))
        {
          oss << "Index #" << i << " [== " << indexArray[i] << "]"
            << " is out of the [0, " << (m_dimensionLengths[i]-1) << "] range. ";
        }
      }

      // if nothing has been written to oss then all indices are valid
      if (!oss.str().empty())
      {
        throw std::out_of_range(oss.str());
      }
    }

  private:
    static constexpr SizeType s_dimensions = Dimensions;

    std::array<SizeType, Dimensions> const m_dimensionLengths;
    T *                                    m_pData;
    SizeType const                         m_dataLength;
    std::array<SizeType, Dimensions> const m_indexCoeffs; 
  };
}

#endif