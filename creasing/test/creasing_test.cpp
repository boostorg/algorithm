//  Copyright (c) 2010 Nuovation System Designs, LLC
//    Grant Erickson <gerickson@nuovations.com>
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/ for latest version.

#include <algorithm>
#include <iostream>

#include <boost/algorithm/creasing.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/test/unit_test.hpp>

using namespace boost;

/* Preprocessor Defines */

#define elementsof(v)	(sizeof (v) / sizeof (v[0]))
#define begin(v)		(v)
#define end(v)			(v + elementsof (v))

template <typename InputIterator, typename Function>
static bool
test_sequence(InputIterator inBegin,
			  InputIterator inEnd,
			  Function inFunction)
{
	return (inFunction(inBegin, inEnd));
}

static void
test_creasing(void)
{
	const int strictlyIncreasingValues[] = { 1, 2, 3, 4, 5 };
	const int strictlyDecreasingValues[] = { 9, 8, 7, 6, 5 };
	const int increasingValues[] = { 1, 2, 2, 2, 5 };
	const int decreasingValues[] = { 9, 7, 7, 7, 5 };
	const int randomValues[] = { 3, 6, 1, 2, 7 };
	const int constantValues[] = { 7, 7, 7, 7, 7 };
	bool result;

	// Test a strictly increasing sequence

	result = test_sequence(begin(strictlyIncreasingValues),
						   end(strictlyIncreasingValues),
						   is_strictly_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, true);
	result = test_sequence(begin(strictlyIncreasingValues),
						   end(strictlyIncreasingValues),
						   is_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, true);
	result = test_sequence(begin(strictlyIncreasingValues),
						   end(strictlyIncreasingValues),
						   is_strictly_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(strictlyIncreasingValues),
						   end(strictlyIncreasingValues),
						   is_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);

	// Test a strictly decreasing sequence

	result = test_sequence(begin(strictlyDecreasingValues),
						   end(strictlyDecreasingValues),
						   is_strictly_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(strictlyDecreasingValues),
						   end(strictlyDecreasingValues),
						   is_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(strictlyDecreasingValues),
						   end(strictlyDecreasingValues),
						   is_strictly_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, true);
	result = test_sequence(begin(strictlyDecreasingValues),
						   end(strictlyDecreasingValues),
						   is_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, true);

	// Test an increasing sequence

	result = test_sequence(begin(increasingValues),
						   end(increasingValues),
						   is_strictly_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(increasingValues),
						   end(increasingValues),
						   is_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, true);
	result = test_sequence(begin(increasingValues),
						   end(increasingValues),
						   is_strictly_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(increasingValues),
						   end(increasingValues),
						   is_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);

	// Test a decreasing sequence

	result = test_sequence(begin(decreasingValues),
						   end(decreasingValues),
						   is_strictly_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(decreasingValues),
						   end(decreasingValues),
						   is_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(decreasingValues),
						   end(decreasingValues),
						   is_strictly_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(decreasingValues),
						   end(decreasingValues),
						   is_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, true);

	// Test a random sequence

	result = test_sequence(begin(randomValues),
						   end(randomValues),
						   is_strictly_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(randomValues),
						   end(randomValues),
						   is_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(randomValues),
						   end(randomValues),
						   is_strictly_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(randomValues),
						   end(randomValues),
						   is_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	
	// Test a constant sequence

	result = test_sequence(begin(constantValues),
						   end(constantValues),
						   is_strictly_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(constantValues),
						   end(constantValues),
						   is_increasing<int const *>);
	BOOST_CHECK_EQUAL(result, true);
	result = test_sequence(begin(constantValues),
						   end(constantValues),
						   is_strictly_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, false);
	result = test_sequence(begin(constantValues),
						   end(constantValues),
						   is_decreasing<int const *>);
	BOOST_CHECK_EQUAL(result, true);	
}

int test_main( int, char * [] )
{
	test_creasing();

	return 0;
}
