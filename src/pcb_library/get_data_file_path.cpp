#include "pcbtools/get_data_file_path.h"
#include <cstdlib>
#include <stdexcept>
#include <filesystem>
#include <string>


namespace pcb_tools {


std::string getEnvVar(const char *name)
{
  std::string result;
#if defined(_MSC_VER)
  // Define a static const deleter inside the function to limit its scope and ensure it's immutable.
  static const auto freeDeleter = [](char *ptr) { free(ptr); };
  using cstring_uptr = std::unique_ptr<char, decltype(freeDeleter)>;

  char *rawValue = nullptr;
  size_t size = 0;
  if (_dupenv_s(&rawValue, &size, name) != 0 || rawValue == nullptr) {
    throw std::runtime_error("Failed to get environment variable.");
  }
  // Declare the smart pointer const to enforce const-correctness.
  const cstring_uptr value(rawValue, freeDeleter);
  result = value.get();
#else
  const char *value = std::getenv(name);// NOLINT(concurrency-mt-unsafe)
  if (value != nullptr) {   result =  value; }
#endif
  return result;
}

std::filesystem::path get_data_file_path(const std::filesystem::path &file_name)
{
  const auto basePath = getEnvVar("PCB_TOOLS_DATA");
  if (basePath.empty()) { throw std::runtime_error("Environment variable 'PCB_TOOLS_DATA' not set."); }

  return std::filesystem::path(basePath) / file_name;
}
}// namespace pcb_tools