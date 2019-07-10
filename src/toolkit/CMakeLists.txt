add_library(mirmidivi_no_toolkit MODULE no-toolkit.cpp)
target_link_libraries(mirmidivi_no_toolkit ${CMAKE_THREAD_LIBS_INIT} dlldr mirmidivi_fluidsynth)
install(TARGETS mirmidivi_no_toolkit DESTINATION ${MIRMIDIVI_LIBRARY_DIR})

