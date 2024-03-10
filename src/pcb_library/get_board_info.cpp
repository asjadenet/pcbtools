#include "pcbtools/get_board_info.h"

#include <fstream>
#include <iostream>
#include <string>

namespace pcb_tools {
double parseCoordinate(const std::string &coordinate)
{
  return std::stod(coordinate) / 1000000.0;// Convert to double and adjust scale
}

board get_board_info(const std::filesystem::path &gerber_file)
{
  board b{};
  std::ifstream file(gerber_file);

  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << gerber_file << '\n';
    return b;
  }

  double minX = std::numeric_limits<double>::max();
  double maxX = std::numeric_limits<double>::lowest();
  double minY = std::numeric_limits<double>::max();
  double maxY = std::numeric_limits<double>::lowest();

  std::string line;
  while (std::getline(file, line)) {
    if (line[0] == 'X') {
      std::istringstream iss(line.substr(1));// Create a stream skipping 'X'
      std::string xStr, yStr;
      std::getline(iss, xStr, 'Y');// Read up to 'Y'
      std::getline(iss, yStr);// Read the rest

      double x = parseCoordinate(xStr);
      double y = parseCoordinate(yStr);

      minX = std::min(minX, x);
      maxX = std::max(maxX, x);
      minY = std::min(minY, y);
      maxY = std::max(maxY, y);
    }
  }

  double length = maxX - minX;
  double width = maxY - minY;

  b.start.x = minX;
  b.start.y = minY;

  b.size.x = width;
  b.size.y = length;
  return b;
}
}// namespace pcb_tools
