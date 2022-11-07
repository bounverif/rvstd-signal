#include <boost/icl/interval.hpp>
#include <boost/icl/interval_set.hpp>
#include <climits>
#include <iostream>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE( "boost_icl" )
{
   std::vector< std::vector< int > > A_vec = { { -99, -95 }, { -93, -89 }, { -82, -80 }, { -66, -58 }, { -55, -52 }, { -42, -34 }, { -33, -15 }, { -13, -11 }, { -5, -1 }, { 2, 5 }, { 6, 17 }, { 42, 47 }, { 59, 67 }, { 85, 89 }, { 97, 100 } };
   std::vector< std::vector< int > > B_vec = { { -94, -80 }, { -59, -48 }, { -40, -38 }, { -23, -9 }, { 6, 12 }, { 23, 38 }, { 46, 59 }, { 63, 69 }, { 72, 89 }, { 91, 92 } };

   boost::icl::interval_set< int > A{};
   boost::icl::interval_set< int > B{};

   boost::icl::interval< int >::type interval_all( INT_MIN, INT_MAX );
   boost::icl::interval_set< int > U{ interval_all };

   for( std::vector< int > interval_vec : A_vec ) {
      boost::icl::interval< int >::type interval_( interval_vec[ 0 ], interval_vec[ 1 ] );
      A.add( interval_ );
   }

   for( std::vector< int > interval_vec : B_vec ) {
      boost::icl::interval< int >::type interval_( interval_vec[ 0 ], interval_vec[ 1 ] );
      B.add( interval_ );
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

   BENCHMARK( "Not" )
   {
      U - A;
      return 0;
   };

   //is done correctly

   std::cout << "\n results from boost::icl \n";

   boost::icl::interval_set< int > C{};
   C = A + B;
   std::cout << "A + B : ";
   for( boost::icl::interval_set< int >::iterator iter = C.begin();
        iter != C.end();
        iter++ ) {
      std::cout << *iter << " ";
   }
   std::cout << "\n";

   C = A - B;
   std::cout << "A - B : ";
   for( boost::icl::interval_set< int >::iterator iter = C.begin();
        iter != C.end();
        iter++ ) {
      std::cout << *iter << " ";
   }
   std::cout << "\n";

   C = A & B;
   std::cout << "A & B : ";
   for( boost::icl::interval_set< int >::iterator iter = C.begin();
        iter != C.end();
        iter++ ) {
      std::cout << *iter << " ";
   }
   std::cout << "\n";

   C = U - A;
   std::cout << "U - A : ";
   for( boost::icl::interval_set< int >::iterator iter = C.begin();
        iter != C.end();
        iter++ ) {
      std::cout << *iter << " ";
   }
   std::cout << "\n";
}
