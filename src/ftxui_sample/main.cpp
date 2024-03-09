//#include <array>
//#include <atomic>
//#include <chrono>
//#include <cstddef>
//#include <cstdint>
#include <cstdlib>
#include <exception>
//#include <functional>
#include <optional>
//#include <random>
#include <string>
//#include <thread>
//#include <utility>
//#include <vector>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>// NOLINT
#include <CLI/Formatter.hpp>// NOLINT
#include <fmt/core.h>// for print
//#include <fmt/format.h>// for format
//#include <ftxui/component/component.hpp>// for Slider
//#include <ftxui/component/screen_interactive.hpp>// for ScreenInteractive
//#include <ftxui/dom/elements.hpp>// for text, border
//#include <ftxui/dom/requirement.hpp>// for Requirement
//#include <ftxui/screen/color.hpp>
//#include <ftxui/screen/screen.hpp>// for Screen
#include <spdlog/spdlog.h>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `pcbtools`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>


// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char **argv)
{
  try {
    CLI::App app{ fmt::format("{} version {}", pcbtools::cmake::project_name, pcbtools::cmake::project_version) };

    std::optional<std::string> dir;
    app.add_option("-d,--dir", dir, "Working directory where all KiCad and FlatCAM files are located");
    bool show_version = false;
    app.add_flag("--version", show_version, "Show version information");



    CLI11_PARSE(app, argc, argv)

    if (show_version) {
      fmt::print("{}\n", pcbtools::cmake::project_version);
      return EXIT_SUCCESS;
    }


  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
}
