#ifndef BOOST_ALGORITHM_BYTE_SWAP
#define BOOST_ALGORITHM_BYTE_SWAP

#include <type_traits>

namespace boost { namespace algorithm {

template <typename T> BOOST_CXX14_CONSTEXPR
typename std::enable_if<std::is_integral<T>::value, T>::type byteswap(T value) BOOST_NOEXCEPT
{
    if (sizeof(T) == 1) return value;
    T res = 0;
    for (size_t i = 0; i < sizeof(T); ++i) {
        res |= (value & T(0xff)) << (sizeof(T) - i - 1)*8;
        value >>= 8;
    }
    return res;
}

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_BIT_CAST
