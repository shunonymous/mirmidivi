add_library(mirmidivi_Msg2Txt SHARED Msg2Txt.cpp)
target_link_libraries(mirmidivi_Msg2Txt mirmidivi_fluidsynth)
install(TARGETS mirmidivi_Msg2Txt DESTINATION ${MIRMIDIVI_LIBRARY_DIR})

add_library(mirmidivi_PianoRoll SHARED PianoRoll.cpp)
target_link_libraries(mirmidivi_PianoRoll mirmidivi_fluidsynth)
install(TARGETS mirmidivi_PianoRoll DESTINATION ${MIRMIDIVI_LIBRARY_DIR})
