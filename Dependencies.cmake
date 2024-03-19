include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(pcbtools_setup_dependencies)
  # For each dependency, see if it's
  # already been provided to us by a parent project

  if(NOT TARGET fmtlib::fmtlib)
    cpmaddpackage(
      NAME
        fmt
      GIT_TAG
        "10.2.1"
      GITHUB_REPOSITORY
        "fmtlib/fmt")  
  endif()

  if(NOT TARGET spdlog::spdlog)
    cpmaddpackage(
      NAME
        spdlog
      VERSION
        1.13.0
      GITHUB_REPOSITORY
        "gabime/spdlog"
      OPTIONS
        "SPDLOG_FMT_EXTERNAL ON")
  endif()

  if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage(
      NAME
        Catch2
      VERSION
        3.5.3
      GITHUB_REPOSITORY
        "catchorg/Catch2")
  endif()

  if(NOT TARGET CLI11::CLI11)
    cpmaddpackage(
      NAME
        CLI11
      GITHUB_REPOSITORY
        "CLIUtils/CLI11"
      VERSION
        2.3.2)
  endif()

endfunction()
