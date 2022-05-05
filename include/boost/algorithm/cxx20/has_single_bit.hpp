#ifndef BOOST_ALGORITHM_HAS_SINGLE_BIT
#define BOOST_ALGORITHM_HAS_SINGLE_BIT

#include <type_traits>

namespace boost { namespace algorithm {

template <typename T>
BOOST_CXX14_CONSTEXPR
    typename std::enable_if<std::is_unsigned<T>::value, bool>::type
    has_single_bit(T value) BOOST_NOEXCEPT {
  return value && (value & (value - 1)) == 0;
}
}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
