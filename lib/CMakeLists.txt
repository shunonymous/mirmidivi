### Build external libraries

# dlldr
add_subdirectory(dlldr/)
install(TARGETS dlldr OPTIONAL DESTINATION ${MIRMIDIVI_LIBRARY_DIR})

# Library for mirmidivi
#add_subdirectory(mirmidivi/)
