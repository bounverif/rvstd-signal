#include <boost/icl/interval.hpp>
#include <boost/icl/interval_set.hpp>
#include <climits>
#include <iostream>
#include <string>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

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

TEST_CASE( "boost::icl  different intensity 0" )
{
   std::vector< std::vector< int > > A_vec = { { -99, -98 }, { -97, -96 }, { -95, -94 }, { -93, -92 }, { -80, 0 }, { 2, 5 }, { 6, 17 }, { 42, 47 }, { 59, 67 }, { 85, 89 }, { 97, 100 } };
   std::vector< std::vector< int > > B_vec = { { -94, -80 }, { -79, -78 }, { -77, -76 }, { -75, -74 }, { -73, -72 }, { -71, -70 }, { -69, -68 }, { -67, -66 }, { -65, -64 }, { 1, 100 } };

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
   std::cout << "\n";
   print_interval_set( A + B, "A + B" );
   print_interval_set( A - B, "A - B" );
   print_interval_set( A & B, "A & B" );
}

TEST_CASE( "boost::icl  different intensity 1" )
{
   std::vector< std::vector< int > > A_vec = { { -905, -667 }, { -622, -614 }, { -592, -585 }, { -583, -493 }, { -476, -299 }, { -295, -281 }, { -174, -56 }, { 86, 132 }, { 191, 287 }, { 320, 366 }, { 440, 514 }, { 563, 622 }, { 650, 749 }, { 820, 924 }, { 934, 975 } };
   std::vector< std::vector< int > > B_vec = { { -50, -49 }, { -44, -41 }, { -37, -29 }, { -24, -18 }, { -16, -13 }, { -12, -10 }, { -8, -6 }, { -2, 9 }, { 10, 17 }, { 19, 20 }, { 21, 23 }, { 30, 40 }, { 41, 42 }, { 47, 48 }, { 49, 50 } };

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
   std::cout << "\n";
   print_interval_set( A + B, "A + B" );
   print_interval_set( A - B, "A - B" );
   print_interval_set( A & B, "A & B" );
}

TEST_CASE( "boost::icl  different intensity 2" )
{
   std::vector< std::vector< int > > A_vec = { { -905, -667 }, { -622, -614 }, { -592, -585 }, { -583, -493 }, { -476, -299 }, { -295, -281 }, { -174, -156 }, { -86, 132 }, { 191, 287 }, { 320, 366 }, { 440, 514 }, { 563, 622 }, { 650, 749 }, { 820, 924 }, { 934, 975 } };
   std::vector< std::vector< int > > B_vec = { { -50, -49 }, { -44, -41 }, { -37, -29 }, { -24, -18 }, { -16, -13 }, { -12, -10 }, { -8, -6 }, { -2, 9 }, { 10, 17 }, { 19, 20 }, { 21, 23 }, { 30, 40 }, { 41, 42 }, { 47, 48 }, { 49, 50 } };

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
   std::cout << "\n";
   print_interval_set( A + B, "A + B" );
   print_interval_set( A - B, "A - B" );
   print_interval_set( A & B, "A & B" );
}

