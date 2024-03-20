#include "pcbtools/board.h"
#include "pcbtools/get_board_info.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <sstream>

namespace pcb_tools {
double parseCoordinate(const std::string &coordinate)
{
  constexpr auto scale = 1000000.0;
  return std::stod(coordinate) / scale;// Convert to double and adjust scale
}

board get_board_info(const std::filesystem::path &gerber_file)
{
  board brd{};
  std::ifstream file(gerber_file);

  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << gerber_file << '\n';
    return brd;
  }

  double min_x = std::numeric_limits<double>::max();
  double max_x = std::numeric_limits<double>::lowest();
  double min_y = std::numeric_limits<double>::max();
  double max_y = std::numeric_limits<double>::lowest();

  std::string line;
  while (std::getline(file, line)) {
    if (line[0] == 'X') {
      std::istringstream iss(line.substr(1));// Create a stream skipping 'X'
      std::string x_str;
      std::string y_str;
      std::getline(iss, x_str, 'Y');// Read up to 'Y'
      std::getline(iss, y_str);// Read the rest

      const double x = parseCoordinate(x_str);
      const double y = parseCoordinate(y_str);

      min_x = std::min(min_x, x);
      max_x = std::max(max_x, x);
      min_y = std::min(min_y, y);
      max_y = std::max(max_y, y);
    }
  }

  const double length = max_x - min_x;
  const double width = max_y - min_y;

  brd.start.x = min_x;
  brd.start.y = min_y;

  brd.size.x = width;
  brd.size.y = length;
  return brd;
}
}// namespace pcb_tools
