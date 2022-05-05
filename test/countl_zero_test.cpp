#include <boost/config.hpp>
#include <boost/algorithm/cxx20/countl_zero.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <cstdint>

void test_all() {
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint8_t(0)) == 8);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint8_t(1)) == 7);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint8_t(2)) == 6);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint8_t(-1)) == 0);

  BOOST_CHECK(boost::algorithm::countl_zero(std::uint16_t(0)) == 16);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint16_t(1)) == 15);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint16_t(2)) == 14);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint16_t(-1)) == 0);

  BOOST_CHECK(boost::algorithm::countl_zero(std::uint32_t(0)) == 32);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint32_t(1)) == 31);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint32_t(2)) == 30);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint32_t(-1)) == 0);

  BOOST_CHECK(boost::algorithm::countl_zero(std::uint64_t(0)) == 64);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint64_t(1)) == 63);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint64_t(2)) == 62);
  BOOST_CHECK(boost::algorithm::countl_zero(std::uint64_t(-1)) == 0);

#if __cplusplus >= 201402L
  static_assert(boost::algorithm::countl_zero(std::uint8_t(0)) == 8,
                "constexpr test failed");
  static_assert(boost::algorithm::countl_zero(std::uint8_t(1)) == 7,
                "constexpr test failed");
  static_assert(boost::algorithm::countl_zero(std::uint8_t(2)) == 6,
                "constexpr test failed");
  static_assert(boost::algorithm::countl_zero(std::uint8_t(-1)) == 0,
                "constexpr test failed");
#endif
}

BOOST_AUTO_TEST_CASE(test_main) { test_all(); }
