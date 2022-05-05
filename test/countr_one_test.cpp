#include <boost/config.hpp>
#include <boost/algorithm/cxx20/countr_one.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <cstdint>

template <typename T> BOOST_CXX14_CONSTEXPR T lower1() {
  return T(T(-1) >> sizeof(T) * 4);
}

void test_all() {
  BOOST_CHECK(boost::algorithm::countr_one(std::uint8_t(0)) == 0);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint8_t(1)) == 1);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint8_t(2)) == 0);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint8_t(3)) == 2);
  BOOST_CHECK(boost::algorithm::countr_one(lower1<std::uint8_t>()) == 4);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint8_t(-1)) == 8);

  BOOST_CHECK(boost::algorithm::countr_one(std::uint16_t(0)) == 0);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint16_t(1)) == 1);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint16_t(2)) == 0);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint16_t(3)) == 2);
  BOOST_CHECK(boost::algorithm::countr_one(lower1<std::uint16_t>()) == 8);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint16_t(-1)) == 16);

  BOOST_CHECK(boost::algorithm::countr_one(std::uint32_t(0)) == 0);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint32_t(1)) == 1);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint32_t(2)) == 0);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint32_t(3)) == 2);
  BOOST_CHECK(boost::algorithm::countr_one(lower1<std::uint32_t>()) == 16);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint32_t(-1)) == 32);

  BOOST_CHECK(boost::algorithm::countr_one(std::uint64_t(0)) == 0);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint64_t(1)) == 1);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint64_t(2)) == 0);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint64_t(3)) == 2);
  BOOST_CHECK(boost::algorithm::countr_one(lower1<std::uint64_t>()) == 32);
  BOOST_CHECK(boost::algorithm::countr_one(std::uint64_t(-1)) == 64);

#if __cplusplus >= 201402L
  static_assert(boost::algorithm::countr_one(std::uint8_t(0)) == 0,
                "constexpr test failed");
  static_assert(boost::algorithm::countr_one(std::uint8_t(1)) == 1,
                "constexpr test failed");
  static_assert(boost::algorithm::countr_one(std::uint8_t(2)) == 0,
                "constexpr test failed");
  static_assert(boost::algorithm::countr_one(std::uint8_t(3)) == 2,
                "constexpr test failed");
  static_assert(boost::algorithm::countr_one(lower1<std::uint8_t>()) == 4,
                "constexpr test failed");
  static_assert(boost::algorithm::countr_one(std::uint8_t(-1)) == 8,
                "constexpr test failed");
#endif
}

BOOST_AUTO_TEST_CASE(test_main) { test_all(); }
