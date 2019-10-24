add_executable(01_launch-many-times 01_launch-many-times.cpp)
target_link_libraries(01_launch-many-times dlldr ${CMAKE_THREAD_LIBS_INIT} ParseOptions)

add_executable(02_multi-synth-instance 02_multi-synth-instance.cpp)
target_link_libraries(02_multi-synth-instance dlldr ${CMAKE_THREAD_LIBS_INIT} ParseOptions mirmidivi_fluidsynth)

install(TARGETS 01_launch-many-times 02_multi-synth-instance DESTINATION ${MIRMIDIVI_EXECUTABLE_DIR})
