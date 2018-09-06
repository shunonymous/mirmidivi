cmake_minimum_required(VERSION 2.8)
project(mirmidivi)

find_package(RtMidi)
if(RTMIDI_FOUND)
  add_library(mirmidivi_PrintMessage MODULE PrintMessage.cpp)
  target_link_libraries(mirmidivi_PrintMessage jdksmidi)
  install(TARGETS mirmidivi_PrintMessage DESTINATION ${MIRMIDIVI_LIBRARY_DIR})

  find_package(Curses)
  if(CURSES_HAVE_CURSES_H)
    add_library(mirmidivi_curses MODULE curses.cpp)
    target_link_libraries(mirmidivi_curses jdksmidi ${CURSES_LIBRARIES})
    install(TARGETS mirmidivi_curses DESTINATION ${MIRMIDIVI_LIBRARY_DIR})
  endif()
endif()

