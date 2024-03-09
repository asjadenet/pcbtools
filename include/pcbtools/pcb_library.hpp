#ifndef PCB_LIBRARY_HPP
#define PCB_LIBRARY_HPP

#include <pcbtools/pcb_library_export.hpp>

[[nodiscard]] PCB_LIBRARY_EXPORT int increment(int) noexcept;

[[nodiscard]] constexpr int factorial_constexpr(int input) noexcept
{
  if (input == 0) { return 1; }

  return input * factorial_constexpr(input - 1);
}

#endif
