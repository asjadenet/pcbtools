add_executable(pcb main.cpp)

target_link_libraries(
  pcb
  PRIVATE pcbtools::pcb_library 
          pcbtools::pcbtools_options
          pcbtools::pcbtools_warnings)

target_link_system_libraries(
  pcb
  PRIVATE
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog
)

target_include_directories(pcb PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
