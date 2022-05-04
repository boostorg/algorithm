#ifndef BOOST_ALGORITHM_BIT_CAST
#define BOOST_ALGORITHM_BIT_CAST

#include <type_traits>

namespace boost { namespace algorithm {

template <typename To, typename From>
BOOST_CXX14_CONSTEXPR
    typename std::enable_if<sizeof(To) == sizeof(From) &&
                                std::is_trivially_copyable<To>::value &&
                                std::is_trivially_copyable<From>::value,
                            To>::type
    bit_cast(const From &from) BOOST_NOEXCEPT {
  return __builtin_bit_cast(To, from);
}

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
