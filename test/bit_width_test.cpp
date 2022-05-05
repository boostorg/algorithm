#include <boost/config.hpp>
#include <boost/algorithm/cxx20/bit_width.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <cstdint>

void test_all() {
  BOOST_CHECK(boost::algorithm::bit_width(0u) == 0);
  BOOST_CHECK(boost::algorithm::bit_width(1u) == 1);
  BOOST_CHECK(boost::algorithm::bit_width(2u) == 2);
  BOOST_CHECK(boost::algorithm::bit_width(3u) == 2);
  BOOST_CHECK(boost::algorithm::bit_width(4u) == 3);
  BOOST_CHECK(boost::algorithm::bit_width(5u) == 3);
  BOOST_CHECK(boost::algorithm::bit_width(6u) == 3);
  BOOST_CHECK(boost::algorithm::bit_width(7u) == 3);
  BOOST_CHECK(boost::algorithm::bit_width(8u) == 4);

  BOOST_CHECK(boost::algorithm::bit_width(std::uint8_t(-1)) == 8);
  BOOST_CHECK(boost::algorithm::bit_width(std::uint16_t(-1)) == 16);
  BOOST_CHECK(boost::algorithm::bit_width(std::uint32_t(-1)) == 32);
  BOOST_CHECK(boost::algorithm::bit_width(std::uint64_t(-1)) == 64);

#if __cplusplus >= 201402L
  static_assert(boost::algorithm::bit_width(0u) == 0, "constexpr test failed");
  static_assert(boost::algorithm::bit_width(1u) == 1, "constexpr test failed");
  static_assert(boost::algorithm::bit_width(2u) == 2, "constexpr test failed");
  static_assert(boost::algorithm::bit_width(std::uint8_t(-1)) == 8,
                "constexpr test failed");
#endif
}

BOOST_AUTO_TEST_CASE(test_main) { test_all(); }
