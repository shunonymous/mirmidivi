add_executable(01_launch-many-times 01_launch-many-times.cpp)
target_link_libraries(01_launch-many-times dlldr ${CMAKE_THREAD_LIBS_INIT} ParseOptions)

install(TARGETS 01_launch-many-times DESTINATION ${MIRMIDIVI_EXECUTABLE_DIR})
