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

TEST_CASE( "Union operation" ) {
   ns::interval_set< int > is1 { { { 1, 3 }, { 9, 11 } }, false };
   ns::interval_set< int > is2 { { { 5, 7 } }, false };
   ns::interval_set< int > is3 { { { 1, 2 } }, false };
   ns::interval_set< int > is4 { { { 1, 14 } }, false };
   ns::interval_set< int > is5 { { { 2, 4 } }, false };
   ns::interval_set< int > is6 { { { 2, 10 } }, false };

   SECTION("Union with non-intersecting sets") {
      ns::interval_set< int > true_val { {1, 3}, {5, 7}, {9, 11} };
      auto test_val = is1 + is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Union with two overlapping sets") {
      ns::interval_set< int > true_val { {1, 3}, {9, 11} };
      auto test_val = is1 + is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Union with completely encapsulating set") {
      ns::interval_set< int > true_val { {1, 14} };
      auto test_val = is1 + is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Union with partially overlapped set") {
      ns::interval_set< int > true_val { {1, 4}, {9, 11} };
      auto test_val = is1 + is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Union with partially encapsulating set") {
      ns::interval_set< int > true_val { {1, 11} };
      auto test_val = is1 + is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

}

TEST_CASE( "Intersection operation" ) {

   ns::interval_set< int > is1 { { { 2, 7 }, { 15, 21 } }, false };
   ns::interval_set< int > is2 { { { 3, 4 } }, false };
   ns::interval_set< int > is3 { { { 3, 4 }, { 16, 20 } }, false };
   ns::interval_set< int > is4 { { { 2, 7 } }, false };
   ns::interval_set< int > is5 { { { 2, 7 }, { 15, 21 } }, false };
   ns::interval_set< int > is6 { { { 5, 18 } }, false };
   ns::interval_set< int > is7 { { { 8, 10 } }, false };
   ns::interval_set< int > is8 { { { 7, 15 } }, false };
   ns::interval_set< int > is9 { { { 2, 21 } }, false };
   ns::interval_set< int > is10 { { { 1, 22 } }, false };

   SECTION("Intersection on one part") {
      ns::interval_set< int > true_val { {3, 4} };
      auto test_val = is1 & is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Intersection on two parts") {
      ns::interval_set< int > true_val { {3, 4}, {16, 20} };
      auto test_val = is1 & is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Intersection on one part completely") {
      ns::interval_set< int > true_val { {2, 7} };
      auto test_val = is1 & is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Intersection of two parts completely") {
      ns::interval_set< int > true_val { {2, 7}, {15, 21} };
      auto test_val = is1 & is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Partially intersection with one consecutive interval") {
      ns::interval_set< int > true_val { {5, 7}, {15, 18} };
      auto test_val = is1 & is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Intersection with two separate sets") {
      ns::interval_set< int > true_val {};
      auto test_val = is1 & is7;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Intersection with two separate sets boundaries") {
      ns::interval_set< int > true_val {};
      auto test_val = is1 & is8;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Intersection with completely surjective set boundaries") {
      ns::interval_set< int > true_val { {2, 7}, {15, 21} };
      auto test_val = is1 & is9;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Intersection with completely surjective set") {
      ns::interval_set< int > true_val { {2, 7}, {15, 21} };
      auto test_val = is1 & is10;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

}

TEST_CASE( "Difference operation" ) {

   ns::interval_set< int > is1 { { { 2, 7 }, { 15, 21 } }, false };
   ns::interval_set< int > is2 { { { 3, 4 } }, false };
   ns::interval_set< int > is3 { { { 3, 4 }, { 16, 20 } }, false };
   ns::interval_set< int > is4 { { { 2, 7 } }, false };
   ns::interval_set< int > is5 { { { 2, 7 }, { 15, 21 } }, false };
   ns::interval_set< int > is6 { { { 5, 18 } }, false };
   ns::interval_set< int > is7 { { { 8, 10 } }, false };
   ns::interval_set< int > is8 { { { 7, 15 } }, false };
   ns::interval_set< int > is9 { { { 2, 21 } }, false };
   ns::interval_set< int > is10 { { { 1, 22 } }, false };

   SECTION("Difference inside one part") {
      ns::interval_set< int > true_val { {2, 3}, {4, 7}, {15, 21} };
      auto test_val = is1 - is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Difference inside two parts") {
      ns::interval_set< int > true_val { {2, 3}, {4, 7}, {15, 16}, {20, 21} };
      auto test_val = is1 - is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Difference of one part completely") {
      ns::interval_set< int > true_val { {15, 21} };
      auto test_val = is1 - is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Difference of two parts completely") {
      ns::interval_set< int > true_val {};
      auto test_val = is1 - is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Partially difference with one consecutive interval") {
      ns::interval_set< int > true_val { {2, 5}, {18, 21} };
      auto test_val = is1 - is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Difference with two separate sets") {
      ns::interval_set< int > true_val { {2, 7}, {15, 21} };
      auto test_val = is1 - is7;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Difference with two separate sets boundaries") {
      ns::interval_set< int > true_val { {2, 7}, {15, 21} };
      auto test_val = is1 - is8;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Difference with completely surjective set boundaries") {
      ns::interval_set< int > true_val {};
      auto test_val = is1 - is9;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION("Difference with completely surjective set") {
      ns::interval_set< int > true_val {};
      auto test_val = is1 - is10;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

}

TEST_CASE("NOT operation") {
   ns::interval_set< int > test_val { {10, 20}, {35, 45},{46, 50} };
   CHECK((not(test_val).get_init()) == !(test_val.get_init()));
}







