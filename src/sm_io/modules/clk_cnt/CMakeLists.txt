# library basename
set (sm_io_module_clk_cnt_OUTPUT_BASENAME "sm_io_module_clk_cnt"
    CACHE STRING
    "Output sm_io_module_clk_cnt library base name"
)

# Source files
set(sm_io_module_clk_cnt_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_clk_cnt_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_clk_cnt_exp.c
)

# Header files
list(APPEND sm_io_module_clk_cnt_other_search_dirs "")

#######################################
# SMIO header definitions
#######################################

set(sm_io_module_clk_cnt_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_clk_cnt_core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_clk_cnt_exp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_clk_cnt_codes.h
)

# Get directory names only
list(APPEND sm_io_module_clk_cnt_search_dirs "")

foreach(search_dir_header ${sm_io_module_clk_cnt_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND sm_io_module_clk_cnt_search_dirs ${search_dir})
endforeach()
list(REMOVE_DUPLICATES sm_io_module_clk_cnt_search_dirs)

# append to global search list
list(APPEND sm_io_module_clk_cnt_other_search_dirs ${sm_io_module_clk_cnt_search_dirs})

# build static library
add_library(sm_io_module_clk_cnt STATIC
    ${sm_io_module_clk_cnt_sources}
    ${sm_io_module_clk_cnt_public_headers}
)

set_target_properties(sm_io_module_clk_cnt PROPERTIES
  PUBLIC_HEADER "${sm_io_module_clk_cnt_public_headers}"
  OUTPUT_NAME "${sm_io_module_clk_cnt_OUTPUT_BASENAME}"
  PREFIX "lib")

target_include_directories(sm_io_module_clk_cnt
    PRIVATE
    "$<BUILD_INTERFACE:${sm_io_module_clk_cnt_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# set debug posfix in case of debug builds
set_target_properties(sm_io_module_clk_cnt PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(sm_io_module_clk_cnt
    PRIVATE
    sm_io_compiler_flags
)

# set depedency to targets
target_link_libraries(sm_io_module_clk_cnt
    PRIVATE
    ${sm_io_module_clk_cnt_LIBRARIES}
    ${halcs_LIBRARIES}
)
