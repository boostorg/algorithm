/* 
   Copyright (c) David R. Musser & Gor V. Nishanov 1997.
   Copyright (c) Jeremy W. Murphy 2016.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#ifndef BOOST_ALGORITHM_SEARCH_DETAIL_MN_TRAITS
#define BOOST_ALGORITHM_SEARCH_DETAIL_MN_TRAITS

#include <boost/functional/hash.hpp>

#include <string>

namespace boost { namespace algorithm {

template <class T>
struct search_trait {
    enum {hash_range_max = 0};
    enum {suffix_size = 0};
    template <class RandomAccessIterator>
    inline static
    T hash(RandomAccessIterator) {
        return 0;
    }
};


template <>
struct search_trait<short unsigned> {
    enum {hash_range_max = 1u << sizeof(short unsigned) * 8};
    enum {suffix_size = sizeof(short unsigned)};
    template <class RandomAccessIterator>
    inline static
    short unsigned hash(RandomAccessIterator i) {
        return *i;
    }
};


template <> struct search_trait<char> {
    enum {hash_range_max = 256};
    enum {suffix_size = 1};
    template <class RandomAccessIterator>
    inline static 
    char unsigned hash(RandomAccessIterator i) {
        return *i;
    }
};

template <> struct search_trait<char signed> {
    enum {hash_range_max = 256};
    enum {suffix_size = 1};
    template <class RandomAccessIterator>
    inline static 
    char unsigned hash(RandomAccessIterator i) {
        return *i;              
    }
};

template <> struct search_trait<char unsigned> {
    enum {hash_range_max = 256};
    enum {suffix_size = 1};
    template <class RandomAccessIterator>
    inline static 
    char unsigned hash(RandomAccessIterator i) {
        return *i;              
    }
};

// NOTE: This std::string specialization is experimental.
// It simply fills the gap that would otherwise be here.
template <> struct search_trait<std::string> {
    enum {hash_range_max = 256};
    enum {suffix_size = 1};
    template <class RandomAccessIterator>
    inline static
    int hash(RandomAccessIterator i) {
        static boost::hash<std::string> string_hash;
        return string_hash(*i) % hash_range_max;
    }
};

struct search_trait_dna2 {
    enum {hash_range_max = 64};
    enum {suffix_size = 2};
    template <class RAI>
    inline static unsigned int hash(RAI i) {
        return (*(i-1) + ((*i) << 3)) & 63;
    }
};

struct search_trait_dna3 {
    enum {hash_range_max = 512};
    enum {suffix_size = 3};
    template <class RAI>
    inline static unsigned short int hash(RAI i) {
        return (*(i-2) + (*(i-1) << 3) + ((*i) << 6)) & 511;
    }
};

struct search_trait_dna4 {
    enum {hash_range_max = 256};
    enum {suffix_size = 4};
    template <class RAI>
    inline static unsigned int hash(RAI i) {
        return (*(i-3) + (*(i-2) << 2) + (*(i-1) << 4)
        + ((*i) << 6)) & 255;
    }
};

struct search_trait_dna5 {
    enum {hash_range_max = 256};
    enum {suffix_size = 5};
    template <class RAI>
    inline static unsigned int hash(RAI i) {
        return (*(i-4) + (*(i-3) << 2) + (*(i-2) << 4)
        + (*(i-1) << 6) + ((*i) << 8)) & 255;
    }
};

}} // namespace boost::algorithm

#endif

