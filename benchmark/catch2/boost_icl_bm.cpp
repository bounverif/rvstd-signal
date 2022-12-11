#include "catch_reporter_csv.hpp"
#include <boost/icl/interval.hpp>
#include <boost/icl/interval_set.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <climits>
#include <iostream>
#include <string>

template< typename T >
void print_interval_set( boost::icl::interval_set< T > C, std::string message )
{
   std::cout << message << " : ";
   for( typename boost::icl::interval_set< T >::iterator iter = C.begin();
        iter != C.end();
        iter++ ) {
      std::cout << *iter << " ";
   }
   std::cout << "\n";
};

template< typename T >
boost::icl::interval_set< T > create_is( std::vector< std::vector< T > > vec_ )
{
   boost::icl::interval_set< T > is{};
   for( std::vector< T > interval_vec : vec_ ) {
      typename boost::icl::interval< T >::type interval_( interval_vec[ 0 ], interval_vec[ 1 ] );
      is.add( interval_ );
   }

   return is;
};

TEST_CASE( "uniform dist." )
{
   std::vector< std::vector< int > > A_vec = { { 1, 5 }, { 7, 11 }, { 18, 20 }, { 34, 42 }, { 45, 48 }, { 58, 66 }, { 67, 85 }, { 87, 89 }, { 95, 99 }, { 102, 105 }, { 106, 117 }, { 142, 147 }, { 159, 167 }, { 185, 189 }, { 197, 200 } };
   std::vector< std::vector< int > > B_vec = { { 6, 20 }, { 41, 52 }, { 60, 62 }, { 77, 91 }, { 6, 12 }, { 123, 138 }, { 146, 159 }, { 163, 169 }, { 172, 189 }, { 191, 192 } };

   boost::icl::interval_set< int > A = create_is( A_vec );
   boost::icl::interval_set< int > B = create_is( B_vec );

   boost::icl::interval< int >::type interval_all( INT_MIN, INT_MAX );
   boost::icl::interval_set< int > U{ interval_all };

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
      U - A;
      return 0;
   };

   //is done correctly

   //std::cout << "\n results from boost::icl \n";

   //print_interval_set( A + B, "A + B" );
   //print_interval_set( A - B, "A - B" );
   //print_interval_set( A & B, "A & B" );
   //print_interval_set( U - A, "U - A" );
}

TEST_CASE( "different intensity 0" )
{
   std::vector< std::vector< int > > A_vec = { { 1, 2 }, { 3, 4 }, { 5, 6 }, { 7, 8 }, { 20, 100 }, { 102, 105 }, { 106, 117 }, { 142, 147 }, { 159, 167 }, { 185, 189 }, { 197, 200 } };
   std::vector< std::vector< int > > B_vec = { { 6, 20 }, { 21, 22 }, { 23, 24 }, { 25, 26 }, { 27, 28 }, { 29, 30 }, { 31, 32 }, { 33, 34 }, { 35, 36 }, { 101, 200 } };

   boost::icl::interval_set< int > A = create_is( A_vec );
   boost::icl::interval_set< int > B = create_is( B_vec );

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
   //print_interval_set( A + B, "A + B" );
   //print_interval_set( A - B, "A - B" );
   //print_interval_set( A & B, "A & B" );
}

TEST_CASE( "different intensity 1" )
{
   std::vector< std::vector< int > > A_vec = { { 95, 352 }, { 355, 390 }, { 401, 405 }, { 440, 510 }, { 530, 770 }, { 790, 805 }, { 820, 946 }, { 1086, 1132 }, { 1191, 1287 }, { 1320, 1366 }, { 1440, 1514 }, { 1563, 1622 }, { 1650, 1749 }, { 1820, 1924 }, { 1934, 1975 } };
   std::vector< std::vector< int > > B_vec = { { 950, 951 }, { 956, 959 }, { 963, 971 }, { 976, 982 }, { 984, 987 }, { 988, 990 }, { 992, 994 }, { 998, 1009 }, { 1010, 1017 }, { 1019, 1020 }, { 1021, 1023 }, { 1030, 1040 }, { 1041, 1042 }, { 1047, 1048 }, { 1049, 1050 } };

   boost::icl::interval_set< int > A = create_is( A_vec );
   boost::icl::interval_set< int > B = create_is( B_vec );

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
   //print_interval_set( A + B, "A + B" );
   //print_interval_set( A - B, "A - B" );
   //print_interval_set( A & B, "A & B" );
}

TEST_CASE( "different intensity 2" )
{
   std::vector< std::vector< int > > A_vec = { { 95, 352 }, { 355, 390 }, { 401, 405 }, { 440, 510 }, { 530, 770 }, { 790, 805 }, { 820, 946 }, { 949, 1132 }, { 1191, 1287 }, { 1320, 1366 }, { 1440, 1514 }, { 1563, 1622 }, { 1650, 1749 }, { 1820, 1924 }, { 1934, 1975 } };
   std::vector< std::vector< int > > B_vec = { { 950, 951 }, { 956, 959 }, { 963, 971 }, { 976, 982 }, { 984, 987 }, { 988, 990 }, { 992, 994 }, { 998, 1009 }, { 1010, 1017 }, { 1019, 1020 }, { 1021, 1023 }, { 1030, 1040 }, { 1041, 1042 }, { 1047, 1048 }, { 1049, 1050 } };

   boost::icl::interval_set< int > A = create_is( A_vec );
   boost::icl::interval_set< int > B = create_is( B_vec );

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
   //print_interval_set( A + B, "A + B" );
   //print_interval_set( A - B, "A - B" );
   //print_interval_set( A & B, "A & B" );
}

