cmake_minimum_required(VERSION 3.17)
project(Catch2Benchmark)

enable_testing()

find_package(Catch2 3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS json)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#link_directories(${CMAKE_SOURCE_DIR}/lib)


function(add_test_ name_test)
  add_executable(test__${name_test} ${name_test}.cpp)
  target_link_libraries(test__${name_test} PRIVATE Catch2::Catch2WithMain)
  add_test(NAME  ctest_${name_test} COMMAND test__${name_test} --reporter CSV::out=- --reporter console::out=out_${name_test}.txt)
endfunction(add_test_)

function(add_test_static_ name_test folder)
  add_library( forward_signal STATIC ../../../src/rvstd/forward_signal_bool/forward_signal.cpp )
  add_executable(test__${name_test} ${folder}/${name_test}.cpp)
  target_link_libraries(test__${name_test} PRIVATE Catch2::Catch2WithMain forward_signal)
  add_test(NAME  ctest_${name_test} COMMAND test__${name_test} --reporter CSV::out=- --reporter console::out=out_${name_test}.txt)
endfunction(add_test_static_)

#function(add_test_boost_ name_test folder)
#  add_executable(test__${name_test} ${folder}/${name_test}.cpp)
#  target_link_libraries(test__${name_test} PRIVATE Catch2::Catch2WithMain PRIVATE Boost::json)
#  add_test(NAME  ctest_${name_test} COMMAND test__${name_test} --reporter CSV::out=- --reporter console::out=out_${name_test}.txt)
#endfunction(add_test_boost_)

#add_test_("boost_icl_bm")
add_test_("bool_and_vec_sweep_line_bm")
add_test_("bool_and_vec_skip_line_bm")
add_test_("vec_of_pairs_bm")

add_test_static_(forward_signal_bool_bm .)
#add_test_boost_(boost_json_bm . " ")

include_directories("../../include/rvstd")
include_directories("../../src/rvstd")
