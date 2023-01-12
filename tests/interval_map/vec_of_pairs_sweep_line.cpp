#define CATCH_CONFIG_MAIN
#include "vec_of_pairs/interval_map.hpp"
#include <catch2/catch_test_macros.hpp>

#define ns rvstd::vec_of_pairs

TEST_CASE( "construction with new keyword" )
{
   ns::interval_map< int, int >* p_is = new ns::interval_map< int, int >;
   CHECK( p_is != nullptr );
   CHECK( p_is->empty() );
}

TEST_CASE( "construction as empty" )
{
   ns::interval_map< int, int > is;
   CHECK( is.empty() );
}

TEST_CASE( "construction with initilazer list for type::chrono" )
{
   typedef std::chrono::duration< int > seconds_type;
   seconds_type time0( 1 );
   seconds_type time1( 8 );
   seconds_type time2( 11 );
   seconds_type time3( 17 );
   ns::interval_map< seconds_type, int > is = { { time0, 5 }, { time2, 10 } };
   CHECK( !is.empty() );
}

TEST_CASE( "construction common initilazer list with invalid interval" )
{
   SECTION( "lower than next" )
   {
      ns::interval_map< int, int > is0;
      CHECK_THROWS( is0 = { { 10, 5 }, { 7, 5 } } );
   }
}

TEST_CASE( "construction common initilazer list with equal values" )
{
   SECTION( "left and right is equal" )
   {
      ns::interval_map< int, double > is0 = { { 1, 2.1 }, { 1, 7.1 } };
      CHECK( is0.size() == 1 );
   }
}

TEST_CASE( "construction with common initilazer list" )
{
   ns::interval_map< int, int > is = { { 0, 1 }, { 3, 0 }, { 3, 2 }, { 6, 0 }, { 7, 3 }, { 10, 0 }, { 10, 3 }, { 12, 0 } };
   std::vector< std::pair< int, int > > true_val = { { 0, 1 }, { 3, 2 }, { 6, 0 }, { 7, 3 }, { 12, 0 } };
   CHECK( is.get_data_vector() == true_val );
}

TEST_CASE( "construction with common initilazer list zero data" )
{
   ns::interval_map< int, int > is = { { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 } };
   std::vector< std::pair< int, int > > true_val = {};
   CHECK( is.get_data_vector() == true_val );
}

TEST_CASE( "checking a time values with at() function " )
{
   ns::interval_map< int, int > is0( 5 );
   REQUIRE( is0.empty() );

   ns::interval_map< int, int > is3 = { { 0, 10 }, { 3, 0 }, { 3, 20 }, { 6, 0 }, { 6, 30 }, { 10, 0 } };
   REQUIRE( !is3.empty() );

   std::vector< std::pair< int, int > > true_val = { { 0, 10 }, { 3, 20 }, { 6, 30 }, { 10, 0 } };
   REQUIRE( is3.get_data_vector() == true_val );

   SECTION( "checking empty interval set" )
   {
      CHECK( is0.at( 15 ) == 5 );
   }
   SECTION( " checking mid values " )
   {
      CHECK( is3.at( 2 ) == 10 );
      CHECK( is3.at( 5 ) == 20 );
      CHECK( is3.at( 8 ) == 30 );
   }
   SECTION( " checking value when signal changed" )
   {
      CHECK( is3.at( 0 ) == 10 );
      CHECK( is3.at( 3 ) == 20 );
      CHECK( is3.at( 6 ) == 30 );
      CHECK( is3.at( 10 ) == 0 );
   }
   SECTION( " cheking value at start and infinity" )
   {
      CHECK( is3.at( -1 ) == 0 );
      CHECK( is3.at( 42 ) == 0 );
   }
}

TEST_CASE( "appending to the set " )
{
   ns::interval_map< int, int > is0( -1 );
   REQUIRE( is0.empty() );
   REQUIRE( is0.get_init() == -1 );

   SECTION( "adding to set init = -1" )
   {
      //empty
      is0.append( 2, -1 );
      CHECK( is0.at( 0 ) == -1 );
      CHECK( is0.at( 3 ) == -1 );
      CHECK( is0.size() == 0 );
      //empty

      is0.append( 2, 1 );
      CHECK( is0.at( 0 ) == -1 );
      CHECK( is0.at( 3 ) == 1 );
      CHECK( is0.size() == 1 );
      //is0 -> { 3-1 }

      //non-empty
      is0.append( 15, 10 );
      CHECK( is0.at( 13 ) == 1 );
      CHECK( is0.at( 17 ) == 10 );
      CHECK( is0.size() == 2 );
      // is0 -> { 3-1 15-10 }

      //adding to mid of set
      CHECK_THROWS( is0.append( 13, 5 ) );
      // is0 -> { 3-1 15-10 }

      //the same value with end
      is0.append( 19, 10 );
      CHECK( is0.at( 16 ) == 10 );
      CHECK( is0.size() == 2 );
      // is0 -> { 3-1 15-10 }

      //the same time with end
      is0.append( 15, 20 );
      CHECK( is0.at( 16 ) == 20 );
      CHECK( is0.size() == 2 );
      // is0 -> { 3-1 15-20 }

      //the same time with end and same value with previous
      is0.append( 15, 1 );
      CHECK( is0.at( 16 ) == 1 );
      CHECK( is0.size() == 1 );
      // is0 -> { 3-1 }
   }
}

