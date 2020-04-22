#
# CMakeFixCPackPaths.cmake
#
# Fix CMake DESTDIR path so in CPack strings that use DESTDIR can be
# used as "CMAKE_FINAL_DESTDIR" and it will point
# to the final DESTDIR and not the the CPack intermediate DESTDIR

# In a CPACK_PROJECT_CONFIG_FILE, CPACK_GENERATOR is a string naming a
# single generator. If nothing is defined it means that we are not in
# CPack build. Actually, this file will simply not be executed. So, the
# else() does not really happen.
if(DEFINED CPACK_GENERATOR)
    set(CMAKE_FINAL_DESTDIR ${CPACK_PACKAGING_INSTALL_PREFIX})
else()
    set(CMAKE_FINAL_DESTDIR $ENV{DESTDIR})
endif()
