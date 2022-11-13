#include "../../include/rvstd/bool_and_vec/interval_set_sweep_line.hpp"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#define ns rvstd::bool_and_vec_sweep_line

TEST_CASE( "bool_and_vec/sweep_line" )
{
   ns::interval_set< int > A = { { -99, -95 }, { -93, -89 }, { -82, -80 }, { -66, -58 }, { -55, -52 }, { -42, -34 }, { -33, -15 }, { -13, -11 }, { -5, -1 }, { 2, 5 }, { 6, 17 }, { 42, 47 }, { 59, 67 }, { 85, 89 }, { 97, 100 } };
   ns::interval_set< int > B = { { -94, -80 }, { -59, -48 }, { -40, -38 }, { -23, -9 }, { 6, 12 }, { 23, 38 }, { 46, 59 }, { 63, 69 }, { 72, 89 }, { 91, 92 } };

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

   //is done correctly

   std::cout << "\n results from bool_and_vec/interval_set_sweep_line.hpp \n";

   ns::interval_set< int > C{};
   C = A + B;
   std::vector< int > C_vec = C.get_data_vector();
   bool C_init = C.get_init();
   std::cout << "A + B :  init = " << C_init << " vec = {";
   for( int i = 0; i < C_vec.size(); i++ ) {
      std::cout << " " << C_vec[ i ] << " ";
   }
   std::cout << "}\n";

   C = A - B;
   C_vec = C.get_data_vector();
   C_init = C.get_init();
   std::cout << "A - B :  init = " << C_init << " vec = {";
   for( int i = 0; i < C_vec.size(); i++ ) {
      std::cout << " " << C_vec[ i ] << " ";
   }
   std::cout << "}\n";

   C = A & B;
   C_vec = C.get_data_vector();
   C_init = C.get_init();
   std::cout << "A & B :  init = " << C_init << " vec = {";
   for( int i = 0; i < C_vec.size(); i++ ) {
      std::cout << " " << C_vec[ i ] << " ";
   }
   std::cout << "}\n";

   C = not( A );
   C_vec = C.get_data_vector();
   C_init = C.get_init();
   std::cout << "not(A) :  init = " << C_init << " vec = {";
   for( int i = 0; i < C_vec.size(); i++ ) {
      std::cout << " " << C_vec[ i ] << " ";
   }
   std::cout << "}\n";
}
