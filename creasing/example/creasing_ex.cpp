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

/* Preprocessor Defines */

#define elementsof(v)	(sizeof (v) / sizeof (v[0]))
#define begin(v)		(v)
#define end(v)			(v + elementsof (v))

static void
output_sequence_property(bool result, const char * property)
{
	std::cout << "  "
			  << ((result) ? "Is " : "Is not ")
			  << property
			  << std::endl;
}

template <typename InputIterator>
static void
analyze_sequence(InputIterator first, InputIterator last)
{
	bool is_increasing;
	bool is_decreasing;
	bool is_strictly_increasing;
	bool is_strictly_decreasing;

	using namespace std;
	using namespace boost::lambda;

	is_increasing = boost::is_increasing(first, last);
	is_decreasing = boost::is_decreasing(first, last);
	is_strictly_increasing = boost::is_strictly_increasing(first, last);
	is_strictly_decreasing = boost::is_strictly_decreasing(first, last);

	cout << "The sequence { ";
	for_each(first, last, cout << _1 << ' ');
	cout << " }..." << endl;

	output_sequence_property(is_increasing, "increasing");
	output_sequence_property(is_strictly_increasing, "strictly increasing");
	output_sequence_property(is_decreasing, "decreasing");
	output_sequence_property(is_strictly_decreasing, "strictly decreasing");
}

int main(void)
{
  const int sequence1[] = { 1, 2, 3, 4, 5 };
  const int sequence2[] = { 7, 7, 7, 7, 7 };
  const float sequence3[] = { 7.618, 4.971, 6.126, 1.727, 6.510 };

  analyze_sequence(begin(sequence1), end(sequence1));
  analyze_sequence(begin(sequence2), end(sequence2));
  analyze_sequence(begin(sequence3), end(sequence3));

  return 0;
}
