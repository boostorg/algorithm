#ifndef BOOST_ALGORITHM_COUNTL_ZERO
#define BOOST_ALGORITHM_COUNTL_ZERO

#include <cstdint>
#include <type_traits>

namespace boost { namespace algorithm {

template <typename T>
BOOST_CXX14_CONSTEXPR
    typename std::enable_if<std::is_unsigned<T>::value, int>::type
    countl_zero(T value) BOOST_NOEXCEPT {
  static_assert(sizeof(T) <= sizeof(std::uint64_t),
                "Counting zeros on types larger than 8 bytes is not supported");
  if (value == 0)
    return sizeof(T) * 8;
  return __builtin_clzl(value) - (8 - sizeof(T)) * 8;
}
}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
