#include <iostream>
#include <boost/icl/interval.hpp>
#include <boost/icl/interval_set.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>


static boost::icl::interval<double>::type interval1(0.0, 100.0);
static boost::icl::interval<double>::type interval2(10.0, 90.0);


static boost::icl::interval_set<double> interval_set1(interval1);
static boost::icl::interval_set<double> interval_set2(interval2);


TEST_CASE("main") {

    BENCHMARK("Add") {
        interval_set1 + interval_set2;
        return 0;
    };

    BENCHMARK("Subtract") {
        interval_set1 + interval_set2;
        return 0;
    };

    BENCHMARK("Contains") {
        boost::icl::contains(interval_set1, interval_set2);
        return 0;
    };
    
}











