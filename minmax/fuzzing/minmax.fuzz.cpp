//  (C) Copyright Marshall Clow 2018
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/algorithm/minmax.hpp>

//	Fuzzing tests for:
//
// 		template <class T>
// 		tuple<T const&, T const&>
// 		minmax(const T& a, const T& b);
// 
// 		template <class T, class BinaryPredicate>
// 		tuple<T const&, T const&>
// 		minmax(const T& a, const T& b, BinaryPredicate comp);


bool greater(uint8_t lhs, uint8_t rhs) { return lhs > rhs; }

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t sz) {
	typedef boost::tuple<uint8_t const&, uint8_t const&> result_t;
	if (sz < 2) return 0; // we only need two elements
	
	{
	result_t result = boost::minmax(data[0], data[1]);
	uint8_t const& first  = result.get<0>();
	uint8_t const& second = result.get<1>();

//	first must be <= second
	if (second < first) return 1;

//	The references returned must be data[0] and data[1]
	if (&first  != data && &first  != data + 1) return 2;
	if (&second != data && &second != data + 1) return 2;
	}
	
	{
	result_t result = boost::minmax(data[0], data[1], greater);
	uint8_t const& first  = result.get<0>();
	uint8_t const& second = result.get<1>();

//	first must be <= second
	if (greater(second, first)) return 1;

//	The references returned must be data[0] and data[1]
	if (&first  != data && &first  != data + 1) return 2;
	if (&second != data && &second != data + 1) return 2;
	}

  return 0;
}