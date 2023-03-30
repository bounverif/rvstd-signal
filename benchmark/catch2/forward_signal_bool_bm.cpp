#include "catch_reporter_csv.hpp"
#include "forward_signal_bool/forward_signal.hpp"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>

#define ns rvstd::forward_signal_bool
#define MILLI( x ) std::chrono::milliseconds( x )
#define MILLI_t std::chrono::milliseconds

void print_signal( ns::forward_signal C, std::string message )
{
   std::cout << message << " " << C.to_string() << "\n";
}

TEST_CASE( "uniform dist." )
{
   ns::forward_signal A = { { 1, 5 }, { 7, 11 }, { 18, 20 }, { 34, 42 }, { 45, 48 }, { 58, 66 }, { 67, 85 }, { 87, 89 }, { 95, 99 }, { 102, 105 }, { 106, 117 }, { 142, 147 }, { 159, 167 }, { 185, 189 }, { 197, 200 } };
   ns::forward_signal B = { { 6, 20 }, { 41, 52 }, { 60, 62 }, { 77, 91 }, { 106, 112 }, { 123, 138 }, { 146, 159 }, { 163, 169 }, { 172, 189 }, { 191, 192 } };
   BENCHMARK( "Union" )
   {
      A + B;
      return 0;
   };

   BENCHMARK( "Intersect" )
   {
      A& B;
      return 0;
   };

   BENCHMARK( "Difference" )
   {
      A - B;
      return 0;
   };

   BENCHMARK( "Not" )
   {
      not( A );
      return 0;
   };

   BENCHMARK( "at" )
   {
      A.at( MILLI( 44 ) );
   };

   //std::cout << "\n";
   //print_signal( A + B, "A + B" );
   //print_signal( A - B, "A - B" );
   //print_signal( A & B, "A & B" );
   //print_signal( not( A ), "not(A)" );
}

TEST_CASE( "different intensity 0" )
{
   ns::forward_signal A = { { 1, 2 }, { 3, 4 }, { 5, 6 }, { 7, 8 }, { 20, 100 }, { 102, 105 }, { 106, 117 }, { 142, 147 }, { 159, 167 }, { 185, 189 }, { 197, 200 } };
   ns::forward_signal B = { { 6, 20 }, { 21, 22 }, { 23, 24 }, { 25, 26 }, { 27, 28 }, { 29, 30 }, { 31, 32 }, { 33, 34 }, { 35, 36 }, { 101, 200 } };
   BENCHMARK( "Union" )
   {
      A + B;
      return 0;
   };

   BENCHMARK( "Intersect" )
   {
      A& B;
      return 0;
   };

   BENCHMARK( "Difference" )
   {
      A - B;
      return 0;
   };
   //std::cout << "\n";
   //print_signal( A + B, "A + B" );
   //print_signal( A - B, "A - B" );
   //print_signal( A & B, "A & B" );
}

TEST_CASE( "different intensity 1" )
{
   ns::forward_signal A = { { 95, 352 }, { 355, 390 }, { 401, 405 }, { 440, 510 }, { 530, 770 }, { 790, 805 }, { 820, 946 }, { 1086, 1132 }, { 1191, 1287 }, { 1320, 1366 }, { 1440, 1514 }, { 1563, 1622 }, { 1650, 1749 }, { 1820, 1924 }, { 1934, 1975 } };
   ns::forward_signal B = { { 950, 951 }, { 956, 959 }, { 963, 971 }, { 976, 982 }, { 984, 987 }, { 988, 990 }, { 992, 994 }, { 998, 1009 }, { 1010, 1017 }, { 1019, 1020 }, { 1021, 1023 }, { 1030, 1040 }, { 1041, 1042 }, { 1047, 1048 }, { 1049, 1050 } };

   BENCHMARK( "Union" )
   {
      A + B;
      return 0;
   };

   BENCHMARK( "Intersect" )
   {
      A& B;
      return 0;
   };

   BENCHMARK( "Difference" )
   {
      A - B;
      return 0;
   };
   //std::cout << "\n";
   //print_signal( A + B, "A + B" );
   //print_signal( A - B, "A - B" );
   //print_signal( A & B, "A & B" );
}

