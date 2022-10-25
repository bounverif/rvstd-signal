#define CATCH_CONFIG_MAIN
#include "bool_and_vec/interval_set.hpp"
#include <catch2/catch_test_macros.hpp>

#define ns rvstd::bool_and_vec

TEST_CASE( "construction with new keyword" )
{
   ns::interval_set< int >* p_is = new ns::interval_set< int >;
   CHECK( p_is != nullptr );
   CHECK( p_is->empty() );
}

TEST_CASE( "construction as empty" )
{
   ns::interval_set< int > is;
   CHECK( is.empty() );
}

TEST_CASE( "construction with initilazer list for type::chrono" )
{
   typedef std::chrono::duration< int > seconds_type;
   seconds_type time0( 1 );
   seconds_type time1( 8 );
   seconds_type time2( 11 );
   seconds_type time3( 17 );
   ns::interval_set< seconds_type > is = { { time0, time1 }, { time2, time3 } };
   CHECK( !is.empty() );
}

TEST_CASE( "construction common initilazer list with invalid interval" )
{
   SECTION( "left is lower than right" )
   {
      ns::interval_set< int > is0;
      CHECK_THROWS( is0 = { { 1, 2 }, { 7, 5 } } );
   }
   SECTION( "left is lower than previous right" )
   {
      ns::interval_set< int > is1;
      CHECK_THROWS( is1 = { { 1, 2 }, { 7, 5 } } );
   }
}

TEST_CASE( "construction common initilazer list with equal values" )
{
   SECTION( "left and right is equal" )
   {
      ns::interval_set< int > is0 = { { 1, 2 }, { 7, 7 } };
      CHECK( !is0.empty() );
      CHECK( is0.size() == 1 );
   }
   SECTION( "left is equal to previous right" )
   {
      ns::interval_set< int > is1 = { { 1, 2 }, { 2, 7 } };
      CHECK( !is1.empty() );
      CHECK( is1.size() == 1 );
   }
}

TEST_CASE( "construction with common initilazer list" )
{
   ns::interval_set< int > is = { { 1, 2 }, { 3, 7 }, { 9, 10 }, { 11, 22 } };
   CHECK( !is.empty() );
   CHECK( is.size() == 4 );
}

TEST_CASE( "using reserve function " )
{
   ns::interval_set< int > is0;
   REQUIRE( is0.empty() );

   SECTION( "calling reserve function" )
   {
      is0.reserve( 100 );
      CHECK( is0.capacity() == 100 );
   }
}

TEST_CASE( "getting size of interval_set" )
{
   ns::interval_set< int > is0;
   REQUIRE( is0.empty() );

   ns::interval_set< int > is1 = { { 1, 8 }, { 11, 18 } };
   REQUIRE( !is1.empty() );

   SECTION( "size function for empty" )
   {
      CHECK( is0.size() == 0 );
   }

   SECTION( "size function for non-empty" )
   {
      CHECK( is1.size() == 2 );
   }
}

TEST_CASE( "checking a time values with at() function " )
{
   ns::interval_set< int > is0;
   REQUIRE( is0.empty() );

   ns::interval_set< int > is1 = { { 1, 8 }, { 11, 18 } };
   REQUIRE( !is1.empty() );

   SECTION( "checking empty interval set" )
   {
      CHECK( is0.at( 5 ) == false );
   }
   SECTION( " checking mid values " )
   {
      CHECK( is1.at( 9 ) == false );
      CHECK( is1.at( 3 ) == true );
   }
   SECTION( " checking value when signal changed" )
   {
      CHECK( is1.at( 1 ) == true );
      CHECK( is1.at( 8 ) == false );
   }
   SECTION( " cheking value at start and infinity" )
   {
      CHECK( is1.at( 0 ) == false );
      CHECK( is1.at( INT_MAX ) == false );
   }
}

TEST_CASE( "appending one interval to the set " )
{
   ns::interval_set< int > is0;
   REQUIRE( is0.empty() );

   SECTION( "adding to empty set " )
   {
      std::pair< int, int > p( 2, 7 );
      is0.append( p );
      CHECK( is0.at( 3 ) == true );
      CHECK( is0.size() == 1 );
      // is0 -> { 2, 7 } { T, F }
   }

   SECTION( "adding to non-empty set " )
   {
      std::pair< int, int > p( 15, 17 );
      is0.append( p );
      CHECK( is0.at( 16 ) == true );
      CHECK( is0.size() == 2 );
      // is0 -> { 2, 7, 15, 17 } { T, F, T, F }
   }

   SECTION( "adding to mid of set " )
   {
      std::pair< int, int > p( 16, 22 );
      CHECK_THROWS( is0.append( p ) );
      // is0 -> { 2, 7, 15, 17 } { T, F, T, F }
   }

   SECTION( "adding to exact end of set " )
   {
      std::pair< int, int > p( 17, 22 );
      is0.append( p );
      CHECK( is0.at( 19 ) == true );
      CHECK( is0.at( 17 ) == true );
      CHECK( is0.size() == 5 );
      // is0 -> { 2, 7, 15, 22 } { T, F, T, F }
   }
}