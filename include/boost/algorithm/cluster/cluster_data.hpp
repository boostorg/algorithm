#if ! defined BOOST_ALGORITHM_CLUSTER_CLUSTER_DATA_HPP
#define BOOST_ALGORITHM_CLUSTER_CLUSTER_DATA_HPP

#include <boost/shared_ptr.hpp>
#include <vector>

namespace boost
{
namespace algorithm
{
namespace cluster
{

/*! TODO: Document this type.
 */
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

} // End of namespace cluster

// TODO: Should we be exporting this?
using namespace cluster;

} // End of namespace algorithm

} // End of namespace boost

#endif // BOOST_ALGORITHM_CLUSTER_CLUSTER_DATA_HPP
