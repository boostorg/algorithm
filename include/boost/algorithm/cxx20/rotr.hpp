#ifndef BOOST_ALGORITHM_ROTR
#define BOOST_ALGORITHM_ROTR

#include "rotl.hpp"

#include <type_traits>

namespace boost { namespace algorithm {

template <typename T>
BOOST_CXX14_CONSTEXPR
    typename std::enable_if<std::is_unsigned<T>::value, T>::type
    rotr(T value, int n) BOOST_NOEXCEPT {
  return rotl(value, -n);
}
}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
