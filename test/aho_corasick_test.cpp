/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016
  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
  See http://www.boost.org/ for latest version.
*/

#include <boost/config.hpp>
#include <boost/algorithm/searching/aho_corasick.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <utility>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>


namespace ba = boost::algorithm;
const std::vector<std::vector<std::string>> patterns({std::vector<std::string>({"he", "is", "she", "his", "her", "h", "hishera", "azaza"}),
                                                      std::vector<std::string>({"he", "she", "his", "her", "he", "usher", "d sh", "she hi"}),
                                                      std::vector<std::string>({"he", "she", "his", "her", "he", "usher", "d sh", "she hi"})});

const std::vector<std::string> corpus({"hisher",
                                       "usher and she he her",
                                       ""});

const std::vector<std::vector<std::string>> rightResults({std::vector<std::string>({"h", "his", "is", "h", "she", "he", "her"}),
                                                          std::vector<std::string>({"she", "he", "he", "usher", "her", "d sh", "she",
                                                                                    "he", "he", "he", "he", "he", "he", "her"}),
                                                          std::vector<std::string>()});;
template<typename T, typename Cont1, typename Cont2>
void fromIteratorsToContainer(const Cont1& from, Cont2& to)
{
    for (const auto &val: from)
    {
        T str;
        auto begin = val.first;
        auto end = val.second;
        while (begin != end)
        {
            str += *begin;
            ++begin;
        }
        to.push_back(std::move(str));
    }
}

void test_aho_corasick()
{
    BOOST_CHECK(patterns.size() == corpus.size());
    //aho_corasick_map
    for(size_t i = 0; i < patterns.size(); ++i)
    {
        std::vector<std::pair<std::string::const_iterator, std::string::const_iterator>> res;
        std::vector<std::string> localResult;
        ba::aho_corasick_map<char>(corpus[i].begin(), corpus[i].end(), patterns[i].begin(), patterns[i].end(), 
				   [&res](std::string::const_iterator begin, std::string::const_iterator end) -> bool
                                   { res.push_back({begin, end}); return true; });
        fromIteratorsToContainer<std::string>(res, localResult);
        BOOST_CHECK(localResult == rightResults[i]);
    }

    //aho_corasick_hashmap
    for(size_t i = 0; i < patterns.size(); ++i)
    {
        std::vector<std::pair<std::string::const_iterator, std::string::const_iterator>> res;
        std::vector<std::string> localResult;
        ba::aho_corasick_hashmap<char>(corpus[i].begin(), corpus[i].end(), patterns[i].begin(), patterns[i].end(), 
				       [&res](std::string::const_iterator begin, std::string::const_iterator end) -> bool
				       { res.push_back({begin, end}); return true; });
        fromIteratorsToContainer<std::string>(res, localResult);
        BOOST_CHECK(localResult == rightResults[i]);
    }

    //Aho_Corasick_Map
    for(size_t i = 0; i < patterns.size(); ++i)
    {
        std::vector<std::pair<std::string::const_iterator, std::string::const_iterator>> res;
        std::vector<std::string> localResult;
        ba::Aho_Corasick_Map<char> obj(patterns[i].begin(), patterns[i].end());
        obj(corpus[i].begin(), corpus[i].end(), 
	    [&res](std::string::const_iterator begin, std::string::const_iterator end) -> bool
            { res.push_back({begin, end}); return true; });
        fromIteratorsToContainer<std::string>(res, localResult);
        BOOST_CHECK(localResult == rightResults[i]);
    }

    //Aho_Corasick_HashMap
    for(size_t i = 0; i < patterns.size(); ++i)
    {
        std::vector<std::pair<std::string::const_iterator, std::string::const_iterator>> res;
        std::vector<std::string> localResult;
        ba::Aho_Corasick_HashMap<char> obj(patterns[i].begin(), patterns[i].end());
        obj(corpus[i].begin(), corpus[i].end(), 
	    [&res](std::string::const_iterator begin, std::string::const_iterator end) -> bool
            { res.push_back({begin, end}); return true; });
        fromIteratorsToContainer<std::string>(res, localResult);
        BOOST_CHECK(localResult == rightResults[i]);
    }

    //General AhoCorasick
    for(size_t i = 0; i < patterns.size(); ++i)
    {
        std::vector<std::pair<std::string::const_iterator, std::string::const_iterator>> res;
        std::vector<std::string> localResult;
        ba::AhoCorasick<char, std::unordered_map, std::hash<char>, std::equal_to<char>> obj(patterns[i].begin(), patterns[i].end());
        obj(corpus[i].begin(), corpus[i].end(), 
	    [&res](std::string::const_iterator begin, std::string::const_iterator end) -> bool
            { res.push_back({begin, end}); return true; });
        fromIteratorsToContainer<std::string>(res, localResult);
        BOOST_CHECK(localResult == rightResults[i]);
    }
}

BOOST_AUTO_TEST_CASE( test_main )
{
    test_aho_corasick();
}