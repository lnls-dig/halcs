########################################################################
# Library setup
########################################################################

# set installation variables compliant with distributions
include(GNUInstallDirs)
# configure and generate *.Config.cmake files
include(CMakePackageConfigHelpers)

# library basename
set (boards_OUTPUT_BASENAME "boards"
    CACHE STRING
    "Output boards library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND boards_LIBRARIES XXX)

########################################################################
# User compile-time options
########################################################################

# should we shrink DDR memory space for AFCv3 or not
option(boards_SHRINK_AFCV3_DDR_SIZE "Enable AFCv3 DDR space shrinking" OFF)
if(boards_SHRINK_AFCV3_DDR_SIZE)
    set(boards_SHRINK_AFCV3_DDR_SIZE_VALUE y)
else()
    set(boards_SHRINK_AFCV3_DDR_SIZE_VALUE n)
endif()

message(STATUS
    "Setting compile-time option boards_SHRINK_AFCV3_DDR_SIZE='${boards_SHRINK_AFCV3_DDR_SIZE_VALUE}'")

########################################################################
# Library compile options
########################################################################

add_library(boards_compiler_flags INTERFACE)

target_link_libraries(boards_compiler_flags
    INTERFACE
    halcs_compiler_flags
)

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Other sources
#######################################

# setting compile options
string(APPEND boards_definitions "")
string(APPEND boards_afcv3_definitions "")
string(APPEND boards_afcv3_1_definitions "")

if(boards_SHRINK_AFCV3_DDR_SIZE_VALUE STREQUAL y)
    list(APPEND boards_afcv3_definitions
        __SHRINK_AFCV3_DDR_SIZE__
    )
    list(APPEND boards_afcv3_1_definitions
        __SHRINK_AFCV3_DDR_SIZE__
    )
endif()

# select board directory to include
string(TOLOWER ${halcs_BOARD_OPT} halcs_BOARD_OPT_LOWER)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${halcs_BOARD_OPT_LOWER})
list(APPEND boards_LIBRARIES boards_${halcs_BOARD_OPT_LOWER})

#######################################
# Our sources
#######################################

# Source files

set(boards_sources
)

# for IDE project generation
source_group("Source Files" FILES
    ${boards_sources}
)

# Header files
list(APPEND boards_other_search_dirs "")

#######################################
# BOARDS header definitions
#######################################

set(boards_public_headers
)

# Get directory names only
list(APPEND boards_search_dirs "")

foreach(search_dir_header ${boards_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND boards_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES boards_search_dirs)

# append to global search list
list(APPEND boards_other_search_dirs ${boards_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${boards_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(boards INTERFACE)

# set compiler flags
target_link_libraries(boards
    INTERFACE
    boards_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(boards
    INTERFACE
    ${boards_LIBRARIES}
    ${halcs_LIBRARIES}
)

########################################################################
# Installer
########################################################################

foreach(target ${boards_LIBRARIES})
    install(TARGETS ${target} boards_compiler_flags
        EXPORT $<TARGET_FILE_BASE_NAME:${target}>-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Libs
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Libs
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Libs
        FRAMEWORK DESTINATION "Library/Frameworks"
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Libs
    )
endforeach()
