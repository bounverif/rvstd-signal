#define CATCH_CONFIG_MAIN
#include "vec_of_pairs/interval_map_sweep_line.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#define ns rvstd::vec_of_pairs_sweep_line

TEST_CASE( "construction with new keyword" )
{
   ns::interval_map< int, int >* p_is = new ns::interval_map<int , int>;
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
   ns::interval_map< seconds_type, int > is = { {{ time0, time1 },5}, {{ time2, time3 },10} };
   CHECK( !is.empty() );
}

TEST_CASE( "construction common initilazer list with invalid interval" )
{
   SECTION( "left is lower than right" )
   {
      ns::interval_map< int, int > is0;
      CHECK_THROWS( is0 = { {{ 1, 2 },5}, {{ 7, 5 },6} } );
   }
   SECTION( "left is lower than previous right" )
   {
      ns::interval_map< int, int > is1;
      CHECK_THROWS( is1 = { {{ 1, 5 },3}, {{ 2, 7 },2} } );
   }
}

TEST_CASE( "construction common initilazer list with equal values" )
{
   SECTION( "left and right is equal" )
   {
      ns::interval_map< int, double > is0 = { {{ 1, 2.1 },1} , {{ 7.1, 7.1 },7} };
      CHECK( is0.size() == 2 );
   }
   SECTION( "left is equal to previous right" )
   {
      ns::interval_map< int, double > is1 = { {{ 1, 2 },1} , {{ 2, 7 },7} };
      CHECK( is1.size() == 3 );
   }
   SECTION( "left is equal to previous right and values equal" )
   {
      ns::interval_map< int, double > is1 =  { {{ 1, 2 },1} , {{ 2, 7 },1} };
      CHECK( is1.size() == 2 );
   }   
}

TEST_CASE( "construction with common initilazer list" )
{
   ns::interval_map< int, int > is = { {{0,3},1 },{{3,6},2 },{{7,10},3 },{{10,12},3 } };
   std::vector<std::pair<int,int>> true_val = {{0,1},{3,2},{6,0},{7,3},{12,0}} ;
   CHECK( is.get_data_vector() == true_val);

}

TEST_CASE( "construction with common initilazer list zero data" )
{
   ns::interval_map< int, int > is = { {{0,3},0 },{{3,6},0 },{{7,10},0 },{{10,12},0 } };
   std::vector<std::pair<int,int>> true_val = {} ;
   CHECK( is.get_data_vector() == true_val);

}


TEST_CASE( "checking a time values with at() function " )
{
   ns::interval_map< int, int > is0(5);
   REQUIRE( is0.empty() );

   ns::interval_map< int, int > is3 = { {{0,3},10 },{{3,6},20 },{{6,10},30 } };
   REQUIRE( !is3.empty() );

   std::vector<std::pair<int,int>> true_val = {{0,10},{3,20},{6,30},{10,0}} ;
   REQUIRE( is3.get_data_vector() == true_val);

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
   ns::interval_map< int,int > is0( -1 );
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

TEST_CASE( "Union operation as 0 1 value" )
{
   ns::interval_map< int, int > is1{ { { { 1, 3 }, 1 }, { { 9, 11 }, 1 } }, 0 };
   ns::interval_map< int, int > is2{ { { { 5, 7 }, 1 } }, 0 };
   ns::interval_map< int, int > is3{ { { { 1, 2 }, 1 } }, 0 };
   ns::interval_map< int, int > is4{ { { { 1, 14 }, 1 } }, 0 };
   ns::interval_map< int, int > is5{ { { { 2, 4 }, 1 } }, 0 };
   ns::interval_map< int, int > is6{ { { { 2, 10 }, 1 } }, 0 };

   SECTION( "Union with non-intersecting sets" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 3 }, 1 }, { { 5, 7 }, 1 }, { { 9, 11 }, 1 } };
      auto test_val = is1 + is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Union with two overlapping sets" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 3 }, 1 }, { { 9, 11 }, 1 } };
      auto test_val = is1 + is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Union with completely encapsulating set" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 14 }, 1 } };
      auto test_val = is1 + is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Union with partially overlapped set" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 4 }, 1 }, { { 9, 11 }, 1 } };
      auto test_val = is1 + is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Union with partially encapsulating set" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 11 }, 1 } };
      auto test_val = is1 + is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }
}