TEST_CASE( "boost::icl  large sets" )
{
   bool A_bool = false;
   bool B_bool = false;
   std::vector< std::vector< int > > vec_A;
   std::vector< std::vector< int > > vec_B;

   for( int i = -1000; i < 1000; i += 14 ) {
      std::vector< int > tmp;
      tmp.push_back( i );
      tmp.push_back( i + 7 );
      vec_A.push_back( tmp );
   }

   for( int i = -1000; i < 1000; i += 22 ) {
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
   std::cout << "\n";
   print_interval_set( A, "A" );
   print_interval_set( B, "B" );
   print_interval_set( A + B, "A + B" );
   print_interval_set( A - B, "A - B" );
   print_interval_set( A & B, "A & B" );
}

TEST_CASE( "boost::icl rare cases" )
{
   std::vector< std::vector< int > > A_vec = { { -99, -95 }, { -93, -89 }, { -82, -80 }, { -66, -58 }, { -55, -52 }, { -42, -34 }, { -33, -15 }, { -13, -11 }, { -5, -1 }, { 2, 5 }, { 6, 17 }, { 42, 47 }, { 59, 67 }, { 85, 89 }, { 97, 100 } };

   boost::icl::interval_set< int > A = create_is( A_vec );

   boost::icl::interval< int >::type interval_all( INT_MIN, INT_MAX );
   boost::icl::interval_set< int > U{ interval_all };

   boost::icl::interval_set< int > not_A = U - A;

   boost::icl::interval_set< int > NULL_{};

   BENCHMARK( "Union with itself" )
   {
      A + A;
      return 0;
   };

   BENCHMARK( "Union with NULL" )
   {
      A + NULL_;
      return 0;
   };

   BENCHMARK( "Union with complementary" )
   {
      A + not_A;
      return 0;
   };

   BENCHMARK( "Difference by itself" )
   {
      A - A;
      return 0;
   };

   BENCHMARK( "Intersection by itself" )
   {
      A& A;
      return 0;
   };
   std::cout << "\n";
   print_interval_set( A, "A" );
   print_interval_set( not_A, "not_A" );
   print_interval_set( A + A, "A + A" );
   print_interval_set( A + NULL_, "A + NULL_" );
   print_interval_set( A + not_A, "A + not_A" );
   print_interval_set( A - A, "A - A" );
   print_interval_set( A & A, "A& A" );
}

TEST_CASE( "boost::icl  basic" )
{
   std::vector< std::vector< int > > A_vec = { { -99, -95 }, { -93, -89 }, { -82, -80 }, { -66, -58 }, { -55, -52 }, { -42, -34 }, { -33, -15 }, { -13, -11 }, { -5, -1 }, { 2, 5 }, { 6, 17 }, { 42, 47 }, { 59, 67 }, { 85, 89 }, { 97, 100 } };
   std::vector< std::vector< int > > B_vec = { { -94, -80 }, { -59, -48 }, { -40, -38 }, { -23, -9 }, { 6, 12 }, { 23, 38 }, { 46, 59 }, { 63, 69 }, { 72, 89 }, { 91, 92 } };

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

   std::cout << "\n results from boost::icl \n";

   print_interval_set( A + B, "A + B" );
   print_interval_set( A - B, "A - B" );
   print_interval_set( A & B, "A & B" );
   print_interval_set( U - A, "U - A" );
}

TEST_CASE( "boost_icl basic double" )
{
   std::vector< std::vector< double > > A_vec = { { -9.99898498, -9.554985 }, { -9.36546, -8.987 }, { -8.2, -8.05688 }, { -6.66868, -5.84 }, { -5.5999999, -5.2546 }, { -4.22232, -3.4111111 }, { -3.33, -1.52 }, { -1.364895, -1.1389595 }, { -0.525959, -0.16 }, { 0.2999999, 0.500000 }, { 0.6322312, 1.711235 }, { 4.22359, 4.79595 }, { 5.999999, 6.79999 }, { 8.59999, 8.999595 }, { 9.723135, 10.046646122 } };
   std::vector< std::vector< double > > B_vec = { { -9.40404040, -8.012151 }, { -5.9526252, -4.86544848 }, { -4.065888, -3.835595952 }, { -2.3122, -0.9956 }, { 0.6926, 1.2965 }, { 2.3658489, 3.889529 }, { 4.6456289, 5.98959595 }, { 6.3123595, 6.987654 }, { 7.2232826, 8.93595 }, { 9.166666, 9.2126888 } };

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

   std::cout << "\n results from boost::icl \n";

   print_interval_set( A + B, "A + B" );
   print_interval_set( A - B, "A - B" );
   print_interval_set( A & B, "A & B" );
   print_interval_set( U - A, "U - A" );
}
