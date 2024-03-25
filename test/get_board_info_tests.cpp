#include "catch2/catch_approx.hpp"

#include <catch2/catch_test_macros.hpp>
#include "pcbtools/get_board_info.h"
#include "pcbtools/get_data_file_path.h"

TEST_CASE("PCB tests get_board_info empty")
{
  //REQUIRE(pcb_tools::get_prefix("") == "");
}

 TEST_CASE("PCB tests get_board_info real file")
 {
   const std::string file_path = "amp1-Edge_Cuts.gbr";
   const auto board_info = pcb_tools::get_board_info(pcb_tools::get_data_file_path(file_path));
   REQUIRE(board_info.size.x == Catch::Approx(54.864));
   REQUIRE(board_info.size.y == Catch::Approx(51.308));
 }