TEST_CASE( "Union operation different values" )
{
   ns::interval_map< int, int > is1{ { { { 1, 3 }, 2 }, { { 9, 11 }, 2 } }, 0 };
   ns::interval_map< int, int > is2{ { { { 5, 7 }, 5 } }, 0 };

   SECTION( "Union with non-intersecting sets" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 3 }, 2 }, { { 5, 7 }, 5 }, { { 9, 11 }, 2 } };
      auto test_val = is1 + is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }


   ns::interval_map< int, int > is3{ { { { 1, 2 }, 5 } }, 0 };

   SECTION( "Union with two overlapping sets" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 2 }, 5 }, { { 2, 3 }, 2 }, { { 9, 11 }, 2 } };
      auto test_val = is1 + is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }
   

   ns::interval_map< int, int > is3_2{ { { { 1, 2 }, 1 } }, 0 };

   SECTION( "Union with two overlapping sets 2" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 3 }, 2 }, { { 9, 11 }, 2 } };
      auto test_val = is1 + is3_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }


   ns::interval_map< int, int > is4{ { { { 1, 14 }, 5 } }, 0 };

   SECTION( "Union with completely encapsulating set" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 14 }, 5 } };
      auto test_val = is1 + is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }
   

   ns::interval_map< int, int > is4_2{ { { { 1, 14 }, 1 } }, 0 };
   SECTION( "Union with completely encapsulating set 2" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 3 }, 2 }, { { 3, 9 }, 1 }, { { 9, 11 }, 2 }, { { 11, 14 }, 1 } };
      auto test_val = is1 + is4_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }


   ns::interval_map< int, int > is5{ { { { 2, 4 }, 5 } }, 0 };

   SECTION( "Union with partially overlapped set" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 2 }, 2 }, { { 2, 4 }, 5 }, { { 9, 11 }, 2 } };
      auto test_val = is1 + is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }


   ns::interval_map< int, int > is5_2{ { { { 2, 4 }, 1 } }, 0 };

   SECTION( "Union with partially overlapped set 2" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 3 }, 2 }, { { 3, 4 }, 1 }, { { 9, 11 }, 2 } };
      auto test_val = is1 + is5_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );

      /*for(std::pair<int,int> x : test_val.get_data_vector()){
         std::cout << "time: " << x.first << " value: "<< x.second << "\n";
      }*/
   }


   ns::interval_map< int, int > is6{ { { { 2, 10 }, 5} }, 0 };

   SECTION( "Union with partially encapsulating set" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 2 }, 2 }, { { 2, 10 }, 5 }, { { 10, 11 }, 2 } };
      auto test_val = is1 + is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is6_2{ { { { 2, 10 }, 1} }, 0 };

   SECTION( "Union with partially encapsulating set 2" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 3 }, 2 }, { { 3, 9 }, 1 }, { { 9, 11 }, 2 } };
      auto test_val = is1 + is6_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

}

