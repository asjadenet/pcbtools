include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(pcbtools_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(pcbtools_setup_options)
  option(pcbtools_ENABLE_HARDENING "Enable hardening" ON)
  option(pcbtools_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    pcbtools_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    pcbtools_ENABLE_HARDENING
    OFF)

  pcbtools_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR pcbtools_PACKAGING_MAINTAINER_MODE)
    option(pcbtools_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(pcbtools_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(pcbtools_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(pcbtools_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(pcbtools_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(pcbtools_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(pcbtools_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(pcbtools_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(pcbtools_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(pcbtools_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(pcbtools_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(pcbtools_ENABLE_PCH "Enable precompiled headers" OFF)
    option(pcbtools_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(pcbtools_ENABLE_IPO "Enable IPO/LTO" ON)
    option(pcbtools_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(pcbtools_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(pcbtools_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(pcbtools_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(pcbtools_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(pcbtools_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(pcbtools_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(pcbtools_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(pcbtools_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(pcbtools_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(pcbtools_ENABLE_PCH "Enable precompiled headers" OFF)
    option(pcbtools_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      pcbtools_ENABLE_IPO
      pcbtools_WARNINGS_AS_ERRORS
      pcbtools_ENABLE_USER_LINKER
      pcbtools_ENABLE_SANITIZER_ADDRESS
      pcbtools_ENABLE_SANITIZER_LEAK
      pcbtools_ENABLE_SANITIZER_UNDEFINED
      pcbtools_ENABLE_SANITIZER_THREAD
      pcbtools_ENABLE_SANITIZER_MEMORY
      pcbtools_ENABLE_UNITY_BUILD
      pcbtools_ENABLE_CLANG_TIDY
      pcbtools_ENABLE_CPPCHECK
      pcbtools_ENABLE_COVERAGE
      pcbtools_ENABLE_PCH
      pcbtools_ENABLE_CACHE)
  endif()

  pcbtools_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (pcbtools_ENABLE_SANITIZER_ADDRESS OR pcbtools_ENABLE_SANITIZER_THREAD OR pcbtools_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(pcbtools_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(pcbtools_global_options)
  if(pcbtools_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    pcbtools_enable_ipo()
  endif()

  pcbtools_supports_sanitizers()

  if(pcbtools_ENABLE_HARDENING AND pcbtools_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR pcbtools_ENABLE_SANITIZER_UNDEFINED
       OR pcbtools_ENABLE_SANITIZER_ADDRESS
       OR pcbtools_ENABLE_SANITIZER_THREAD
       OR pcbtools_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${pcbtools_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${pcbtools_ENABLE_SANITIZER_UNDEFINED}")
    pcbtools_enable_hardening(pcbtools_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(pcbtools_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(pcbtools_warnings INTERFACE)
  add_library(pcbtools_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  pcbtools_set_project_warnings(
    pcbtools_warnings
    ${pcbtools_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(pcbtools_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(pcbtools_options)
  endif()

  include(cmake/Sanitizers.cmake)
  pcbtools_enable_sanitizers(
    pcbtools_options
    ${pcbtools_ENABLE_SANITIZER_ADDRESS}
    ${pcbtools_ENABLE_SANITIZER_LEAK}
    ${pcbtools_ENABLE_SANITIZER_UNDEFINED}
    ${pcbtools_ENABLE_SANITIZER_THREAD}
    ${pcbtools_ENABLE_SANITIZER_MEMORY})

  set_target_properties(pcbtools_options PROPERTIES UNITY_BUILD ${pcbtools_ENABLE_UNITY_BUILD})

  if(pcbtools_ENABLE_PCH)
    target_precompile_headers(
      pcbtools_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(pcbtools_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    pcbtools_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(pcbtools_ENABLE_CLANG_TIDY)
    pcbtools_enable_clang_tidy(pcbtools_options ${pcbtools_WARNINGS_AS_ERRORS})
  endif()

  if(pcbtools_ENABLE_CPPCHECK)
    pcbtools_enable_cppcheck(${pcbtools_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(pcbtools_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    pcbtools_enable_coverage(pcbtools_options)
  endif()

  if(pcbtools_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(pcbtools_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(pcbtools_ENABLE_HARDENING AND NOT pcbtools_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR pcbtools_ENABLE_SANITIZER_UNDEFINED
       OR pcbtools_ENABLE_SANITIZER_ADDRESS
       OR pcbtools_ENABLE_SANITIZER_THREAD
       OR pcbtools_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    pcbtools_enable_hardening(pcbtools_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
