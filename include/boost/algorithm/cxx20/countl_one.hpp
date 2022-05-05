#ifndef BOOST_ALGORITHM_COUNTL_ONE
#define BOOST_ALGORITHM_COUNTL_ONE

#include "countl_zero.hpp"

#include <type_traits>

namespace boost { namespace algorithm {

template <typename T>
BOOST_CXX14_CONSTEXPR
    typename std::enable_if<std::is_unsigned<T>::value, int>::type
    countl_one(T value) BOOST_NOEXCEPT {
  return countl_zero(T(value ^ T(-1)));
}
}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