TEST_CASE( "Intersection operation as 0 1 value" )
{
  ns::interval_map< int, int > is1{ { { { 2, 7 }, 1 }, { { 15, 21 }, 1 } }, 0 };
  ns::interval_map< int, int > is2{ { { { 3, 4 }, 1 } }, 0 };
  ns::interval_map< int, int > is3{ { { { 3, 4 }, 1 }, { { 16, 20 }, 1} }, 0 };
  ns::interval_map< int, int > is4{ { { { 2, 7 }, 1 } }, 0 };
  ns::interval_map< int, int > is5{ { { { 2, 7 }, 1 }, { { 15, 21 }, 1 } }, 0 };
  ns::interval_map< int, int > is6{ { { { 5, 18 }, 1} }, 0 };
  ns::interval_map< int, int > is7{ { { { 8, 10 }, 1} }, 0 };
  ns::interval_map< int, int > is8{ { { { 7, 15 }, 1} }, 0 };
  ns::interval_map< int, int > is9{ { { { 2, 21 }, 1} }, 0 };
  ns::interval_map< int, int > is10{ { { { 1, 22 }, 1} }, 0 };

   SECTION( "Intersection on one part" )
   {
     ns::interval_map< int, int > true_val{ { { 3, 4 }, 1 } };
      auto test_val = is1 & is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection on two parts" )
   {
     ns::interval_map< int, int > true_val{ { { 3, 4 }, 1 }, { { 16, 20 }, 1} };
      auto test_val = is1 & is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection on one part completely" )
   {
     ns::interval_map< int, int > true_val{ { { 2, 7 }, 1 } };
      auto test_val = is1 & is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection of two parts completely" )
   {
     ns::interval_map< int, int > true_val{ { { 2, 7 }, 1 }, { { 15, 21 }, 1 } };
      auto test_val = is1 & is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Partially intersection with one consecutive interval" )
   {
     ns::interval_map< int, int > true_val{ { { 5, 7 }, 1}, { { 15, 18 }, 1} };
      auto test_val = is1 & is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection with two separate sets" )
   {
     ns::interval_map< int, int > true_val{};
      auto test_val = is1 & is7;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection with two separate sets boundaries" )
   {
     ns::interval_map< int, int > true_val{};
      auto test_val = is1 & is8;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection with completely surjective set boundaries" )
   {
     ns::interval_map< int, int > true_val{ { { 2, 7 }, 1 }, { { 15, 21 }, 1 } };
      auto test_val = is1 & is9;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   SECTION( "Intersection with completely surjective set" )
   {
     ns::interval_map< int, int > true_val{ { { 2, 7 }, 1 }, { { 15, 21 }, 1 } };
      auto test_val = is1 & is10;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }
}

TEST_CASE( "Intersection operation different values" )
{
   ns::interval_map< int, int > is1{ { { { 1, 3 }, 2 }, { { 9, 11 }, 2 } }, 0 };


   ns::interval_map< int, int > is2{ { { { 5, 7 }, 5 } }, 0 };

   SECTION( "Intersection with non-intersecting sets" )
   {
      ns::interval_map< int, int > true_val{};
      auto test_val = is1 & is2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }


   ns::interval_map< int, int > is3{ { { { 1, 2 }, 5 } }, 0 };

   SECTION( "Intersection with two overlapping sets" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 2 }, 2 } };
      auto test_val = is1 & is3;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }
   

   ns::interval_map< int, int > is3_2{ { { { 1, 2 }, 1 } }, 0 };

   SECTION( "Intersection with two overlapping sets 2" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 2 }, 1 } };
      auto test_val = is1 & is3_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }


   ns::interval_map< int, int > is4{ { { { 1, 14 }, 5 } }, 0 };

   SECTION( "Intersection with completely encapsulating set" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 3 }, 2 }, { { 9, 11 }, 2 } };
      auto test_val = is1 & is4;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }
   

   ns::interval_map< int, int > is4_2{ { { { 1, 14 }, 1 } }, 0 };

   SECTION( "Intersection with completely encapsulating set 2" )
   {
      ns::interval_map< int, int > true_val{ { { 1, 3 }, 1 }, {{ 9, 11 }, 1 }} ;
      auto test_val = is1 & is4_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }


   ns::interval_map< int, int > is5{ { { { 2, 4 }, 5 } }, 0 };

   SECTION( "Intersection with partially overlapped set" )
   {
      ns::interval_map< int, int > true_val{ { { 2, 3 }, 2 } };
      auto test_val = is1 & is5;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }


   ns::interval_map< int, int > is5_2{ { { { 2, 4 }, 1 } }, 0 };

   SECTION( "Intersection with partially overlapped set 2" )
   {
      ns::interval_map< int, int > true_val{  { { 2, 3 }, 1 } };
      auto test_val = is1 & is5_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );

      /*for(std::pair<int,int> x : test_val.get_data_vector()){
         std::cout << "time: " << x.first << " value: "<< x.second << "\n";
      }*/
   }


   ns::interval_map< int, int > is6{ { { { 2, 10 }, 5} }, 0 };

   SECTION( "Intersection with partially encapsulating set" )
   {
      ns::interval_map< int, int > true_val{ { { 2, 3 }, 2 }, { { 9, 10 }, 2 } };
      auto test_val = is1 & is6;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

   ns::interval_map< int, int > is6_2{ { { { 2, 10 }, 1} }, 0 };

   SECTION( "Intersection with partially encapsulating set 2" )
   {
      ns::interval_map< int, int > true_val{ { { 2, 3 }, 1 }, { { 9, 10 }, 1 } };
      auto test_val = is1 & is6_2;
      CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
      CHECK( test_val.get_init() == true_val.get_init() );
   }

}



TEST_CASE( "NOT operation" )
{
   ns::interval_map< int, int > set_{ { { 10, 20 }, 1} , { { 35, 45 }, 2 }, { { 46, 50 }, 3} };
   auto test_val = not(set_);

   ns::interval_map< int, int > true_val{ { { 10, 20 }, -1} , { { 35, 45 }, -2 }, { { 46, 50 }, -3} };

   CHECK( test_val.get_data_vector() == true_val.get_data_vector() );
   CHECK( test_val.get_init() == true_val.get_init() );
}



TEST_CASE( "set rules" )
{
   ns::interval_map< int, int > A = { { { -99, -95 }, 6 }, { { -93, -89 },6 }, {{ -82, -80 },7}, {{ -66, -58 },3}, {{ -55, -52 },2}, {{ -42, -34 },4}, {{ -33, -15 },9}, {{ -13, -11 },1}, {{ -5, -1 },4}, {{ 2, 5 },4}, {{ 6, 17 },1}, {{ 42, 47 },6}, {{ 59, 67 },4}, {{ 85, 89 },2}, {{ 97, 100 },1} };
   ns::interval_map< int, int > B = { { { -94, -80 },4}, {{ -59, -48 },3}, {{ -40, -38 },1}, {{ -23, -9 },1}, {{ 6, 12 },2}, {{ 23, 38 },7}, {{ 46, 59 },6}, {{ 63, 69 },8}, {{ 72, 89 },9}, {{ 91, 92 },1 }};
   ns::interval_map< int, int > C{ {{ -84, -42 },7}, {{ -20, 17 },1}, {{ 22, 27 },9}, {{ 36, 38 },3}, {{ 52, 59 },5} };
   ns::interval_map< int, int > U( 10 );
   ns::interval_map< int, int > NULL_( 0 );

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