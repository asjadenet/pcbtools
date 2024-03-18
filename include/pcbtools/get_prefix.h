#pragma once

#include <string>
#include <filesystem>

namespace pcb_tools
{
  std::string get_prefix();
  std::string get_prefix(const std::filesystem::path &current_path);
}