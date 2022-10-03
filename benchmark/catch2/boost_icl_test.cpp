#include <iostream>
#include <boost/icl/interval.hpp>
#include <boost/icl/interval_set.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/generators/catch_generators.hpp>

/*
 * Regular use case. Every element is included.
 */
static int mixture();

/*
 * Tests the creation performance of interval objects.
 */
static int interval_test();

/*
 * Tests the intersection operation performance.
 */
static int intersection_test();

/*
 * Tests the union operation performance.
 */
static int union_test();

/*
 * Tests the difference operation performance.
 */
static int difference_test();


TEST_CASE("main") {

    BENCHMARK("Mixture Test") { return mixture(); };

    BENCHMARK("Creation of intervals") { return interval_test(); };

    BENCHMARK("Intersection") { return intersection_test(); };

    BENCHMARK("Union") { return union_test(); };

    BENCHMARK("Difference") { return difference_test(); };
    
}

static int mixture() {

    boost::icl::interval<double>::type interval1 = boost::icl::interval<double>::closed(34.5, 78.9);
    boost::icl::interval<double>::type interval2(11.8, 68.5);
    boost::icl::interval<double>::type interval3(0.0, 1.0);
    boost::icl::interval<double>::type interval4(99.8, 100.0);

    boost::icl::interval_set<double> intersection_set;
    boost::icl::interval_set<double> union_set;
    boost::icl::interval_set<double> difference_set;

    intersection_set = boost::icl::interval_set<double>(interval1);
    intersection_set &= boost::icl::interval_set<double>(interval2);

    union_set = boost::icl::interval_set<double>(interval1) +
                boost::icl::interval_set<double>(interval2) +
                boost::icl::interval_set<double>(interval3) +
                boost::icl::interval_set<double>(interval4);

    difference_set = static_cast <boost::icl::interval_set<double> >(interval1);
    difference_set -= boost::icl::interval_set<double>(interval2);
    difference_set -= boost::icl::interval_set<double>(interval3);

    return 0;
}

static int interval_test() {

    boost::icl::interval<double>::type interval1(34.5, 78.9);
    boost::icl::interval<double>::type interval2(11.8, 68.5);
    boost::icl::interval<double>::type interval3(0.0, 1.0);
    boost::icl::interval<double>::type interval4(99.8, 100.0);
    boost::icl::interval<double>::type interval5(99.8, 100.0);
    boost::icl::interval<double>::type interval6(99.8, 100.0);
    boost::icl::interval<double>::type interval7(99.8, 100.0);
    boost::icl::interval<double>::type interval8(99.8, 100.0);
    boost::icl::interval<double>::type interval9(99.8, 100.0);
    boost::icl::interval<double>::type interval10(99.8, 100.0);

    boost::icl::interval<double>::type interval11 = boost::icl::interval<double>::closed(34.5, 78.9);
    boost::icl::interval<double>::type interval12 = boost::icl::interval<double>::closed(34.5, 78.9);
    boost::icl::interval<double>::type interval13 = boost::icl::interval<double>::closed(34.5, 78.9);
    boost::icl::interval<double>::type interval14 = boost::icl::interval<double>::closed(34.5, 78.9);
    boost::icl::interval<double>::type interval15 = boost::icl::interval<double>::closed(34.5, 78.9);
    boost::icl::interval<double>::type interval16 = boost::icl::interval<double>::closed(34.5, 78.9);
    boost::icl::interval<double>::type interval17 = boost::icl::interval<double>::closed(34.5, 78.9);
    boost::icl::interval<double>::type interval18 = boost::icl::interval<double>::closed(34.5, 78.9);
    boost::icl::interval<double>::type interval19 = boost::icl::interval<double>::closed(34.5, 78.9);
    boost::icl::interval<double>::type interval20 = boost::icl::interval<double>::closed(34.5, 78.9);

    boost::icl::interval<double>::type interval21 = boost::icl::interval<double>::left_open(34.5, 78.9);
    boost::icl::interval<double>::type interval22 = boost::icl::interval<double>::left_open(34.5, 78.9);
    boost::icl::interval<double>::type interval23 = boost::icl::interval<double>::left_open(34.5, 78.9);
    boost::icl::interval<double>::type interval24 = boost::icl::interval<double>::left_open(34.5, 78.9);
    boost::icl::interval<double>::type interval25 = boost::icl::interval<double>::left_open(34.5, 78.9);
    boost::icl::interval<double>::type interval26 = boost::icl::interval<double>::left_open(34.5, 78.9);
    boost::icl::interval<double>::type interval27 = boost::icl::interval<double>::left_open(34.5, 78.9);
    boost::icl::interval<double>::type interval28 = boost::icl::interval<double>::left_open(34.5, 78.9);
    boost::icl::interval<double>::type interval29 = boost::icl::interval<double>::left_open(34.5, 78.9);
    boost::icl::interval<double>::type interval30 = boost::icl::interval<double>::left_open(34.5, 78.9);

    boost::icl::interval<double>::type interval31 = boost::icl::interval<double>::right_open(34.5, 78.9);
    boost::icl::interval<double>::type interval32 = boost::icl::interval<double>::right_open(34.5, 78.9);
    boost::icl::interval<double>::type interval33 = boost::icl::interval<double>::right_open(34.5, 78.9);
    boost::icl::interval<double>::type interval34 = boost::icl::interval<double>::right_open(34.5, 78.9);
    boost::icl::interval<double>::type interval35 = boost::icl::interval<double>::right_open(34.5, 78.9);
    boost::icl::interval<double>::type interval36 = boost::icl::interval<double>::right_open(34.5, 78.9);
    boost::icl::interval<double>::type interval37 = boost::icl::interval<double>::right_open(34.5, 78.9);
    boost::icl::interval<double>::type interval38 = boost::icl::interval<double>::right_open(34.5, 78.9);
    boost::icl::interval<double>::type interval39 = boost::icl::interval<double>::right_open(34.5, 78.9);
    boost::icl::interval<double>::type interval40 = boost::icl::interval<double>::right_open(34.5, 78.9);

    return 0;

}

