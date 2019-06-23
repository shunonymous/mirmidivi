add_library(mirmidivi_Msg2Txt SHARED Msg2Txt.cpp)
install(TARGETS mirmidivi_Msg2Txt DESTINATION ${MIRMIDIVI_LIBRARY_DIR})

add_library(mirmidivi_PianoRoll SHARED PianoRoll.cpp)
install(TARGETS mirmidivi_PianoRoll DESTINATION ${MIRMIDIVI_LIBRARY_DIR})
