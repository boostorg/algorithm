#include <boost/config.hpp>
#include <boost/algorithm/cxx20/has_single_bit.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <cstdint>

void test_uint8() {
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint8_t(0)) == false);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint8_t(1)) == true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint8_t(2)) == true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint8_t(3)) == false);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint8_t(1u << 7)) == true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint8_t(0xff)) == false);
}

void test_uint16() {
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint16_t(0)) == false);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint16_t(1)) == true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint16_t(2)) == true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint16_t(3)) == false);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint16_t(1u << 15)) ==
              true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint16_t(0xffff)) == false);
}

void test_uint32() {
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint32_t(0)) == false);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint32_t(1)) == true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint32_t(2)) == true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint32_t(3)) == false);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint32_t(1024)) == true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint32_t(1111)) == false);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint32_t(1U << 31)) ==
              true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint32_t(0xffffffff)) ==
              false);
}

void test_uint64() {
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint64_t(0)) == false);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint64_t(1)) == true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint64_t(2)) == true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint64_t(3)) == false);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint64_t(1024)) == true);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint64_t(1111)) == false);
  BOOST_CHECK(boost::algorithm::has_single_bit(std::uint64_t(1UL << 63)) ==
              true);
  BOOST_CHECK(boost::algorithm::has_single_bit(
                  std::uint64_t(0xfffffffffffffffful)) == false);
}

void test_constexpr() {
#if __cplusplus >= 201402L
  static_assert(!boost::algorithm::has_single_bit(0u), "Constexpr false test failed"); static_assert( boost::algorithm::has_single_bit(2u), "Constexpr true test failed");
#endif
}

void test_all() {
  test_uint8();
  test_uint16();
  test_uint32();
  test_uint64();
  test_constexpr();
}

BOOST_AUTO_TEST_CASE(test_main) { test_all(); }
