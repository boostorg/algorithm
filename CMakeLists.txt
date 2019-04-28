# Copyright 2019 Sam Day
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt
#
# NOTE: CMake support for Boost.algorithm is currently experimental at best
#       and the interface is likely to change in the future

cmake_minimum_required(VERSION 3.5)
project(BoostAlgorithm LANGUAGES CXX)

add_library(boost_algorithm INTERFACE)
add_library(Boost::algorithm ALIAS boost_algorithm)

target_include_directories(boost_algorithm INTERFACE include)

target_link_libraries(boost_algorithm
        INTERFACE
        Boost::array
        Boost::assert
        Boost::bind
        Boost::concept_check
        Boost::config
        Boost::core
        Boost::detail
        Boost::exception
        Boost::function
        Boost::iterator
        Boost::mpl
        Boost::range
        Boost::regex
        Boost::static_assert
        Boost::throw_exception
        Boost::tuple
        Boost::type_traits
        Boost::unordered
        Boost::utility
        )
