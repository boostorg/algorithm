#ifndef BOOST_ALGORITHM_POPCOUNT
#define BOOST_ALGORITHM_POPCOUNT

#include <cstdint>
#include <type_traits>

namespace boost { namespace algorithm {

template <typename T>
BOOST_CXX14_CONSTEXPR
    typename std::enable_if<std::is_unsigned<T>::value, int>::type
    popcount(T value) BOOST_NOEXCEPT {
  static_assert(sizeof(T) <= sizeof(std::uint64_t),
                "Popcount on types larger than 8 bytes is not supported");
  return __builtin_popcountl(value);
}
}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
