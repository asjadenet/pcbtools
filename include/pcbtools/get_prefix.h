#pragma once

#include <filesystem>
#include <string>

namespace pcb_tools {
std::string get_prefix();
std::string get_prefix(const std::filesystem::path &current_path);
}// namespace pcb_tools
