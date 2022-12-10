cmake_minimum_required(VERSION 3.17)
project(Catch2Benchmark)

enable_testing()

find_package(Catch2 3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE RELEASE)

function(add_test_ name_test)
  add_executable(test__${name_test} ${name_test}.cpp)
  target_link_libraries(test__${name_test} PRIVATE Catch2::Catch2WithMain)
  add_test(NAME  ctest_${name_test} COMMAND test__${name_test} --reporter CSV::out=- --reporter console::out=out_${name_test}.txt)
endfunction(add_test_)

add_test_("boost_icl_bm")
add_test_("bool_and_vec_sweep_line_bm")
add_test_("bool_and_vec_skip_line_bm")

