#ifndef BOOST_ALGORITHM_BIT_FLOOR
#define BOOST_ALGORITHM_BIT_FLOOR

#include "bit_width.hpp"

#include <type_traits>

namespace boost { namespace algorithm {

template <typename T>
BOOST_CXX14_CONSTEXPR
    typename std::enable_if<std::is_unsigned<T>::value, T>::type
    bit_floor(T value) BOOST_NOEXCEPT {
  if (value == 0)
    return 0;
  return T(1) << (bit_width(value) - 1);
}
}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
