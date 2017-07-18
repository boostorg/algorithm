/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2017

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#include <vector>
#include <iostream>
#include <random>

#include <boost/algorithm/shuffle_weighted.hpp>


namespace ba = boost::algorithm;

int main ( int /*argc*/, char * /*argv*/ [] )
{
    // WARNING: Example require C++11 or newer compiler
    std::random_device rd;
    std::mt19937 g(rd());
    {
        std::cout << "shuffle_weighted with iterators:\n";
        std::vector<int> vec{1, 2, 3, 4, 5}, order{4, 2, 3, 1, 0};

        ba::shuffle_weighted(vec.begin(), vec.end(), order.begin(), g);
        for (const auto& x : vec)
        {
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }
    {
        std::cout << "shuffle_weighted with ranges:\n";
        std::vector<int> vec{1, 2, 3, 4, 5}, order{4, 2, 3, 1, 0};

        ba::shuffle_weighted(vec, order, g);
        for (const auto& x : vec)
        {
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}

