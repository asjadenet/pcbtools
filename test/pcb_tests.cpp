#include <catch2/catch_test_macros.hpp>

#include <pcbtools/pcb_library.hpp>

TEST_CASE("PCB tests increments are computed", "[increment]")
{
  REQUIRE(increment(0) == 1);
  REQUIRE(increment(1) == 2);
  REQUIRE(increment(2) == 3);
  REQUIRE(increment(3) == 4);
  REQUIRE(increment(10) == 11);
}
