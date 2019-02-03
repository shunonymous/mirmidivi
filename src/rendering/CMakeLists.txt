add_library(mirmidivi_PrintMessage MODULE PrintMessage.cpp)
target_link_libraries(mirmidivi_PrintMessage)
install(TARGETS mirmidivi_PrintMessage DESTINATION ${MIRMIDIVI_LIBRARY_DIR})

find_package(Curses)
if(CURSES_HAVE_CURSES_H)
#  add_library(mirmidivi_curses MODULE curses.cpp)
#  target_link_libraries(mirmidivi_curses ${CURSES_LIBRARIES})
#  install(TARGETS mirmidivi_curses DESTINATION ${MIRMIDIVI_LIBRARY_DIR})
endif()

