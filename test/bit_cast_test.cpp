#include <boost/config.hpp>
#include <boost/algorithm/cxx20/bit_cast.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

struct S_nodc {
  BOOST_CXX14_CONSTEXPR explicit S_nodc(int i) : i(i) {}

  int i;
};

struct S_notc {
  BOOST_CXX14_CONSTEXPR S_notc() {}
  BOOST_CXX14_CONSTEXPR S_notc(const S_notc &s) : i(s.i) {}

  int i;
};

void test_int() {
  int i = 42;
  BOOST_CHECK(42u == boost::algorithm::bit_cast<unsigned int>(i));
}

void test_constexpr() {
#if __cplusplus >= 201402L
  static_assert(boost::algorithm::bit_cast<unsigned int>(42) == 42u, "");
#endif
}

void test_no_default_constructor() {
  S_nodc s(42);
  BOOST_CHECK(42u == boost::algorithm::bit_cast<unsigned int>(s));
  BOOST_CHECK(42 == boost::algorithm::bit_cast<S_nodc>(42).i);
}

#if __cplusplus >= 201402L
template <typename To, typename From>
static std::enable_if<sizeof(boost::algorithm::bit_cast<To>(From())) != 0,
                      std::true_type>
f_is_bin_castable(To &&);
template <typename To, typename From>
static std::false_type f_is_bin_castable(...);
template <typename To, typename From>
struct is_bit_castable : public decltype(f_is_bin_castable<To, From>(0)) {};
#endif

void test_no_trivial_constructor() {
#if __cplusplus >= 201402L
  static_assert(!is_bit_castable<std::uint32_t, std::uint64_t>::value, "");
  static_assert(!is_bit_castable<std::uint64_t, std::uint32_t>::value, "");
  static_assert(!is_bit_castable<int, S_notc>::value, "");
  static_assert(!is_bit_castable<S_notc, int>::value, "");
#endif
}

BOOST_AUTO_TEST_CASE(test_main) {
  test_int();
  test_constexpr();
  test_no_default_constructor();
  test_no_trivial_constructor();
}
