#ifndef BOOST_ALGORITHM_COUNTR_ZERO
#define BOOST_ALGORITHM_COUNTR_ZERO

#include <cstdint>
#include <type_traits>

namespace boost { namespace algorithm {

template <typename T>
BOOST_CXX14_CONSTEXPR
    typename std::enable_if<std::is_unsigned<T>::value, int>::type
    countr_zero(T value) BOOST_NOEXCEPT {
  static_assert(sizeof(T) <= sizeof(std::uint64_t),
                "Counting ones on types larger than 8 bytes is not supported");
  if (value == 0)
    return sizeof(T) * 8;
  return __builtin_ctzl(value);
}
}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
