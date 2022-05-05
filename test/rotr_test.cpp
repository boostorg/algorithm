#include <boost/config.hpp>
#include <boost/algorithm/cxx20/rotr.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <cstdint>

void test_all() {
  BOOST_CHECK(boost::algorithm::rotr(std::uint8_t(0), 0) == 0);
  BOOST_CHECK(boost::algorithm::rotr(std::uint8_t(0b00001111), 1) ==
              0b10000111);
  BOOST_CHECK(boost::algorithm::rotr(std::uint8_t(0b00001111), 2) ==
              0b11000011);
  BOOST_CHECK(boost::algorithm::rotr(std::uint8_t(0b00001111), -2) ==
              0b00111100);
  BOOST_CHECK(boost::algorithm::rotr(std::uint8_t(0b01010101), 1) ==
              0b10101010);
  BOOST_CHECK(boost::algorithm::rotr(std::uint8_t(0b11110000), 2) ==
              0b00111100);
  BOOST_CHECK(boost::algorithm::rotr(std::uint8_t(0b11110000), -2) ==
              0b11000011);

#if __cplusplus >= 201402L
  static_assert(boost::algorithm::rotr(std::uint8_t(0b00001111), 1) ==
                    0b10000111,
                "constexpr test failed");
#endif
}

BOOST_AUTO_TEST_CASE(test_main) { test_all(); }