static int intersection_test() {

    boost::icl::interval<double>::type interval1(0.0, 100.0);
    boost::icl::interval<double>::type interval2(10.0, 90.0);
    boost::icl::interval<double>::type interval3(20.0, 80.0);
    boost::icl::interval<double>::type interval4(30.0, 70.0);
    boost::icl::interval<double>::type interval5(40.0, 60.0);
    boost::icl::interval<double>::type interval6(45.0, 55.0);

    boost::icl::interval_set<double> intersection_set;

    intersection_set = boost::icl::interval_set<double>(interval1);
    intersection_set &= boost::icl::interval_set<double>(interval2);
    intersection_set &= boost::icl::interval_set<double>(interval3);
    intersection_set &= boost::icl::interval_set<double>(interval4);
    intersection_set &= boost::icl::interval_set<double>(interval5);
    intersection_set &= boost::icl::interval_set<double>(interval6);

    return 0;
}

static int union_test() {

    boost::icl::interval<double>::type interval1(0.0, 100.0);
    boost::icl::interval<double>::type interval2(10.0, 90.0);
    boost::icl::interval<double>::type interval3(20.0, 80.0);
    boost::icl::interval<double>::type interval4(30.0, 70.0);
    boost::icl::interval<double>::type interval5(40.0, 60.0);
    boost::icl::interval<double>::type interval6(45.0, 55.0);

    boost::icl::interval_set<double> union_set;

    union_set = boost::icl::interval_set<double>(interval1) +
                boost::icl::interval_set<double>(interval2) +
                boost::icl::interval_set<double>(interval3) +
                boost::icl::interval_set<double>(interval4) +
                boost::icl::interval_set<double>(interval5) +
                boost::icl::interval_set<double>(interval6);

    return 0;
}

static int difference_test() {

    boost::icl::interval<double>::type interval1(0.0, 100.0);
    boost::icl::interval<double>::type interval2(10.0, 90.0);
    boost::icl::interval<double>::type interval3(20.0, 80.0);
    boost::icl::interval<double>::type interval4(30.0, 70.0);
    boost::icl::interval<double>::type interval5(40.0, 60.0);
    boost::icl::interval<double>::type interval6(45.0, 55.0);

    boost::icl::interval_set<double> difference_set;

    difference_set =  boost::icl::interval_set<double>(interval1);
    difference_set -= boost::icl::interval_set<double>(interval2);
    difference_set -= boost::icl::interval_set<double>(interval3);
    difference_set -= boost::icl::interval_set<double>(interval4);
    difference_set -= boost::icl::interval_set<double>(interval5);
    difference_set -= boost::icl::interval_set<double>(interval6);

    return 0;
}



















