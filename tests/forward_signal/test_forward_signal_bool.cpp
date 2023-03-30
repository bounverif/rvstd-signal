#define CATCH_CONFIG_MAIN
#include "forward_signal_bool/forward_signal.hpp"
#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

#define ns rvstd::forward_signal_bool
#define MILLI( x ) std::chrono::milliseconds( x )

TEST_CASE( "construction with new keyword" )
{
   ns::forward_signal* p_is = new ns::forward_signal;
   CHECK( p_is != nullptr );
   CHECK( p_is->empty() );
}

TEST_CASE( "construction as empty" )
{
   ns::forward_signal is;
   CHECK( is.empty() );
}

TEST_CASE( "construction common initilazer list with invalid interval" )
{
   SECTION( "lower than next" )
   {
      ns::forward_signal is0;
      CHECK_THROWS( is0 = { { 1, 3 }, { 7, 5 } } );
   }
   SECTION( "lower than next interval" )
   {
      ns::forward_signal is0;
      CHECK_THROWS( is0 = { { 1, 5 }, { 4, 5 } } );
   }
}

TEST_CASE( "construction common initilazer list with equal values" )
{
   SECTION( "left and right is equal" )
   {
      ns::forward_signal is0 = { { 1, 2 }, { 7, 7 } };
      CHECK( !is0.empty() );
      CHECK( is0.size() == 2 );
   }
   SECTION( "left is equal to previous right" )
   {
      ns::forward_signal is1 = { { 1, 2 }, { 2, 7 } };
      CHECK( !is1.empty() );
      CHECK( is1.size() == 2 );
   }
}

TEST_CASE( "construction with common initilazer list" )
{
   ns::forward_signal is = { { 1, 2 }, { 3, 7 }, { 9, 10 }, { 11, 22 } };
   CHECK( !is.empty() );
   CHECK( is.size() == 8 );
}

TEST_CASE( "using reserve function " )
{
   ns::forward_signal is0;
   REQUIRE( is0.empty() );

   SECTION( "calling reserve function" )
   {
      is0.reserve( 100 );
      CHECK( is0.capacity() == 100 );
   }
}

TEST_CASE( "getting size of interval_set" )
{
   ns::forward_signal is0;
   REQUIRE( is0.empty() );

   ns::forward_signal is1 = { { 1, 8 }, { 11, 18 } };
   REQUIRE( !is1.empty() );

   SECTION( "size function for empty" )
   {
      CHECK( is0.size() == 0 );
   }

   SECTION( "size function for non-empty" )
   {
      CHECK( is1.size() == 4 );
   }
}

TEST_CASE( "checking a time values with at() function " )
{
   ns::forward_signal is0;
   REQUIRE( is0.empty() );

   ns::forward_signal is1 = { { 1, 8 }, { 11, 18 } };
   REQUIRE( !is1.empty() );

   ns::forward_signal is2 = { { { 1, 8 } }, true };
   REQUIRE( !is2.empty() );

   SECTION( "checking empty interval set" )
   {
      CHECK( is0.at( MILLI( 5 ) ) == false );
   }
   SECTION( " checking mid values " )
   {
      CHECK( is1.at( MILLI( 9 ) ) == false );
      CHECK( is1.at( MILLI( 3 ) ) == true );
   }
   SECTION( " checking value when signal changed" )
   {
      CHECK( is1.at( MILLI( 1 ) ) == true );
      CHECK( is1.at( MILLI( 8 ) ) == false );
   }
   SECTION( " cheking value at start and infinity" )
   {
      CHECK( is1.at( MILLI( 0 ) ) == false );
      CHECK( is1.at( MILLI( INT_MAX ) ) == false );
   }

   SECTION( " cheking value at init = true" )
   {
      CHECK( is2.at( MILLI( 0 ) ) == true );
      CHECK( is2.at( MILLI( 1 ) ) == false );
      CHECK( is2.at( MILLI( 5 ) ) == false );
      CHECK( is2.at( MILLI( 8 ) ) == true );
      CHECK( is2.at( MILLI( INT_MAX ) ) == true );
   }
}

