/*
*  compiling from terminal:
*  g++ boost_icl.test.cpp -std=c++11 -isystem /dev/google/benchmark/include \
  -L /dev/google/benchmark/build/src -lbenchmark -lpthread -o boost_icl.testBenchmark
*
*/
#include <benchmark/benchmark.h>
#include <boost/icl/interval.hpp>
#include <boost/icl/interval_set.hpp>

static void BM_IntervalSetAdd( benchmark::State& state )
{
   boost::icl::interval< double >::type interval1( -1.0, 0.5 );
   boost::icl::interval< double >::type interval2( 0.0, 1.0 );
   boost::icl::interval_set< double > set1 = boost::icl::interval_set< double >( interval1 );
   boost::icl::interval_set< double > set2 = boost::icl::interval_set< double >( interval2 );
   for( auto _ : state )
      set1 + set2;
}

static void BM_IntervalSetSubtract( benchmark::State& state )
{
   boost::icl::interval< double >::type interval1( -1.0, 1.5 );
   boost::icl::interval< double >::type interval2( 0.0, 1.0 );
   boost::icl::interval_set< double > set1 = boost::icl::interval_set< double >( interval1 );
   boost::icl::interval_set< double > set2 = boost::icl::interval_set< double >( interval2 );
   for( auto _ : state )
      set1 - set2;
}

static void BM_IntervalSetContains( benchmark::State& state )
{
   boost::icl::interval< double >::type interval1( -1.0, 1.5 );
   boost::icl::interval< double >::type interval2( 0.0, 1.0 );
   for( auto _ : state )
      boost::icl::contains( interval1, interval2 );
}

BENCHMARK( BM_IntervalSetAdd );
BENCHMARK( BM_IntervalSetSubtract );
BENCHMARK( BM_IntervalSetContains );

BENCHMARK_MAIN();
