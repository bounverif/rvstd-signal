#include "../../include/rvstd/vec_of_pairs/interval_map_sweep_line.hpp"
#include "catch_reporter_csv.hpp"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>

#define ns rvstd::vec_of_pairs_sweep_line

template< typename T, typename V >
void print_interval_set( ns::interval_map< T,V > C, std::string message )
{
   std::vector< std::pair <T,V> > C_vec = C.get_data_vector();
   V C_init = C.get_init();
   std::cout << message << " :  init = " << C_init << " vec = {";
   for( int i = 0; i < C_vec.size(); i++ ) {
      std::cout << " ( " << C_vec[ i ].first << " " << C_vec[ i ].second << " )";    
   }
   std::cout << "}\n";
}

TEST_CASE( "uniform dist." )
{
   ns::interval_map< int, int > A = { {{ 1, 5 },1}, {{ 7, 11 },1}, {{ 18, 20 },1}, {{ 34, 42 },1}, {{ 45, 48 },1}, {{ 58, 66 },1}, {{ 67, 85 },1}, {{ 87, 89 },1}, {{ 95, 99 },1}, {{ 102, 105 },1}, {{ 106, 117 },1},{ { 142, 147 },1}, {{ 159, 167 },1}, {{ 185, 189 },1}, {{ 197, 200 },1} };
   ns::interval_map< int, int > B = { {{ 6, 20 },1}, {{ 41, 52 },1}, {{ 60, 62 },1}, {{ 77, 91 },1}, {{ 106, 112 },1}, {{ 123, 138 },1}, {{ 146, 159 },1}, {{ 163, 169 },1}, {{ 172, 189 },1}, {{ 191, 192 },1} };
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

   BENCHMARK( "Not" )
   {
      not( A );
      return 0;
   };

   BENCHMARK( "at" )
   {
      A.at( 44 );
   };

   std::cout << "\n";
   print_interval_set( A + B, "A + B" );
   print_interval_set( A & B, "A & B" );
   print_interval_set( not( A ), "not(A)" );
}

TEST_CASE( "different intensity 0" )
{
   ns::interval_map< int, int > A = { {{ 1, 2 },1}, {{ 3, 4 },1}, {{ 5, 6 },1}, {{ 7, 8 },1}, {{ 20, 100 },1}, {{ 102, 105 },1}, {{ 106, 117 },1}, {{ 142, 147 },1}, {{ 159, 167 },1}, {{ 185, 189 },1}, {{ 197, 200 },1} };
   ns::interval_map< int, int > B = { {{ 6, 20 },1}, {{ 21, 22 },1}, {{ 23, 24 },1}, {{ 25, 26 },1}, {{ 27, 28 },1}, {{ 29, 30 },1}, {{ 31, 32 },1}, {{ 33, 34 },1}, {{ 35, 36 },1}, {{ 101, 200 },1} };
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

   std::cout << "\n";
   print_interval_set( A + B, "A + B" );
   print_interval_set( A & B, "A & B" );
}

TEST_CASE( "different intensity 1" )
{
   ns::interval_map< int, int > A = { {{ 95, 352 },1}, {{ 355, 390 },1}, {{ 401, 405 },1}, {{ 440, 510 },1}, {{ 530, 770 },1}, {{ 790, 805 },1}, {{ 820, 946 },1}, {{ 1086, 1132 },1}, {{ 1191, 1287 },1}, {{ 1320, 1366 },1}, {{ 1440, 1514 },1}, {{ 1563, 1622 },1}, {{ 1650, 1749 },1}, {{ 1820, 1924 },1}, {{ 1934, 1975 },1} };
   ns::interval_map< int, int > B = { {{ 950, 951 },1}, {{ 956, 959 },1}, {{ 963, 971 },1}, {{ 976, 982 },1}, {{ 984, 987 },1}, {{ 988, 990 },1}, {{ 992, 994 },1}, {{ 998, 1009 },1}, {{ 1010, 1017 },1}, {{ 1019, 1020 },1}, {{ 1021, 1023 },1}, {{ 1030, 1040 },1}, {{ 1041, 1042 },1}, {{ 1047, 1048 },1}, {{ 1049, 1050 },1} };

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

   std::cout << "\n";
   print_interval_set( A + B, "A + B" );
   //print_interval_set( A - B, "A - B" );
   print_interval_set( A & B, "A & B" );
}

