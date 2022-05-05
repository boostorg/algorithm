#include <boost/config.hpp>
#include <boost/algorithm/cxx20/bit_floor.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <cstdint>

template <typename T> BOOST_CXX14_CONSTEXPR T mid_floor(T x) {
  return T((T(-1) - 2 + x) / 2 + 1);
}

void test_all() {
  BOOST_CHECK(boost::algorithm::bit_floor(0u) == 0);
  BOOST_CHECK(boost::algorithm::bit_floor(1u) == 1);
  BOOST_CHECK(boost::algorithm::bit_floor(2u) == 2);
  BOOST_CHECK(boost::algorithm::bit_floor(3u) == 2);
  BOOST_CHECK(boost::algorithm::bit_floor(4u) == 4);
  BOOST_CHECK(boost::algorithm::bit_floor(5u) == 4);
  BOOST_CHECK(boost::algorithm::bit_floor(6u) == 4);
  BOOST_CHECK(boost::algorithm::bit_floor(7u) == 4);
  BOOST_CHECK(boost::algorithm::bit_floor(8u) == 8);

  BOOST_CHECK(boost::algorithm::bit_floor(std::uint8_t(-1)) == 0x80);
  BOOST_CHECK(boost::algorithm::bit_floor(mid_floor<std::uint8_t>(1)) == 0x80);
  BOOST_CHECK(boost::algorithm::bit_floor(mid_floor<std::uint8_t>(2)) == 0x80);

  BOOST_CHECK(boost::algorithm::bit_floor(std::uint16_t(-1)) == 0x8000);
  BOOST_CHECK(boost::algorithm::bit_floor(mid_floor<std::uint16_t>(1)) ==
              0x8000);
  BOOST_CHECK(boost::algorithm::bit_floor(mid_floor<std::uint16_t>(2)) ==
              0x8000);

  BOOST_CHECK(boost::algorithm::bit_floor(std::uint32_t(-1)) == 0x80000000u);
  BOOST_CHECK(boost::algorithm::bit_floor(mid_floor<std::uint32_t>(1)) ==
              0x80000000u);
  BOOST_CHECK(boost::algorithm::bit_floor(mid_floor<std::uint32_t>(2)) ==
              0x80000000u);

  BOOST_CHECK(boost::algorithm::bit_floor(std::uint64_t(-1)) ==
              0x8000000000000000ul);
  BOOST_CHECK(boost::algorithm::bit_floor(mid_floor<std::uint64_t>(1)) ==
              0x8000000000000000ul);
  BOOST_CHECK(boost::algorithm::bit_floor(mid_floor<std::uint64_t>(2)) ==
              0x8000000000000000ul);

#if __cplusplus >= 201402L
  static_assert(boost::algorithm::bit_floor(0u) == 0, "constexpr test failed");
  static_assert(boost::algorithm::bit_floor(6u) == 4, "constexpr test failed");
#endif
}

BOOST_AUTO_TEST_CASE(test_main) { test_all(); }
