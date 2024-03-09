#include "pcbtools/get_prefix.h"
#include <filesystem>
#include <string>

namespace pcb_tools
{
  std::string get_prefix()
  {
    namespace fs = std::filesystem;
    const fs::path current_path = fs::current_path();
    std::string prefix = current_path.filename().string();
    return prefix;
  }
}