TEST_CASE( "homogeneous large sets" )
{
   bool A_bool = false;
   bool B_bool = false;
   std::vector< std::vector< int > > vec_A;
   std::vector< std::vector< int > > vec_B;

   for( int i = 1; i < 2001; i += 14 ) {
      std::vector< int > tmp;
      tmp.push_back( i );
      tmp.push_back( i + 7 );
      vec_A.push_back( tmp );
   }

   for( int i = 1; i < 2001; i += 22 ) {
      std::vector< int > tmp;
      tmp.push_back( i );
      tmp.push_back( i + 11 );
      vec_B.push_back( tmp );
   }

   boost::icl::interval_set< int > A = create_is( vec_A );
   boost::icl::interval_set< int > B = create_is( vec_B );

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
   //print_interval_set( A, "A" );
   //print_interval_set( B, "B" );
   //print_interval_set( A + B, "A + B" );
   //print_interval_set( A - B, "A - B" );
   //print_interval_set( A & B, "A & B" );
}

TEST_CASE( "double" )
{
   std::vector< std::vector< double > > A_vec = { { 0.2999999, 0.500000 }, { 0.6322312, 1.711235 }, { 4.22359, 4.79595 }, { 5.999999, 6.79999 }, { 8.59999, 8.999595 }, { 9.723135, 10.046646122 } };
   std::vector< std::vector< double > > B_vec = { { 0.6926, 1.2965 }, { 2.3658489, 3.889529 }, { 4.6456289, 5.98959595 }, { 6.3123595, 6.987654 }, { 7.2232826, 8.93595 }, { 9.166666, 9.2126888 } };

   boost::icl::interval_set< double > A = create_is( A_vec );
   boost::icl::interval_set< double > B = create_is( B_vec );

   boost::icl::interval< double >::type interval_all( INT_MIN, INT_MAX );
   boost::icl::interval_set< double > U{ interval_all };

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
      U - A;
      return 0;
   };

   //is done correctly

   //std::cout << "\n results from boost::icl \n";

   //print_interval_set( A + B, "A + B" );
   //print_interval_set( A - B, "A - B" );
   //print_interval_set( A & B, "A & B" );
   //print_interval_set( U - A, "U - A" );
}

//corner cases

TEST_CASE( "with complementary" )
{
   std::vector< std::vector< int > > A_vec = { { 1, 5 }, { 7, 11 }, { 18, 20 }, { 34, 42 }, { 45, 48 }, { 58, 66 }, { 67, 85 }, { 87, 89 }, { 95, 99 }, { 102, 105 }, { 106, 117 }, { 142, 147 }, { 159, 167 }, { 185, 189 }, { 197, 200 } };

   boost::icl::interval_set< int > A = create_is( A_vec );

   boost::icl::interval< int >::type interval_all( INT_MIN, INT_MAX );
   boost::icl::interval_set< int > U{ interval_all };

   boost::icl::interval_set< int > not_A = U - A;

   BENCHMARK( "Union" )
   {
      A + not_A;
      return 0;
   };
   //std::cout << "\n";
   //print_interval_set( A + not_A, "A + not_A" );
}

TEST_CASE( "with null" )
{
   std::vector< std::vector< int > > A_vec = { { 1, 5 }, { 7, 11 }, { 18, 20 }, { 34, 42 }, { 45, 48 }, { 58, 66 }, { 67, 85 }, { 87, 89 }, { 95, 99 }, { 102, 105 }, { 106, 117 }, { 142, 147 }, { 159, 167 }, { 185, 189 }, { 197, 200 } };

   boost::icl::interval_set< int > A = create_is( A_vec );
   boost::icl::interval_set< int > NULL_{};

   BENCHMARK( "Union" )
   {
      A + NULL_;
      return 0;
   };
   //std::cout << "\n";
   //print_interval_set( A + NULL_, "A + NULL_" );
}

TEST_CASE( "with itself" )
{
   std::vector< std::vector< int > > A_vec = { { 1, 5 }, { 7, 11 }, { 18, 20 }, { 34, 42 }, { 45, 48 }, { 58, 66 }, { 67, 85 }, { 87, 89 }, { 95, 99 }, { 102, 105 }, { 106, 117 }, { 142, 147 }, { 159, 167 }, { 185, 189 }, { 197, 200 } };

   boost::icl::interval_set< int > A = create_is( A_vec );

   boost::icl::interval< int >::type interval_all( INT_MIN, INT_MAX );
   boost::icl::interval_set< int > U{ interval_all };

   boost::icl::interval_set< int > not_A = U - A;

   boost::icl::interval_set< int > NULL_{};

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
   //print_interval_set( A + A, "A + A" );
   //print_interval_set( A - A, "A - A" );
   //print_interval_set( A & A, "A & A" );
}
