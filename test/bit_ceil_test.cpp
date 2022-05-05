#include <boost/config.hpp>
#include <boost/algorithm/cxx20/bit_ceil.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <cstdint>

template <typename T> BOOST_CXX14_CONSTEXPR T max_ceil() {
  return T(T(-1) / 2);
}

void test_all() {
  BOOST_CHECK(boost::algorithm::bit_ceil(0u) == 1);
  BOOST_CHECK(boost::algorithm::bit_ceil(1u) == 1);
  BOOST_CHECK(boost::algorithm::bit_ceil(2u) == 2);
  BOOST_CHECK(boost::algorithm::bit_ceil(3u) == 4);
  BOOST_CHECK(boost::algorithm::bit_ceil(4u) == 4);
  BOOST_CHECK(boost::algorithm::bit_ceil(5u) == 8);
  BOOST_CHECK(boost::algorithm::bit_ceil(6u) == 8);
  BOOST_CHECK(boost::algorithm::bit_ceil(7u) == 8);
  BOOST_CHECK(boost::algorithm::bit_ceil(8u) == 8);

  BOOST_CHECK(boost::algorithm::bit_ceil(max_ceil<std::uint8_t>()) == 0x80);
  BOOST_CHECK(boost::algorithm::bit_ceil(max_ceil<std::uint16_t>()) == 0x8000);
  BOOST_CHECK(boost::algorithm::bit_ceil(max_ceil<std::uint32_t>()) ==
              0x80000000u);
  BOOST_CHECK(boost::algorithm::bit_ceil(max_ceil<std::uint64_t>()) ==
              0x8000000000000000ul);
}

BOOST_AUTO_TEST_CASE(test_main) { test_all(); }