TEST_CASE( "different intensity 2" )
{
   ns::forward_signal A = { { 95, 352 }, { 355, 390 }, { 401, 405 }, { 440, 510 }, { 530, 770 }, { 790, 805 }, { 820, 946 }, { 949, 1132 }, { 1191, 1287 }, { 1320, 1366 }, { 1440, 1514 }, { 1563, 1622 }, { 1650, 1749 }, { 1820, 1924 }, { 1934, 1975 } };
   ns::forward_signal B = { { 950, 951 }, { 956, 959 }, { 963, 971 }, { 976, 982 }, { 984, 987 }, { 988, 990 }, { 992, 994 }, { 998, 1009 }, { 1010, 1017 }, { 1019, 1020 }, { 1021, 1023 }, { 1030, 1040 }, { 1041, 1042 }, { 1047, 1048 }, { 1049, 1050 } };

   BENCHMARK( "Union" )
   {
      A + B;
      return 0;
   };

   BENCHMARK( "Intersect" )
   {
      A& B;
      return 0;
   };

   BENCHMARK( "Difference" )
   {
      A - B;
      return 0;
   };
   //std::cout << "\n";
   //print_signal( A + B, "A + B" );
   //print_signal( A - B, "A - B" );
   //print_signal( A & B, "A & B" );
}

TEST_CASE( "homogeneous large sets" )
{
   bool A_bool = false;
   bool B_bool = false;
   ns::forward_signal A( A_bool );
   ns::forward_signal B( B_bool );

   for( int i = 1; i < 2001; i += 7 ) {
      A_bool = ( !A_bool );
      A.append( MILLI( i ), A_bool );
   }

   for( int i = 1; i < 2001; i += 11 ) {
      B_bool = ( !B_bool );
      B.append( MILLI( i ), B_bool );
   }
   BENCHMARK( "Union" )
   {
      A + B;
      return 0;
   };

   BENCHMARK( "Intersect" )
   {
      A& B;
      return 0;
   };

   BENCHMARK( "Difference" )
   {
      A - B;
      return 0;
   };
   //std::cout << "\n";
   //print_signal( A, "A" );
   //print_signal( B, "B" );
   //print_signal( A + B, "A + B" );
   //print_signal( A - B, "A - B" );
   //print_signal( A & B, "A & B" );
}

//corner cases
TEST_CASE( "with complementary" )
{
   ns::forward_signal A = { { 1, 5 }, { 7, 11 }, { 18, 20 }, { 34, 42 }, { 45, 48 }, { 58, 66 }, { 67, 85 }, { 87, 89 }, { 95, 99 }, { 102, 105 }, { 106, 117 }, { 142, 147 }, { 159, 167 }, { 185, 189 }, { 197, 200 } };

   ns::forward_signal not_A = not( A );

   BENCHMARK( "Union" )
   {
      A + not_A;
      return 0;
   };
   //std::cout << "\n";
   //print_signal( A + not_A, "A + not_A" );
}

TEST_CASE( "with null" )
{
   ns::forward_signal A = { { 1, 5 }, { 7, 11 }, { 18, 20 }, { 34, 42 }, { 45, 48 }, { 58, 66 }, { 67, 85 }, { 87, 89 }, { 95, 99 }, { 102, 105 }, { 106, 117 }, { 142, 147 }, { 159, 167 }, { 185, 189 }, { 197, 200 } };

   ns::forward_signal NULL_( false );

   BENCHMARK( "Union" )
   {
      A + NULL_;
      return 0;
   };
   //std::cout << "\n";
   //print_signal( A + NULL_, "A + NULL_" );
}

TEST_CASE( "with itself" )
{
   ns::forward_signal A = { { 1, 5 }, { 7, 11 }, { 18, 20 }, { 34, 42 }, { 45, 48 }, { 58, 66 }, { 67, 85 }, { 87, 89 }, { 95, 99 }, { 102, 105 }, { 106, 117 }, { 142, 147 }, { 159, 167 }, { 185, 189 }, { 197, 200 } };

   BENCHMARK( "Union" )
   {
      A + A;
      return 0;
   };

   BENCHMARK( "Difference" )
   {
      A - A;
      return 0;
   };

   BENCHMARK( "Intersection" )
   {
      A& A;
      return 0;
   };
   //std::cout << "\n";
   //print_signal( A + A, "A + A" );
   //print_signal( A - A, "A - A" );
   //print_signal( A & A, "A & A" );
}
