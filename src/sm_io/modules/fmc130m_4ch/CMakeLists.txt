########################################################################
# Library setup
########################################################################

# library basename
set (sm_io_module_fmc130m_4ch_OUTPUT_BASENAME "sm_io_module_fmc130m_4ch"
    CACHE STRING
    "Output sm_io_module_fmc130m_4ch library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND sm_io_module_fmc130m_4ch_LIBRARIES XXX)

########################################################################
# User compile-time options
########################################################################

# which programming value we want to program on EEPROM.

set(sm_io_module_fmc130m_4ch_EEPROM_PROG_VALUE_OPT "no_program" CACHE STRING
    "FMC130M4CH EEPROM program type")
set(sm_io_module_fmc130m_4ch_EEPROM_PROG_VALUE_VALUES
    "active;passive;no_program"
)

set_property(CACHE sm_io_module_fmc130m_4ch_EEPROM_PROG_VALUE_OPT PROPERTY STRINGS
    ${sm_io_module_fmc130m_4ch_EEPROM_PROG_VALUE_VALUES}
)

message(STATUS
    "Setting compile-time option sm_io_module_fmc130m_4ch_EEPROM_PROG_VALUE='${sm_io_module_fmc130m_4ch_EEPROM_PROG_VALUE_OPT}'")

########################################################################
# Library compile options
########################################################################

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Our sources
#######################################

# Source files

set(sm_io_module_fmc130m_4ch_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_fmc130m_4ch_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_fmc130m_4ch_exp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_fmc130m_4ch_defaults.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${sm_io_module_fmc130m_4ch_sources}
)

# Header files
list(APPEND sm_io_module_fmc130m_4ch_other_search_dirs "")

#######################################
# SMIO header definitions
#######################################

set(sm_io_module_fmc130m_4ch_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_fmc130m_4ch_core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_fmc130m_4ch_exp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_fmc130m_4ch_defaults.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_fmc130m_4ch_codes.h
)

# Get directory names only
list(APPEND sm_io_module_fmc130m_4ch_search_dirs "")

foreach(search_dir_header ${sm_io_module_fmc130m_4ch_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND sm_io_module_fmc130m_4ch_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES sm_io_module_fmc130m_4ch_search_dirs)

# append to global search list
list(APPEND sm_io_module_fmc130m_4ch_other_search_dirs ${sm_io_module_fmc130m_4ch_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${sm_io_module_fmc130m_4ch_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(sm_io_module_fmc130m_4ch STATIC
    ${sm_io_module_fmc130m_4ch_sources}
    ${sm_io_module_fmc130m_4ch_public_headers}
)

set_target_properties(sm_io_module_fmc130m_4ch PROPERTIES
  PUBLIC_HEADER "${sm_io_module_fmc130m_4ch_public_headers}"
  OUTPUT_NAME "${sm_io_module_fmc130m_4ch_OUTPUT_BASENAME}"
  PREFIX "lib")

# sm_io_module_fmc130m_4ch_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(sm_io_module_fmc130m_4ch
    PRIVATE
    "$<BUILD_INTERFACE:${sm_io_module_fmc130m_4ch_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# setting compile options
string(APPEND sm_io_module_fmc130m_4ch_board_definitions "")

# setting board macros
if(sm_io_module_fmc130m_4ch_EEPROM_PROG_VALUE_OPT STREQUAL active)
    list(APPEND sm_io_module_fmc130m_4ch_board_definitions
        __FMC130M_4CH_EEPROM_PROGRAM__=1
    )
elseif(sm_io_module_fmc130m_4ch_EEPROM_PROG_VALUE_OPT STREQUAL passive)
    list(APPEND sm_io_module_fmc130m_4ch_board_definitions
        __FMC130M_4CH_EEPROM_PROGRAM__=2
    )
# no define for no_program
endif()

target_compile_definitions(sm_io_module_fmc130m_4ch PRIVATE
    ${sm_io_module_fmc130m_4ch_board_definitions}
)

# set debug posfix in case of debug builds
set_target_properties(sm_io_module_fmc130m_4ch PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(sm_io_module_fmc130m_4ch
    PRIVATE
    sm_io_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(sm_io_module_fmc130m_4ch
    PRIVATE
    ${sm_io_module_fmc130m_4ch_LIBRARIES}
    ${halcs_LIBRARIES}
)
