cmake_minimum_required(VERSION 2.8)
project(mirmidivi)

if(RTMIDI_FOUND)
  add_library(mirmidivi_RtMidi MODULE RtMidi.cpp)
  target_link_libraries(mirmidivi_RtMidi ${RTMIDI_LIBRARY} jdksmidi)
  install(TARGETS mirmidivi_RtMidi DESTINATION ${MIRMIDIVI_LIBRARY_DIR})
endif()
