#if ! defined BOOST_ALGORITHM_CLUSTER_DBSCAN_HPP
#define BOOST_ALGORITHM_CLUSTER_DBSCAN_HPP

#include <boost/algorithm/cluster/cluster_data.hpp>
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
template<typename NTupleIter, typename DistFun>
static void query(
  NTupleIter const & query_pt,
  NTupleIter const & begin,
  NTupleIter const & end,
  typename NTupleIter::difference_type eps,
  DistFun const & d,
  std::vector<NTupleIter> & v)
{
  for(NTupleIter cur_pt = begin; cur_pt != end; ++cur_pt)
  {
    if (query_pt == cur_pt)
      continue;

    if (d(*query_pt->tuple, *cur_pt->tuple) > eps)
      continue;

    v.push_back(cur_pt);
  }
}

// TODO: Replace this so we don't have to store the cluster info for each tuple?
template<typename NTupleIter>
struct node
{
  node(NTupleIter const & t) : tuple(t), cluster(UNCLASSIFIED) {}

  NTupleIter tuple;
  int cluster;
};

} // End of namespace detail.

/*! DBSCAN density-based clustering algorithm.
 * TODO: Document this function.
 * \param[in] begin
 * \param[in] end
 * \param[in] eps
 * \param[in] min_points
 * \param[in] d
 * \return The cluster data (partitioning of the tuples).
 */
template<typename Cluster, typename NTupleIter, typename DistFun>
cluster_data<Cluster>
dbscan(NTupleIter const & begin,
       NTupleIter const & end, 
       typename NTupleIter::difference_type const & eps,
       size_t min_points,
       DistFun const & d)
{
  int const UNCLASSIFIED = -1;
  int const NOISE = 0;

  // TODO: Rework the algorithm to NOT make this extra collection?
  typedef detail::node<NTupleIter> node;
  typedef std::vector<node> ntuple_nodes;
  ntuple_nodes tuples;

  // Initialize algorithm.
  //size_t num_elems = 0;
  for(NTupleIter it = begin; it != end; ++it)
  {
    //++num_elems;
    tuples.push_back(node(it));
  }

  typedef cluster_data<std::vector<NTupleIter> > cluster_data;
  cluster_data p;

  // TODO: We should try to make cluster_num go away.
  int cluster_num = 0;
  for(ntuple_nodes::iterator it = tuples.begin(); it != tuples.end(); ++it)
  {
    // Skip this tuple if its already been classified as a cluster or noise.
    if (it->cluster != UNCLASSIFIED)
      continue;

    // Expand cluster.

    std::vector<ntuple_nodes::iterator> seeds;
    detail::query(it, tuples.begin(), tuples.end(), eps, d, seeds);
    // If the neighborhood of this tuple is too small, then mark it as noise.
    if (seeds.size() < min_points)
    {
      it->cluster = NOISE;
      continue;
    }

    // Start the next cluster.
    ++cluster_num;
    p.push_back(Cluster()); // TODO: This is goofy.
    Cluster & cur_cluster = p.back();

    // Mark entire neighborhood as part of the current cluster.
    it->cluster = cluster_num;
    cur_cluster.push_back(it->tuple);
    for (size_t n = 0; n < seeds.size(); ++n)
    {
      seeds[n]->cluster = cluster_num;
      cur_cluster.push_back(seeds[n]->tuple);
    }

    // Keep adding seeds and processing them until we find all points that
    // are Density Reachable.
    while (! seeds.empty())
    {
      ntuple_nodes::iterator cur = seeds.back();
      seeds.pop_back();

      std::vector<ntuple_nodes::iterator> results;
      detail::query(cur, tuples.begin(), tuples.end(), eps, d, results);

      if (results.size() >= min_points)
      {
        for (size_t n = 0; n < results.size(); ++n)
        {
          if (results[n]->cluster < 1) // Not assigned to cluster yet.
          {
            if (UNCLASSIFIED == results[n]->cluster)
              seeds.push_back(results[n]);
            results[n]->cluster = cluster_num;
            cur_cluster.push_back(results[n]->tuple);
          }
        }
      }
    }

  } // Outer loop for all tuples.

  return p;
}

} // End of namespace cluster

// TODO: Should we be exporting this?
using namespace cluster;

} // End of namespace algorithm

} // End of namespace boost

#endif // BOOST_ALGORITHM_CLUSTER_DBSCAN_HPP
