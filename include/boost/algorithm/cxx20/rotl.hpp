#ifndef BOOST_ALGORITHM_ROTL
#define BOOST_ALGORITHM_ROTL

#include <type_traits>

namespace boost { namespace algorithm {

template <typename T>
BOOST_CXX14_CONSTEXPR
    typename std::enable_if<std::is_unsigned<T>::value, T>::type
    rotl(T value, int n) BOOST_NOEXCEPT {
  BOOST_CXX14_CONSTEXPR T mask = 8 * sizeof(value) - 1;
  n &= mask;
  return (value << n) | (value >> (T(-n) & mask));
}
}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
