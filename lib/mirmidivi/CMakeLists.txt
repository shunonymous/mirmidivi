
cmake_minimum_required(VERSION 2.8)
project(mirmidivi)

find_package(RtMidi)
if(RTMIDI_FOUND)
  include_directories(${RTMIDI_INCLUDE_DIR})
  add_library(mirmidivi_midi SHARED midi.cpp)
  target_link_libraries(mirmidivi_midi ${RTMIDI_LIBRARY} jdksmidi)
  install(TARGETS mirmidivi_midi DESTINATION ${MIRMIDIVI_LIBRARY_DIR})
endif()
