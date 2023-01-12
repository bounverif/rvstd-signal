cmake_minimum_required(VERSION 3.17)
project(Catch2UnitTest)

enable_testing()


find_package(Catch2 3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

function(add_test_ name_test folder)
  add_executable(test__${name_test} ${folder}/${name_test}.cpp)
  target_link_libraries(test__${name_test} PRIVATE Catch2::Catch2WithMain)
  add_test(NAME  ctest_${name_test} COMMAND test__${name_test})
endfunction(add_test_)

add_test_(bool_and_vec_skip_line .)
add_test_(bool_and_vec_sweep_line .)
add_test_(vec_of_pairs_sweep_line interval_map)
add_test_(vec_of_pairs .)

#add_test_(two_vecs .)



include_directories("../include/rvstd")