TEST_CASE( "append corner cases" )
{
   ns::interval_map< int, int > im{};

   im.append( 1, 10 );
   std::vector< std::pair< int, int > > true_val0 = { { 1, 10 } };
   CHECK( im.get_data_vector() == true_val0 );

   im.append( 1, 10 );
   std::vector< std::pair< int, int > > true_val1 = { { 1, 10 } };
   CHECK( im.get_data_vector() == true_val1 );

   im.append( 1, 6 );
   std::vector< std::pair< int, int > > true_val2 = { { 1, 6 } };
   CHECK( im.get_data_vector() == true_val2 );

   im.append( 3, 3 );
   im.append( 3, 6 );
   std::vector< std::pair< int, int > > true_val3 = { { 1, 6 } };
   CHECK( im.get_data_vector() == true_val3 );

   im.append( 2, 3 );
   std::vector< std::pair< int, int > > true_val4 = { { 1, 6 }, { 2, 3 } };
   CHECK( im.get_data_vector() == true_val4 );
}

TEST_CASE( "Union operation different values" )
{
   ns::interval_map< int, int > is1{ { { 1, 2 }, { 3, 0 }, { 9, 2 }, { 11, 0 } } };
   ns::interval_map< int, int > is2{ { { 5, 5 }, { 7, 0 } } };

   SECTION( "Union with non-intersecting sets" )
   {
      ns::interval_map< int, int > true_val = { { 1, 2 }, { 3, 0 }, { 5, 5 }, { 7, 0 }, { 9, 2 }, { 11, 0 } };
      auto test_val = is1 + is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is3{ { { 1, 5 }, { 2, 0 } } };

   SECTION( "Union with two overlapping sets" )
   {
      ns::interval_map< int, int > true_val = { { 1, 5 }, { 2, 2 }, { 3, 0 }, { 9, 2 }, { 11, 0 } };
      auto test_val = is1 + is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is3_2 = { { 1, 1 }, { 2, 0 } };

   SECTION( "Union with two overlapping sets 2" )
   {
      ns::interval_map< int, int > true_val = { { 1, 2 }, { 3, 0 }, { 9, 2 }, { 11, 0 } };
      auto test_val = is1 + is3_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is4 = { { 1, 5 }, { 14, 0 } };

   SECTION( "Union with completely encapsulating set" )
   {
      ns::interval_map< int, int > true_val = { { 1, 5 }, { 14, 0 } };
      auto test_val = is1 + is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is4_2 = { { 1, 1 }, { 14, 0 } };
   SECTION( "Union with completely encapsulating set 2" )
   {
      ns::interval_map< int, int > true_val = { { 1, 2 }, { 3, 1 }, { 9, 2 }, { 11, 1 }, { 14, 0 } };
      auto test_val = is1 + is4_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is5 = { { 2, 5 }, { 4, 0 } };

   SECTION( "Union with partially overlapped set" )
   {
      ns::interval_map< int, int > true_val = { { 1, 2 }, { 2, 5 }, { 4, 0 }, { 9, 2 }, { 11, 0 } };
      auto test_val = is1 + is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is5_2 = { { 2, 1 }, { 4, 0 } };

   SECTION( "Union with partially overlapped set 2" )
   {
      ns::interval_map< int, int > true_val{ { 1, 2 }, { 3, 1 }, { 4, 0 }, { 9, 2 }, { 11, 0 } };
      auto test_val = is1 + is5_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is6 = { { 2, 5 }, { 10, 0 } };

   SECTION( "Union with partially encapsulating set" )
   {
      ns::interval_map< int, int > true_val{ { 1, 2 }, { 2, 5 }, { 10, 2 }, { 11, 0 } };
      auto test_val = is1 + is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is6_2 = { { 2, 1 }, { 10, 0 } };

   SECTION( "Union with partially encapsulating set 2" )
   {
      ns::interval_map< int, int > true_val{ { 1, 2 }, { 3, 1 }, { 9, 2 }, { 11, 0 } };
      ;
      auto test_val = is1 + is6_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }
}

TEST_CASE( "Intersection operation different values" )
{
   ns::interval_map< int, int > is1 = { { 1, 2 }, { 3, 0 }, { 9, 2 }, { 11, 0 } };

   ns::interval_map< int, int > is2 = { { 5, 5 }, { 7, 0 } };

   SECTION( "Intersection with non-intersecting sets" )
   {
      ns::interval_map< int, int > true_val{};
      auto test_val = is1 & is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is3 = { { 1, 1 }, { 2, 0 } };

   SECTION( "Intersection with two overlapping sets" )
   {
      ns::interval_map< int, int > true_val = { { 1, 1 }, { 2, 0 } };
      auto test_val = is1 & is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is3_2 = { { 1, 1 }, { 2, 0 } };

   SECTION( "Intersection with two overlapping sets 2" )
   {
      ns::interval_map< int, int > true_val = { { 1, 1 }, { 2, 0 } };
      auto test_val = is1 & is3_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is4 = { { 1, 5 }, { 14, 0 } };

   SECTION( "Intersection with completely encapsulating set" )
   {
      ns::interval_map< int, int > true_val = { { 1, 2 }, { 3, 0 }, { 9, 2 }, { 11, 0 } };
      auto test_val = is1 & is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is4_2 = { { 1, 1 }, { 14, 0 } };

   SECTION( "Intersection with completely encapsulating set 2" )
   {
      ns::interval_map< int, int > true_val = { { 1, 1 }, { 3, 0 }, { 9, 1 }, { 11, 0 } };
      auto test_val = is1 & is4_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is5 = { { 2, 5 }, { 4, 0 } };

   SECTION( "Intersection with partially overlapped set" )
   {
      ns::interval_map< int, int > true_val = { { 2, 2 }, { 3, 0 } };
      auto test_val = is1 & is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is5_2 = { { 2, 1 }, { 4, 0 } };

   SECTION( "Intersection with partially overlapped set 2" )
   {
      ns::interval_map< int, int > true_val = { { 2, 1 }, { 3, 0 } };
      auto test_val = is1 & is5_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is6 = { { 2, 5 }, { 10, 0 } };

   SECTION( "Intersection with partially encapsulating set" )
   {
      ns::interval_map< int, int > true_val = { { 2, 2 }, { 3, 0 }, { 9, 2 }, { 10, 0 } };
      auto test_val = is1 & is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is6_2 = { { 2, 1 }, { 10, 0 } };

   SECTION( "Intersection with partially encapsulating set 2" )
   {
      ns::interval_map< int, int > true_val = { { 2, 1 }, { 3, 0 }, { 9, 1 }, { 10, 0 } };
      auto test_val = is1 & is6_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }
}

TEST_CASE( "NOT operation" )
{
   ns::interval_map< int, double > set_ = { { 10, 0.1 }, { 20, 0 }, { 20, 0.2 }, { 45, 0 }, { 46, 0.3 }, { 50, 0 } };
   auto test_val = not( set_ );

   ns::interval_map< int, double > true_val = { { 10, -0.1 }, { 20, 0 }, { 20, -0.2 }, { 45, 0 }, { 46, -0.3 }, { 50, 0 } };

   CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
   CHECK( test_val.get_init() == true_val.get_init() );
}

TEST_CASE( "set rules" )
{
   ns::interval_map< int, int > A = { { -99, 6 }, { -95, 0 }, { -93, -6 }, { -89, 0 }, { -82, 7 }, { -80, 0 }, { -66, -3 }, { -58, 0 }, { -55, 2 }, { -52, 0 }, { -42, 4 }, { -34, 0 }, { -33, -9 }, { -15, 0 }, { -13, 1 }, { -11, 0 }, { -5, -4 }, { -1, 0 }, { 2, 4 }, { 5, 0 }, { 6, 1 }, { 17, 0 }, { 42, -6 }, { 47, 0 }, { 59, 4 }, { 67, 0 }, { 85, -2 }, { 89, 0 }, { 97, 1 }, { 100, 0 } };
   ns::interval_map< int, int > B = { { -94, 4 }, { -80, 0 }, { -59, 3 }, { -48, 0 }, { -40, -1 }, { -38, 0 }, { -23, 1 }, { -9, 0 }, { 6, 2 }, { 12, 0 }, { 23, 7 }, { 38, 0 }, { 46, 6 }, { 59, 0 }, { 63, -8 }, { 69, 0 }, { 72, -9 }, { 89, 0 }, { 91, 1 }, { 92, 0 } };
   ns::interval_map< int, int > C = { { -84, 7 }, { -42, 0 }, { -20, 1 }, { 17, 0 }, { 22, 9 }, { 27, 0 }, { 36, 3 }, { 38, 0 }, { 52, 5 }, { 59, 0 } };
   ns::interval_map< int, int > U( 10 );
   ns::interval_map< int, int > NULL_( -10 );

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
   /*
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
   }*/
}
