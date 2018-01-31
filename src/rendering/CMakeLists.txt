cmake_minimum_required(VERSION 2.8)
project(mirmidivi)

find_package(RtMidi)
if(RTMIDI_FOUND)
  add_library(PrintMessage MODULE PrintMessage.cpp)
  target_link_libraries(PrintMessage jdksmidi mirmidivi_midi)
  install(TARGETS PrintMessage DESTINATION ${MIRMIDIVI_LIBRARY_DIR})

  find_package(Curses)
  if(CURSES_HAVE_CURSES_H)
    add_library(curses MODULE curses.cpp)
    target_link_libraries(curses jdksmidi mirmidivi_midi ${CURSES_LIBRARIES})
    install(TARGETS curses DESTINATION ${MIRMIDIVI_LIBRARY_DIR})
  endif()
endif()