TEST_CASE( "different intensity 2" )
{
   ns::interval_map< int, int > A = { {{ 95, 352 },1}, {{ 355, 390 },1}, {{ 401, 405 },1}, {{ 440, 510 },1}, {{ 530, 770 },1}, {{ 790, 805 },1}, {{ 820, 946 },1}, {{ 949, 1132 },1}, {{ 1191, 1287 },1}, {{ 1320, 1366 },1}, {{ 1440, 1514 },1}, {{ 1563, 1622 },1}, {{ 1650, 1749 },1}, {{ 1820, 1924 },1}, {{ 1934, 1975 },1} };
   ns::interval_map< int, int > B = { {{ 950, 951 },1}, {{ 956, 959 },1}, {{ 963, 971 },1}, {{ 976, 982 },1}, {{ 984, 987 },1}, {{ 988, 990 },1}, {{ 992, 994 },1}, {{ 998, 1009 },1}, {{ 1010, 1017 },1}, {{ 1019, 1020 },1}, {{ 1021, 1023 },1}, {{ 1030, 1040 },1}, {{ 1041, 1042 },1}, {{ 1047, 1048 },1}, {{ 1049, 1050 },1} };

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

   print_interval_set( A + B, "A + B" );
   //print_interval_set( A - B, "A - B" );
   print_interval_set( A & B, "A & B" );
}

TEST_CASE( "homogeneous large sets" )
{

   ns::interval_map< int, int > A( 0 );
   ns::interval_map< int, int > B( 0 );

   for( int i = 1; i < 2001; i += 7 ) {
      A.append( i, i%2 );
   }

   for( int i = 1; i < 2001; i += 11 ) {
      B.append( i, i%2 );
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


   std::cout << "\n";
   print_interval_set( A, "A" );
   print_interval_set( B, "B" );
   print_interval_set( A + B, "A + B" );
   print_interval_set( A & B, "A & B" );
}

TEST_CASE( "double" )
{
   ns::interval_map< double,int > A = { {{ 0.2999999, 0.500000 },1}, {{ 0.6322312, 1.711235 },1}, {{ 4.22359, 4.79595 },1}, {{ 5.999999, 6.79999 },1}, {{ 8.59999, 8.999595 },1}, {{ 9.723135, 10.046646122 },1} };
   ns::interval_map< double,int > B = { {{ 0.6926, 1.2965 },1}, {{ 2.3658489, 3.889529 },1}, {{ 4.6456289, 5.98959595 },1}, {{ 6.3123595, 6.987654 },1}, {{ 7.2232826, 8.93595 },1}, {{ 9.166666, 9.2126888 },1} };

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


   BENCHMARK( "Not" )
   {
      not( A );
      return 0;
   };

   BENCHMARK( "at" )
   {
      A.at( 8.262626 );
   };

   std::cout << "\n";
   print_interval_set( A + B, "A + B" );
   print_interval_set( A & B, "A & B" );
   print_interval_set( not( A ), "not(A)" );
}

//corner cases
TEST_CASE( "with complementary" )
{
   ns::interval_map< int, int > A = { {{ 1, 5 },1}, {{ 7, 11 },1}, {{ 18, 20 },1}, {{ 34, 42 },1}, {{ 45, 48 },1}, {{ 58, 66 },1}, {{ 67, 85 },1}, {{ 87, 89 },1}, {{ 95, 99 },1}, {{ 102, 105 },1}, {{ 106, 117 },1}, {{ 142, 147 },1}, {{ 159, 167 },1}, {{ 185, 189 },1}, {{ 197, 200 },1} };

   ns::interval_map< int, int > not_A = not( A );

   BENCHMARK( "Union" )
   {
      A + not_A;
      return 0;
   };
   std::cout << "\n";
   print_interval_set( A + not_A, "A + not_A" );
}

TEST_CASE( "with null" )
{
   ns::interval_map< int, int > A = { {{ 1, 5 },1}, {{ 7, 11 },1}, {{ 18, 20 },1}, {{ 34, 42 },1}, {{ 45, 48 },1}, {{ 58, 66 },1}, {{ 67, 85 },1}, {{ 87, 89 },1}, {{ 95, 99 },1}, {{ 102, 105 },1}, {{ 106, 117 },1}, {{ 142, 147 },1}, {{ 159, 167 },1}, {{ 185, 189 },1}, {{ 197, 200 },1} };

   ns::interval_map< int, int > NULL_( 0 );

   BENCHMARK( "Union" )
   {
      A + NULL_;
      return 0;
   };
   std::cout << "\n";
   print_interval_set( A + NULL_, "A + NULL_" );
}

TEST_CASE( "with itself" )
{
   ns::interval_map< int, int > A = { {{ 1, 5 },1}, {{ 7, 11 },1}, {{ 18, 20 },1}, {{ 34, 42 },1}, {{ 45, 48 },1}, {{ 58, 66 },1}, {{ 67, 85 },1}, {{ 87, 89 },1}, {{ 95, 99 },1}, {{ 102, 105 },1}, {{ 106, 117 },1}, {{ 142, 147 },1}, {{ 159, 167 },1}, {{ 185, 189 },1}, {{ 197, 200 },1} };

   BENCHMARK( "Union" )
   {
      A + A;
      return 0;
   };


   BENCHMARK( "Intersection" )
   {
      A& A;
      return 0;
   };
   std::cout << "\n";
   print_interval_set( A + A, "A + A" );
   print_interval_set( A & A, "A & A" );
}
