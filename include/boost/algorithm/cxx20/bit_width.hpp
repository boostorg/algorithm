#ifndef BOOST_ALGORITHM_BIT_WIDTH
#define BOOST_ALGORITHM_BIT_WIDTH

#include <type_traits>
#include <cstdint>

namespace boost { namespace algorithm {

template <typename T>
BOOST_CXX14_CONSTEXPR
    typename std::enable_if<std::is_unsigned<T>::value, T>::type
    bit_width(T value) BOOST_NOEXCEPT {
  static_assert(sizeof(T) <= sizeof(std::uint64_t),
                "Cannot calculate width for types over 8 bytes");
  if (value == 0)
    return 0;
  return 1 + (63ul ^ __builtin_clzl(value));
}
}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
