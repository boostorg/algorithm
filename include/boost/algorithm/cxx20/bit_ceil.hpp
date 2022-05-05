#ifndef BOOST_ALGORITHM_BIT_CEIL
#define BOOST_ALGORITHM_BIT_CEIL

#include "bit_width.hpp"

#include <type_traits>

namespace boost { namespace algorithm {

template <typename T>
BOOST_CXX14_CONSTEXPR
    typename std::enable_if<std::is_unsigned<T>::value, T>::type
    bit_ceil(T value) BOOST_NOEXCEPT {
  if (value == 0)
    return 1;
  return T(1) << bit_width(T(value - 1));
}
}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
