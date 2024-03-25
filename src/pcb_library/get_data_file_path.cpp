#include "pcbtools/get_data_file_path.h"
#include <cstdlib>
#include <stdexcept>
#include <filesystem>
#include <string>


namespace pcb_tools {

#if defined(_MSC_VER)
// Custom deleter that uses free. This encapsulates the call to free in a RAII manner.
auto freeDeleter = [](char* ptr) { free(ptr); };

// Wrap the C string in a unique_ptr with the custom deleter.
using cstring_uptr = std::unique_ptr<char, decltype(freeDeleter)>;
#endif

std::string getEnvVar(const char *name)
{
  std::string result;
#if defined(_MSC_VER)
  char *rawValue = nullptr;
  size_t size = 0;
  // _dupenv_s requires a call to free, so we ensure it's managed by a smart pointer.
  if (_dupenv_s(&rawValue, &size, name) != 0 || rawValue == nullptr) {
    throw std::runtime_error("Failed to get environment variable.");
  }
  cstring_uptr value(rawValue, freeDeleter);
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