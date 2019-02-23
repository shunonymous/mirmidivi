### Build external libraries

# dlldr
set(DLLDR_NO_BUILD_EXAMPLE true)
add_subdirectory(dlldr/)
install(TARGETS dlldr OPTIONAL DESTINATION ${MIRMIDIVI_LIBRARY_DIR})

# Library for mirmidivi
#add_subdirectory(mirmidivi/)
