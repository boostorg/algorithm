//  (C) Copyright Jonathan Franklin 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if ! defined BOOST_ALGORITHM_CLUSTER_CONCEPT_HPP
#define BOOST_ALGORITHM_CLUSTER_CONCEPT_HPP

#include <boost/concept_check.hpp>

namespace boost
{
namespace algorithm
{
namespace cluster
{

  // TODO: Document the purpose of this concept.
  template<typename T, typename DistanceFunT>
  struct DistanceComparableConcept
  {
    void constraints()
    {
      // Operation
      d(t, t);
    }
  private:
    T t;
    DistanceFunT d;
  };

  // TODO: Add concepts here, then delete this comment.

} // End of namespace cluster;
} // End of namespace algorithm;
} // End of namespace boost;

#endif // BOOST_ALGORITHM_CLUSTER_CONCEPT_HPP
