
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <iterator>
#include <vector>
#include <list>
#include <deque>

#include <boost/algorithm/integer_sort/radix-sort.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/random.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/bind.hpp>

using namespace boost::algorithm;

typedef boost::mpl::list<unsigned char, unsigned short, unsigned int, unsigned long> all_unsigned_types;


BOOST_AUTO_TEST_CASE_TEMPLATE(empty, T, all_unsigned_types)
{
    // So empty, it would be a crime to dereference it.
    T *const p = NULL, * const result = NULL;
    stable_radix_sort(p, p, result);
    // No BOOST_REQUIRE... we just make sure the program does not crash/throw.
}


template <typename T>
struct count_one
{
    std::vector<T> result;
    
    count_one()
    {
        result.resize(1);
    }
    
    template <typename Container>
    void operator()(Container)
    {
        result[0] = 9;
        Container const c(1, 0);
        stable_radix_sort(c.begin(), c.end(), result.begin());
        BOOST_CHECK(std::equal(result.begin(), result.end(), c.begin()));
    }
};


/* n == 1 is a special case; fundamentally, there is no problem, the data is 
 * already in order.
 */
BOOST_AUTO_TEST_CASE_TEMPLATE(n_equals_one, T, all_unsigned_types)
{
    typedef typename boost::mpl::transform<boost::mpl::list< std::vector<boost::mpl::_1>, std::deque<boost::mpl::_1>, std::list<boost::mpl::_1> >, boost::mpl::apply1<boost::mpl::_1, T> >::type sequence_containers;
    boost::mpl::for_each<sequence_containers>(count_one<T>());
}


template <typename T>
class one_1000_foo
{
private:
    std::vector<T> input;
    std::vector<T> result;
    
public:
    one_1000_foo(std::vector<T> const &input) : input(input)
    {
        result.resize(input.size());
    }
    
    template <typename Container>
    void operator()(Container)
    {
        std::fill(result.begin(), result.end(), 9);
        Container const c(input.begin(), input.end());
        stable_radix_sort(c.begin(), c.end(), result.begin());
        BOOST_CHECK(std::equal(c.begin(), c.end(), result.begin()));
    }
};



BOOST_AUTO_TEST_CASE_TEMPLATE(one_1000, T, all_unsigned_types)
{
    typedef typename boost::mpl::transform<boost::mpl::list< std::vector<boost::mpl::_1>, std::deque<boost::mpl::_1>, std::list<boost::mpl::_1> >, boost::mpl::apply1<boost::mpl::_1, T> >::type test_containers;
    
    unsigned int const n = 1000;
    std::vector<T> const input(n, 1);
    
    boost::mpl::for_each<test_containers>(one_1000_foo<T>(input));
}


template <typename T>
struct random_k
{
    std::vector<T> input;
    
    random_k(T k, unsigned n)
    {
        boost::random::mt19937 gen(0);
        boost::random::uniform_int_distribution<T> dist(0, k);
        std::generate_n(std::back_inserter(input), n, boost::bind(dist, gen));
    }
    
    template <typename Container>
    void operator()(Container)
    {
        Container const c(input.begin(), input.end());
        std::vector<T> result1;
        result1.resize(input.size());
        stable_radix_sort(c.begin(), c.end(), result1.begin());
        std::vector<T> result2(input.begin(), input.end());
        std::stable_sort(result2.begin(), result2.end());
        BOOST_CHECK(std::equal(result1.begin(), result1.end(), result2.begin()));
    }
};


BOOST_AUTO_TEST_CASE_TEMPLATE(random_small_k, T, all_unsigned_types)
{
    typedef typename boost::mpl::transform<boost::mpl::list< std::vector<boost::mpl::_1>, std::deque<boost::mpl::_1>, std::list<boost::mpl::_1> >, boost::mpl::apply1<boost::mpl::_1, T> >::type test_containers;

    T const k = ~T(0);
    unsigned n = 1000;
    boost::mpl::for_each<test_containers>(random_k<T>(k, n));
}
