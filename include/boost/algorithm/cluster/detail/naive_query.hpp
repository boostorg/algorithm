//  (C) Copyright Jonathan Franklin 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if ! defined BOOST_ALGORITHM_CLUSTER_DETAIL_NAIVE_QUERY_HPP
#define BOOST_ALGORITHM_CLUSTER_DETAIL_NAIVE_QUERY_HPP

#include <boost/algorithm/cluster/cluster_data.hpp>
#include <boost/algorithm/cluster/concept.hpp>
#include <vector>

namespace boost
{
namespace algorithm
{
namespace cluster
{
namespace detail
{

// TODO: Replace this naive query function w/ R*-tree or fractional cascading.
// This query mechanism makes the runtime quadratic.
template<typename NTupleIterT, typename DistanceT, typename DistFunT>
static void naive_query(
  NTupleIterT const & query_pt,
  NTupleIterT const & begin,
  NTupleIterT const & end,
  DistanceT const & eps,
  DistFunT const & d,
  std::vector<NTupleIterT> & v)
{
  for(NTupleIterT cur_pt = begin; cur_pt != end; ++cur_pt)
  {
    if (query_pt == cur_pt)
      continue;

    if (d(*query_pt->tuple, *cur_pt->tuple) > eps)
      continue;

    v.push_back(cur_pt);
  }
}

} // End of namespace detail.
} // End of namespace cluster
} // End of namespace algorithm
} // End of namespace boost

#endif // BOOST_ALGORITHM_CLUSTER_DETAIL_NAIVE_QUERY_HPP
