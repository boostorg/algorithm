#include <boost/algorithm/is_palindrome.hpp>
#include <boost/chrono.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using boost::algorithm::is_palindrome;
namespace po = boost::program_options;

void run_is_palindrome_test(size_t iterations) {
    // Prepare test cases: small, medium, large, palindromes and non-palindromes
    std::vector<std::string> test_cases = {
        "madam", "racecar", "level", "hello", "world", // small
        std::string(100, 'a'), // medium palindrome
        std::string(42, 'a') + "b" + std::string(57, 'a'), // medium non-palindrome
        std::string(10000, 'x'), // large palindrome
        std::string(3248, 'y') + "z" + std::string(6751, 'y'), // large non-palindrome
    };

    // Make sure some are not palindromes
    test_cases.push_back("abcdefg");
    test_cases.push_back("abccba");
    test_cases.push_back("abccbx");

    // Warm up
    volatile bool dummy = false;
    for (const auto& s : test_cases) {
        dummy ^= is_palindrome(s);
    }

    // Start timing
    boost::chrono::high_resolution_clock::time_point start = boost::chrono::high_resolution_clock::now();

    size_t palindrome_count = 0;
    for (size_t i = 0; i < iterations; ++i) {
        for (const auto& s : test_cases) {
            if (is_palindrome(s)) {
                ++palindrome_count;
            }
        }
    }

    boost::chrono::high_resolution_clock::time_point end = boost::chrono::high_resolution_clock::now();
    boost::chrono::duration<double> elapsed = end - start;

    size_t total_runs = iterations * test_cases.size();
    double avg_time_per_run = elapsed.count() / total_runs * 1e6; // microseconds

    std::cout << "Total runs: " << total_runs << std::endl;
    std::cout << "Total palindromes found: " << palindrome_count << std::endl;
    std::cout << "Total elapsed time: " << elapsed.count() << " seconds" << std::endl;
    std::cout << "Average time per is_palindrome: " << avg_time_per_run << " microseconds" << std::endl;
}

int main(int argc, char* argv[]) {
    size_t iterations = 1000000;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("iterations,n", po::value<size_t>(&iterations)->default_value(1000000), "number of iterations")
    ;

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const std::exception& ex) {
        std::cerr << "Error parsing options: " << ex.what() << std::endl;
        return 1;
    }

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    run_is_palindrome_test(iterations);
    return 0;   // this is not a correctness test; there are others for that
}
