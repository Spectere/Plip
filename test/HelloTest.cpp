// hello_test.cpp
//
// Just a little test to get things rolling.

#include "catch2/catch_test_macros.hpp"

int HelloTest(int a, int b) {
    return a + b;
}

TEST_CASE("Completely Arbitrary Test Case", "HelloTest") {
    REQUIRE(HelloTest(1, 2) == 3);
    REQUIRE(HelloTest(2, 5) == 7);
    REQUIRE(HelloTest(1, -1) == 0);
}