TEST_CASE( "appending to the set " )
{
   ns::forward_signal is0;
   ns::forward_signal is1( false );
   REQUIRE( is0.empty() );
   REQUIRE( is0.get_init() == false );

   SECTION( "adding to set init = false" )
   {
      //empty
      is0.append( MILLI( 2 ), true );
      CHECK( is0.at( MILLI( 0 ) ) == false );
      CHECK( is0.at( MILLI( 3 ) ) == true );
      CHECK( is0.at( MILLI( INT_MAX ) ) == true );
      CHECK( is0.size() == 1 );
      //is0 -> { 2 }

      //non-empty
      is0.append( MILLI( 15 ), false );
      CHECK( is0.at( MILLI( 13 ) ) == true );
      CHECK( is0.at( MILLI( 17 ) ) == false );
      CHECK( is0.size() == 2 );
      // is0 -> { 2, 15 }

      //the same
      is0.append( MILLI( 19 ), false );
      CHECK( is0.at( MILLI( 17 ) ) == false );
      CHECK( is0.size() == 2 );
      // is0 -> { 2, 15 }

      //adding to mid of set
      CHECK_THROWS( is0.append( MILLI( 13 ), false ) );
      // is0 -> { 2, 15 }

      //adding to exact end of set with opposite
      is0.append( MILLI( 15 ), true );
      CHECK( is0.at( MILLI( INT_MAX ) ) == true );
      CHECK( is0.size() == 1 );
      // is0 -> { 2 }
   }
}

