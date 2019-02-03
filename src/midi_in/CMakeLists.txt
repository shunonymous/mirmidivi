cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)
pkg_check_modules(FLUIDSYNTH REQUIRED fluidsynth)

include_directories(${FLUIDSYNTH_INCLUDE_DIRS})
add_library(mirmidivi_fluidsynth STATIC fluidsynth.cpp)
target_link_libraries(mirmidivi_fluidsynth ${FLUIDSYNTH_LIBRARIES})
