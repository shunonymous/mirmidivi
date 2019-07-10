add_library(ParseOptions STATIC ParseOptions.cpp)
target_link_libraries(ParseOptions ${Boost_LIBRARIES})

add_executable(mirmidivi mirmidivi.cpp)
target_link_libraries(mirmidivi dlldr ${CMAKE_THREAD_LIBS_INIT} ParseOptions)

install(TARGETS mirmidivi DESTINATION ${MIRMIDIVI_EXECUTABLE_DIR})
