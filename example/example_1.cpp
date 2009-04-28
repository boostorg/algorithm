// Boost.Algorithm example

#include <vector>
#include <iostream>

#include <boost/algorithm.hpp>

using namespace boost::algorithm;
using namespace boost::algorithm::sequence;

// Pretty-printer:
template <class T> 
void show(std::vector<T>& v);

// Toy function that ignores parameters:
int one_hundred(int&) { return 100; }

int main()
{
 int some_numbers[]     = { 0, 1, 32, 18, 5 };
 std::vector<int>       vi(some_numbers, some_numbers + 5);

 show(vi);

 // none(): See that the values are not all 100:
 std::cout << "Are none of the values equal to 100? "
           << ( none(vi, 100) ? "yes" : "no" ) << '\n'; 

 // apply(): Change all the values to 100:
 apply(vi, one_hundred);

 show(vi);

 // all(): See that all of the values are now 100:
 std:: cout <<  "Are any values equal to 100? " <<
                ( all(vi, 100) ? "yes" : "no" ) << '\n';
}

template <class T>
void show(std::vector<T>& v)
{
 for(typename std::vector<T>::const_iterator i = v.begin(); i != v.end(); ++i)
  std::cout << *i << '\n';

 std::cout << "----------" << std::endl;
}

/// \file example_1.cpp
/// \brief Some examples using the Boost::Algorithm library.
