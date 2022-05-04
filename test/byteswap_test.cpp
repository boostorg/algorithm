#include <boost/config.hpp>
#include <boost/algorithm/cxx20/byteswap.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

void test_int8() {
  BOOST_CHECK(std::int8_t(0) == boost::algorithm::byteswap(std::int8_t(0)));
  BOOST_CHECK(std::int8_t(1) == boost::algorithm::byteswap(std::int8_t(0x01)));
  BOOST_CHECK(std::int8_t(0xde) == boost::algorithm::byteswap(std::int8_t(0xde)));
}

void test_uint8() {
  BOOST_CHECK(std::uint8_t(0) == boost::algorithm::byteswap(std::uint8_t(0)));
  BOOST_CHECK(std::uint8_t(1) == boost::algorithm::byteswap(std::uint8_t(0x01)));
  BOOST_CHECK(std::uint8_t(0xad) == boost::algorithm::byteswap(std::uint8_t(0xad)));
}

void test_int16() {
  BOOST_CHECK(std::int16_t(0) == boost::algorithm::byteswap(std::int16_t(0)));
  BOOST_CHECK(std::int16_t(1) == boost::algorithm::byteswap(std::int16_t(0x0100)));
  BOOST_CHECK(std::int16_t(0xdead) == boost::algorithm::byteswap(std::int16_t(0xadde)));
}

void test_uint16() {
  BOOST_CHECK(std::uint16_t(0) == boost::algorithm::byteswap(std::uint16_t(0)));
  BOOST_CHECK(std::uint16_t(1) == boost::algorithm::byteswap(std::uint16_t(0x0100)));
  BOOST_CHECK(std::uint16_t(0xdead) == boost::algorithm::byteswap(std::uint16_t(0xadde)));
}

void test_int32() {
  BOOST_CHECK(std::int32_t(0) == boost::algorithm::byteswap(std::int32_t(0)));
  BOOST_CHECK(std::int32_t(1) == boost::algorithm::byteswap(std::int32_t(0x01000000)));
  BOOST_CHECK(std::int32_t(0xdeadbeef) == boost::algorithm::byteswap(std::int32_t(0xefbeadde)));
}

void test_uint32() {
  BOOST_CHECK(std::uint32_t(0) == boost::algorithm::byteswap(std::uint32_t(0)));
  BOOST_CHECK(std::uint32_t(1) == boost::algorithm::byteswap(std::uint32_t(0x01000000)));
  BOOST_CHECK(std::uint32_t(0xdeadbeef) == boost::algorithm::byteswap(std::uint32_t(0xefbeadde)));
}

void test_int64() {
  BOOST_CHECK(std::int64_t(0) == boost::algorithm::byteswap(std::int64_t(0)));
  BOOST_CHECK(std::int64_t(1) == boost::algorithm::byteswap(std::int64_t(0x0100000000000000)));
  BOOST_CHECK(std::int64_t(0xdeadbeefdeadbeef) ==
              boost::algorithm::byteswap(std::int64_t(0xefbeaddeefbeadde)));
}

void test_uint64() {
  BOOST_CHECK(std::uint64_t(0) == boost::algorithm::byteswap(std::uint64_t(0)));
  BOOST_CHECK(std::uint64_t(1) == boost::algorithm::byteswap(std::uint64_t(0x0100000000000000)));
  BOOST_CHECK(std::uint64_t(0xdeadbeefdeadbeef) ==
              boost::algorithm::byteswap(std::uint64_t(0xefbeaddeefbeadde)));
}

void test_constexpr() {
#if __cplusplus >= 201402L
  static_assert(boost::algorithm::byteswap(0xdeadbeef) == 0xefbeadde, "");
  static_assert(boost::algorithm::byteswap(0xdeadbeefu) == 0xefbeaddeu, "");
#endif
}

BOOST_AUTO_TEST_CASE(test_main) {
  test_int8();
  test_uint8();
  test_int16();
  test_uint16();
  test_int32();
  test_uint32();
  test_int64();
  test_uint64();
  test_constexpr();
}
