cmake_minimum_required(VERSION 2.8)
project(mirmidivi CXX C)

option(MIRMIDIVI_INSTALL_DIR "Set install path." /usr)

# Version
set (VERSION_MAJOR "0")
set (VERSION_MINOR "0")
set (VERSION_PATCH "1")
set (VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Set export directories
message("mirmidivi will install to ${MIRMIDIVI_INSTALL_DIR}")
set(MIRMIDIVI_EXECUTABLE_DIR ${MIRMIDIVI_INSTALL_DIR}/bin)
set(MIRMIDIVI_LIBRARY_DIR ${MIRMIDIVI_INSTALL_DIR}/lib)

# PIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Local CMake modules
set(CMAKE_MODULE_PATH
  "${CMAKE_MODULE_PATH}"
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
  )

### Set depended libraries
# Thread
find_package(Threads)
if(CMAKE_USE_PTHREADS_INIT)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
endif()

# Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREAD ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.32 COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

# Poco
include(FindPoco)

find_package(Poco REQUIRED Foundation)
include_directories(${Poco_DIR})

### External projects are here
add_subdirectory(lib/)

### C++ Version
set(CMAKE_CXX_STANDARD 14)

# Build

include_directories(include/)
include_directories(lib/jdksmidi/include/)

add_subdirectory(src/)

