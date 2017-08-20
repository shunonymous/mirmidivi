cmake_minimum_required(VERSION 2.8)
project(mirmidivi)

find_package(RtMidi)
if(RTMIDI_FOUND)
  add_library(PrintMessage MODULE PrintMessage.cpp)
  target_link_libraries(PrintMessage jdksmidi mirmidivi_midi)
  install(TARGETS PrintMessage DESTINATION ${MIRMIDIVI_LIBRARY_DIR})
endif()
