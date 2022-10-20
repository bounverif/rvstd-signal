#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "bool_and_vec/interval_set.hpp"

#define ns rvstd::bool_and_vec
 
TEST_CASE("construction with new keyword")
{
    ns::interval_set<int>* p_is = new ns::interval_set<int>;
    CHECK(p_is != nullptr);
    CHECK(!p_is->empty());
}

TEST_CASE("construction with new keyword and initilazer list")
{
    ns::interval_set<int>* p_is = new ns::interval_set<int>({{1,8,11}, true});
    CHECK(p_is != nullptr);
    CHECK(p_is->empty());
}

TEST_CASE("construction as empty")
{
    ns::interval_set<int> is;
    CHECK(is.empty());
}

TEST_CASE("construction with initilazer list")
{
    ns::interval_set<int> is = {{1,8,11}, true};
    CHECK(!is.empty());
}
