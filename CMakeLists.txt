cmake_minimum_required(VERSION 3.22)
project(bgk2dSYCL)

set(CMAKE_CXX_STANDARD 20)

# Fetch MDSpan
include(FetchContent)
FetchContent_Declare(
  mdspan
  GIT_REPOSITORY https://github.com/kokkos/mdspan.git
  GIT_TAG stable)
FetchContent_MakeAvailable(mdspan)

# Fetch fmt for I/O
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

# FetchContent for frozen
FetchContent_Declare(
  frozen
  GIT_REPOSITORY https://github.com/serge-sans-paille/frozen
  GIT_TAG        master) # Use the master branch
FetchContent_MakeAvailable(frozen)

set(header_files
include/bgk2d/storage.hh
include/bgk2d/precision.hh
include/bgk2d/get_mem.hh
include/bgk2d/file_manager.hh
include/bgk2d/time.hh)

set(source_files
src/bgk2d.cc
src/get_mem.cc
src/file_manager.cc
src/storage.cc
src/time.cc)


add_executable(bgk2dSYCL ${header_files} ${source_files})

get_property(
  mdspan_INCLUDE_DIRS
  TARGET std::mdspan
  PROPERTY INTERFACE_INCLUDE_DIRECTORIES)


get_property(
  frozen_INCLUDE_DIRS
  TARGET frozen
  PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
  
target_include_directories(bgk2dSYCL PUBLIC include/bgk2d ${mdspan_INCLUDE_DIRS} ${frozen_INCLUDE_DIRS})
target_link_libraries(bgk2dSYCL fmt::fmt)