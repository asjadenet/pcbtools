#include <catch2/catch_test_macros.hpp>
#include <pcbtools/get_prefix.h>

TEST_CASE("PCB tests get_prefix empty")
{
  REQUIRE(pcb_tools::get_prefix("") == "");
}

TEST_CASE("PCB tests get_prefix real path")
{
#ifdef WIN32
  REQUIRE(pcb_tools::get_prefix(R"(C:\Users\tiit\Documents\kicad\amp1)") == "amp1");
#else
  REQUIRE(pcb_tools::get_prefix("/mnt/c/Users/tiit/Documents/kicad/amp1") == "amp1");
#endif
}
