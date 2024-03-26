#include "pcbtools/get_data_file_path.h"
#include <cstdlib>
#include <stdexcept>
#include <filesystem>
#include <string>
#if defined(_MSC_VER)
#include <gsl/gsl-lite.hpp>
#endif

namespace pcb_tools {
std::string getEnvVar(const char *name)
{
  std::string result;
#if defined(_MSC_VER)

  // NOLINTNEXTLINE(cppcoreguidelines-no-malloc,hicpp-no-malloc)
  static const auto freeDeleter = [](gsl::owner<char *> ptr) { free(ptr); };

  // NOLINTNEXTLINE(misc-const-correctness)
  using cstring_uptr = std::unique_ptr<char, decltype(freeDeleter)>;

  gsl::owner<char *> rawValue = nullptr;// Explicitly mark rawValue as an owner.
  size_t size = 0;
  if (_dupenv_s(&rawValue, &size, name) != 0 || rawValue == nullptr) {
    throw std::runtime_error("Failed to get environment variable.");
  }
  const cstring_uptr value(rawValue, freeDeleter);// Transfer ownership to the smart pointer.
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
