cmake_minimum_required(VERSION 2.8)
project(mirmidivi)

if(RTMIDI_FOUND)
  add_subdirectory(RtMidi/)
endif()
