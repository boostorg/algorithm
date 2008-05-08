#if ! defined BOOST_ALGORITHM_CLUSTER_DBSCAN_HPP
#define BOOST_ALGORITHM_CLUSTER_DBSCAN_HPP

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <list>

namespace boost
{
namespace algorithm
{
namespace cluster
{

namespace detail
{

// TODO: Replace this naive query function w/ R*-tree or fractional cascading.
// It makes the runtime quadratic.
template<typename NTupleIter, typename DistFun>
static void query(
  NTupleIter const & query_pt,
  NTupleIter const & begin,
  NTupleIter const & end,
  float eps,
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

// TODO: Replace this so we don't have to store the cluster info for each tuple.
template<typename NTupleIter>
struct node
{
  node(NTupleIter const & t) : tuple(t), cluster(UNCLASSIFIED) {}

  NTupleIter tuple;
  int cluster;
};

} // End of namespace detail.

// TODO: Document this type.
template<typename Cluster>
struct cluster_data
{
  typedef Cluster value_type;
  typedef std::vector<value_type> clusters;
  cluster_data() : m_pClusters(new clusters) {}
  ~cluster_data() {}

  cluster_data(cluster_data const & c) : m_pClusters(c.m_pClusters) {}
  cluster_data const & cluster_data::operator=(cluster_data const & rhs)
  { m_pClusters = rhs.m_pClusters; }

  typedef typename clusters::iterator iterator;
  typedef typename clusters::const_iterator const_iterator;
  typedef typename clusters::reverse_iterator reverse_iterator;

  iterator begin() { return m_pClusters->begin(); }
  iterator end() { return m_pClusters->end(); }

  const_iterator begin() const { return m_pClusters->begin(); }
  const_iterator end() const { return m_pClusters->end(); }

  iterator rbegin() { return m_pClusters->rbegin(); }
  iterator rend() { return m_pClusters->rend(); }

  iterator insert(iterator loc, value_type const & val)
  { return m_pClusters->insert(loc, val); }

  void push_back(value_type const & v) { m_pClusters->push_back(v); }
  void pop_back() { m_pClusters->pop_back(); }

  value_type & back() { return m_pClusters->back(); }
  value_type const & back() const { return m_pClusters->back(); }

private:
  boost::shared_ptr<clusters> m_pClusters;
};

/**
 */
template<typename Cluster, typename NTupleIter, typename DistFun>
cluster_data<Cluster>
dbscan(NTupleIter const & begin,
       NTupleIter const & end, 
       typename NTupleIter::difference_type const & eps,
       size_t min_points,
       DistFun const & d)
{
  // TODO: Rework the algorithm to NOT make this extra collection.
  typedef detail::node<NTupleIter> node;
  typedef std::vector<node> ntuple_nodes;
  ntuple_nodes tuples;

  // Initialize algorithm.
  //size_t num_elems = 0;
  for(NTupleIter it = begin; it != end; ++it)
  {
    //++num_elems;
    //it->cluster = UNCLASSIFIED;
    tuples.push_back(node(it));
  }

  typedef cluster_data<std::vector<NTupleIter> > cluster_data;
  cluster_data p;

  // Do it...
  int cluster_num = 0;
  for(ntuple_nodes::iterator it = tuples.begin(); it != tuples.end(); ++it)
  {
    if (it->cluster != UNCLASSIFIED) // Been classified.
      continue;

    // Expand cluster.

    std::vector<ntuple_nodes::iterator> seeds;
    detail::query(it, tuples.begin(), tuples.end(), eps, d, seeds);
    if (seeds.size() < min_points)
    {
      it->cluster = NOISE;
      continue;
    }

    // Start the next cluster.
    ++cluster_num;
    p.push_back(Cluster());
    Cluster & cur_cluster = p.back();

    // Mark entire neighborhood as part of current cluster.
    it->cluster = cluster_num;
    cur_cluster.push_back(it->tuple);
    // TODO: Remove it from noise.
    for (size_t n = 0; n < seeds.size(); ++n)
    {
      seeds[n]->cluster = cluster_num;
      cur_cluster.push_back(seeds[n]->tuple);
      // TODO: Remove it from noise.
    }

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

using namespace cluster;

} // End of namespace algorithm

} // End of namespace boost

#endif // BOOST_ALGORITHM_CLUSTER_DBSCAN_HPP
