include(GenerateExportHeader)


add_library(pcb_library 
pcb_library.cpp 
get_prefix.cpp
get_board_info.cpp
get_data_file_path.cpp
)



add_library(pcbtools::pcb_library ALIAS pcb_library)

target_link_libraries(pcb_library PRIVATE pcbtools_options pcbtools_warnings)

target_include_directories(pcb_library ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include> ${PROJECT_SOURCE_DIR}/external/gsl-lite/include)

target_compile_features(pcb_library PUBLIC cxx_std_20)

set_target_properties(
  pcb_library
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(pcb_library EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/pcbtools/pcb_library_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(pcb_library PUBLIC pcb_library_STATIC_DEFINE)
endif()
