/* 
   Copyright (c) Marshall Clow 2014.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

 Revision history:
    2 Dec 2014 mtc First version; power
   
*/

/// \file algorithm.hpp
/// \brief Misc Algorithms
/// \author Marshall Clow
///

#ifndef BOOST_ALGORITHM_HPP
#define BOOST_ALGORITHM_HPP

#include <boost/utility/enable_if.hpp> // for boost::disable_if

namespace boost { namespace algorithm {

/// \fn power ( T x, Integer n )
/// \return the value "x" raised to the power "n"
/// 
/// \param x     The value to be exponentiated
/// \param n     The exponent
///
//	\remark Taken from Knuth, The Art of Computer Programming, Volume 2:
//		Seminumerical Algorithms, Section 4.6.3
template <typename T, typename Integer>
T power (T x, Integer n) {
	T y = 1; // Should be "T y{1};" 
	if (n == 0) return y;
	while (true) {
		if (n % 2 == 1) {
			y = x * y;
			if (n == 1)
				return y;
			}
		n = n / 2;
		x = x * x;
		}
	return y;
	}

}}

#endif // BOOST_ALGORITHM_HPP