TEST_CASE( "Union operation" )
{
   ns::forward_signal is1{ { { 1, 3 }, { 9, 11 } }, false };
   ns::forward_signal is2{ { { 5, 7 } }, false };
   ns::forward_signal is3{ { { 1, 2 } }, false };
   ns::forward_signal is4{ { { 1, 14 } }, false };
   ns::forward_signal is5{ { { 2, 4 } }, false };
   ns::forward_signal is6{ { { 2, 10 } }, false };

   SECTION( "Union with non-intersecting sets" )
   {
      ns::forward_signal true_val{ { 1, 3 }, { 5, 7 }, { 9, 11 } };
      auto test_val = is1 + is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Union with two overlapping sets" )
   {
      ns::forward_signal true_val{ { 1, 3 }, { 9, 11 } };
      auto test_val = is1 + is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Union with completely encapsulating set" )
   {
      ns::forward_signal true_val{ { 1, 14 } };
      auto test_val = is1 + is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Union with partially overlapped set" )
   {
      ns::forward_signal true_val{ { 1, 4 }, { 9, 11 } };
      auto test_val = is1 + is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Union with partially encapsulating set" )
   {
      ns::forward_signal true_val{ { 1, 11 } };
      auto test_val = is1 + is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }
}

TEST_CASE( "Intersection operation" )
{
   ns::forward_signal is1{ { { 2, 7 }, { 15, 21 } }, false };
   ns::forward_signal is2{ { { 3, 4 } }, false };
   ns::forward_signal is3{ { { 3, 4 }, { 16, 20 } }, false };
   ns::forward_signal is4{ { { 2, 7 } }, false };
   ns::forward_signal is5{ { { 2, 7 }, { 15, 21 } }, false };
   ns::forward_signal is6{ { { 5, 18 } }, false };
   ns::forward_signal is7{ { { 8, 10 } }, false };
   ns::forward_signal is8{ { { 7, 15 } }, false };
   ns::forward_signal is9{ { { 2, 21 } }, false };
   ns::forward_signal is10{ { { 1, 22 } }, false };

   SECTION( "Intersection on one part" )
   {
      ns::forward_signal true_val{ { 3, 4 } };
      auto test_val = is1 & is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection on two parts" )
   {
      ns::forward_signal true_val{ { 3, 4 }, { 16, 20 } };
      auto test_val = is1 & is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection on one part completely" )
   {
      ns::forward_signal true_val{ { 2, 7 } };
      auto test_val = is1 & is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection of two parts completely" )
   {
      ns::forward_signal true_val{ { 2, 7 }, { 15, 21 } };
      auto test_val = is1 & is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Partially intersection with one consecutive interval" )
   {
      ns::forward_signal true_val{ { 5, 7 }, { 15, 18 } };
      auto test_val = is1 & is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection with two separate sets" )
   {
      ns::forward_signal true_val{};
      auto test_val = is1 & is7;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection with two separate sets boundaries" )
   {
      ns::forward_signal true_val{};
      auto test_val = is1 & is8;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection with completely surjective set boundaries" )
   {
      ns::forward_signal true_val{ { 2, 7 }, { 15, 21 } };
      auto test_val = is1 & is9;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection with completely surjective set" )
   {
      ns::forward_signal true_val{ { 2, 7 }, { 15, 21 } };
      auto test_val = is1 & is10;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }
}

TEST_CASE( "Difference operation" )
{
   ns::forward_signal is1{ { { 2, 7 }, { 15, 21 } }, false };
   ns::forward_signal is2{ { { 3, 4 } }, false };
   ns::forward_signal is3{ { { 3, 4 }, { 16, 20 } }, false };
   ns::forward_signal is4{ { { 2, 7 } }, false };
   ns::forward_signal is5{ { { 2, 7 }, { 15, 21 } }, false };
   ns::forward_signal is6{ { { 5, 18 } }, false };
   ns::forward_signal is7{ { { 8, 10 } }, false };
   ns::forward_signal is8{ { { 7, 15 } }, false };
   ns::forward_signal is9{ { { 2, 21 } }, false };
   ns::forward_signal is10{ { { 1, 22 } }, false };

   SECTION( "Difference inside one part" )
   {
      ns::forward_signal true_val{ { { 2, 3 }, { 4, 7 }, { 15, 21 } } };
      auto test_val = is1 - is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Difference inside two parts" )
   {
      ns::forward_signal true_val{ { 2, 3 }, { 4, 7 }, { 15, 16 }, { 20, 21 } };
      auto test_val = is1 - is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Difference of one part completely" )
   {
      ns::forward_signal true_val{ { 15, 21 } };
      auto test_val = is1 - is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Difference of two parts completely" )
   {
      ns::forward_signal true_val{};
      auto test_val = is1 - is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Partially difference with one consecutive interval" )
   {
      ns::forward_signal true_val{ { 2, 5 }, { 18, 21 } };
      auto test_val = is1 - is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Difference with two separate sets" )
   {
      ns::forward_signal true_val{ { 2, 7 }, { 15, 21 } };
      auto test_val = is1 - is7;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Difference with two separate sets boundaries" )
   {
      ns::forward_signal true_val{ { 2, 7 }, { 15, 21 } };
      auto test_val = is1 - is8;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Difference with completely surjective set boundaries" )
   {
      ns::forward_signal true_val{};
      auto test_val = is1 - is9;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Difference with completely surjective set" )
   {
      ns::forward_signal true_val{};
      auto test_val = is1 - is10;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }
}
TEST_CASE( "NOT operation" )
{
   ns::forward_signal test_val{ { 10, 20 }, { 35, 45 }, { 46, 50 } };
   CHECK( ( not( test_val ).get_init() ) == !( test_val.get_init() ) );
}

TEST_CASE( "set rules" )
{
   ns::forward_signal A = { { -99, -95 }, { -93, -89 }, { -82, -80 }, { -66, -58 }, { -55, -52 }, { -42, -34 }, { -33, -15 }, { -13, -11 }, { -5, -1 }, { 2, 5 }, { 6, 17 }, { 42, 47 }, { 59, 67 }, { 85, 89 }, { 97, 100 } };
   ns::forward_signal B = { { -94, -80 }, { -59, -48 }, { -40, -38 }, { -23, -9 }, { 6, 12 }, { 23, 38 }, { 46, 59 }, { 63, 69 }, { 72, 89 }, { 91, 92 } };
   ns::forward_signal C{ { -84, -42 }, { -20, 17 }, { 22, 27 }, { 36, 38 }, { 52, 59 } };
   ns::forward_signal U( true );
   ns::forward_signal NULL_( false );

   SECTION( "idempotent law" )
   {
      SECTION( "union" )
      {
         auto true_val = A;
         auto test_val = A + A;
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
      SECTION( "intersect" )
      {
         auto true_val = A;
         auto test_val = A & A;
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
   }

   SECTION( "associative law" )
   {
      SECTION( "union" )
      {
         auto true_val = ( ( A + B ) + C );
         auto test_val = ( A + ( B + C ) );
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
      SECTION( "intersect" )
      {
         auto true_val = ( ( A & B ) & C );
         auto test_val = ( A & ( B & C ) );
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
   }

   SECTION( "commutative law" )
   {
      SECTION( "union" )
      {
         auto true_val = ( A + B );
         auto test_val = ( B + A );
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
      SECTION( "intersect" )
      {
         auto true_val = ( A & B );
         auto test_val = ( B & A );
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
   }

   SECTION( "distributive law" )
   {
      SECTION( "union" )
      {
         auto true_val = ( A + ( B & C ) );
         auto test_val = ( ( A + B ) & ( A + C ) );
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
      SECTION( "intersect" )
      {
         auto true_val = ( A & ( B + C ) );
         auto test_val = ( ( A & B ) + ( A & C ) );
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
   }

   SECTION( "de morgan's law" )
   {
      SECTION( "union" )
      {
         auto true_val = ( not( A + B ) );
         auto test_val = ( not( A ) & not( B ) );
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
      SECTION( "intersect" )
      {
         auto true_val = ( not( A & B ) );
         auto test_val = ( not( A ) + not( B ) );
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
   }

   SECTION( "identity law" )
   {
      SECTION( "union" )
      {
         auto true_val = ( A + U );
         auto test_val = ( U );
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }

      SECTION( "intersect" )
      {
         auto true_val = ( A & NULL_ );
         auto test_val = ( NULL_ );
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
   }

   SECTION( "complement law" )
   {
      SECTION( "union" )
      {
         auto true_val = C + not( C );
         auto test_val = U;
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }

      SECTION( "intersect" )
      {
         auto true_val = ( C & ( not( C ) ) );
         auto test_val = ( NULL_ );
         CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
         CHECK( test_val.get_init() == true_val.get_init() );
      }
   }
}
