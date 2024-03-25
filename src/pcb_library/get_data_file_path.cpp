#include "pcbtools/get_data_file_path.h"
#include <cstdlib>
#include <stdexcept>
#include <filesystem>
#include <string>


namespace pcb_tools {
std::string getEnvVar(const char *name)
{
#if defined(_MSC_VER)
  char *value = nullptr;
  size_t size = 0;
  if (_dupenv_s(&value, &size, name) == 0 && value != nullptr) {
    std::string result(value);
    free(value);// Still need to free memory allocated by _dupenv_s
    return result;
  }
#else
  const char *value = std::getenv(name);// NOLINT(concurrency-mt-unsafe)
  if (value != nullptr) { return { value }; }
#endif
  throw std::runtime_error("Environment variable not set or error retrieving it.");
}

std::filesystem::path get_data_file_path(const std::filesystem::path &file_name)
{
  const auto basePath = getEnvVar("PCB_TOOLS_DATA");
  if (basePath.empty()) { throw std::runtime_error("Environment variable 'MY_TEXT_FILES_PATH' not set."); }

  return std::filesystem::path(basePath) / file_name;
}
}// namespace pcb_tools