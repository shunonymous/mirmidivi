cmake_minimum_required(VERSION 2.8)
project(mirmidivi)

find_package(RtMidi)
if(RTMIDI_FOUND)
  add_library(RtMidi MODULE RtMidi.cpp)
  target_link_libraries(RtMidi ${RTMIDI_LIBRARY} jdksmidi)
  install(TARGETS RtMidi DESTINATION ${MIRMIDIVI_LIBRARY_DIR})
endif()
